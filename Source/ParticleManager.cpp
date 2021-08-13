//
//  Physics.cpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#include "ParticleManager.h"

namespace synchrony {

bool ParticleManager::AddParticle(Particle& new_particle) {
  if (!paused_ && particles_.size() < MAX_PARTICLES) {
    new_particle.SetId(current_particle_id_++);
    particles_.push_back(std::make_unique<Particle>(new_particle));
    
    addAndMakeVisible(particles_.back().get());
    AxisAlignedBoundingBox* new_particle_box = particles_.back()->GetBoundingBox();
    positions_x_.push_back(new_particle_box->bounds_x.first);
    positions_x_.push_back(new_particle_box->bounds_x.second);
    positions_y_.push_back(new_particle_box->bounds_y.first);
    positions_y_.push_back(new_particle_box->bounds_y.second);
    
    return true;
  }
  
  return false;
}

void ParticleManager::update() {
  AddParticlesFromMidiMessages();

  auto iter = particles_.begin();
  while (iter != particles_.end()) {
    auto& particle = *iter;
    
    if (particle->IsRemoved()) {
      RemoveParticle(particle);
      particles_.erase(iter++);
      continue;
    }
    
    if (!(paused_ || adding_particle_)) {
      particle->UpdatePosition();
    }

    ++iter;
  }
  
  if (particles_.size() > 1 && !paused_ && !adding_particle_) {
    FindCollisions();
    if (!collision_candidate_pairs_.empty()) {
      ResolveCollisions();
    }
  }
}

void ParticleManager::mouseDown(const juce::MouseEvent& event) {
  if (event.originalComponent == this &&
      event.getLengthOfMousePress() < kMouseHoldMSToAddParticle) {
    Particle to_add = Particle(ap_,
                               event.getPosition(),
                               juce::Point<int>(1, 1),
                               kDragParticleRadius);
    adding_particle_ = AddParticle(to_add);
  }
}

void ParticleManager::mouseUp(const juce::MouseEvent& event) {
  if (event.originalComponent == this &&
      adding_particle_ &&
      event.getLengthOfMousePress() < kMouseHoldMSToAddParticle) {
    auto new_velocity = juce::Point<float>(1, 1);
    particles_.back()->SetVelocity(new_velocity);
  } else if (event.originalComponent == this &&
             adding_particle_) {
    auto new_velocity = event.getOffsetFromDragStart().toFloat() *
                        kDragParticleVelocityMultiplier;
    particles_.back()->SetVelocity(new_velocity);
  }
  adding_particle_ = false;
}

void ParticleManager::TogglePause() {
  paused_ = !paused_;
}

void ParticleManager::Reset() {
  overlap_matrix_ = std::unique_ptr<OverlapMatrix>(new OverlapMatrix());
  current_particle_id_ = 0;
  particles_.clear();
  positions_x_.clear();
  positions_y_.clear();
  collision_candidate_pairs_.clear();
  paused_ = false;
}

void ParticleManager::paint(juce::Graphics& g) {
  g.fillAll((paused_ || adding_particle_) ?
            juce::Colours::darkgrey : juce::Colours::black);

  if (SynchronySettings::ShouldShowAABBsAndPairs()) {
    for (auto& pair : collision_candidate_pairs_) {
      g.setColour(juce::Colours::white);
      g.drawLine(pair.first->GetCurrentPosition().x(), pair.first->GetCurrentPosition().y(), pair.second->GetCurrentPosition().x(), pair.second->GetCurrentPosition().y());
    }
  }
}

bool ParticleManager::AddParticlesFromMidiMessages() {
  while (!ap_.midi_in_message_queue_.empty()) {
    MidiData message = ap_.midi_in_message_queue_.front();

    int radius = int(std::ceilf((MAX_RADIUS - MIN_RADIUS)
                                * (128 - message.note_num_) / 128.0f
                                + MIN_RADIUS));
    juce::Point<int> init_pos((int) random_generator_() % (getWidth() - radius) + radius,
                              (int) random_generator_() % (getHeight() - radius) + radius);
    juce::Point<int> init_vel = int(kMidiParticleVelocityMultiplier * message.velocity_) *
                                juce::Point<int>(random_generator_() % 2,
                                                 random_generator_() % 2);
    auto color = kParticleColors[size_t(message.note_num_ % int(kParticleColors.size()))];
    Particle to_add = Particle(ap_,
                               init_pos,
                               init_vel,
                               radius,
                               color,
                               message);
    ap_.midi_in_message_queue_.pop_front();
    if (!AddParticle(to_add)) return false;
  }
  
  return true;
}

void ParticleManager::FindCollisions() {
  SortAxisAndFindCandidates(positions_x_, true);
  SortAxisAndFindCandidates(positions_y_, false);
}

void ParticleManager::ResolveCollisions() {
  std::list<std::pair<Particle*, Particle*>>::iterator iter =
      collision_candidate_pairs_.begin();
  size_t idx = 0;
  
  // do while candidates is populated and iterator not exhausted
  while (!collision_candidate_pairs_.empty() &&
         iter != collision_candidate_pairs_.end()) {
    Particle* p1 = (*iter).first;
    Particle* p2 = (*iter).second;
    
    if (!(GetOverlapMatrix(false, p1->GetId(), p2->GetId()) &&
          GetOverlapMatrix(true, p1->GetId(), p2->GetId()))) {
      // particles no longer in striking range
      collision_candidate_pairs_.erase(iter++);
      ++idx;
      continue;
    }
    
    // Handle collision between particles
    if (Particle::DoParticlesCollide(p1, p2)) {
      Particle::SetCollisionVelocity(p1, p2);
    }
    
    ++idx;
    ++iter;
  }
}

void ParticleManager::SortAxisAndFindCandidates(std::vector<EndPoint*>& axis, bool is_x_axis) {
  for (size_t key_idx = 1; key_idx < axis.size(); ++key_idx) {
    EndPoint* key = axis.at(key_idx);
    
    size_t swapper_idx = key_idx;
    while (swapper_idx > 0 &&
           axis[swapper_idx - 1]->GetValue() >= axis[swapper_idx]->GetValue()) {
      EndPoint* to_swap = axis[swapper_idx - 1];
      size_t key_id = key->owner->GetId();
      size_t swapper_id = to_swap->owner->GetId();

      if (key->owner != to_swap->owner) {
        if (!to_swap->is_min && key->is_min) {
          // upper bound moves ahead of a lower bound
          SetOverlapMatrix(is_x_axis, key_id, swapper_id, true);
          
          if (GetOverlapMatrix(false, key_id, swapper_id) &&
              GetOverlapMatrix(true, key_id, swapper_id)) {
            collision_candidate_pairs_.push_back(
                std::pair<Particle*, Particle*>(key->owner, to_swap->owner));
          }
        }
        
        if (to_swap->is_min && !key->is_min) {
          // lower bound moves ahead of an upper bound
          SetOverlapMatrix(is_x_axis, key_id, swapper_id, false);
        }
      }

      axis[swapper_idx - 1] = axis[swapper_idx];
      axis[swapper_idx] = to_swap;
      
      --swapper_idx;
    }
  }
}

void ParticleManager::RemoveParticle(std::unique_ptr<Particle>& particle) {
  particle->UpdatePosition();
  removeChildComponent(particle.get());
  for (size_t p_idx = 0; p_idx < particles_.size(); ++p_idx) {
    SetOverlapMatrix(true, particle->GetId(), p_idx, false);
    SetOverlapMatrix(false, particle->GetId(), p_idx, false);
  }
  FindCollisions();
  if (!collision_candidate_pairs_.empty()) {
    ResolveCollisions();
  }
  positions_x_.pop_back();
  positions_x_.pop_back();
  positions_y_.pop_back();
  positions_y_.pop_back();
}

void ParticleManager::SetOverlapMatrix(bool is_x_axis, size_t i, size_t j, bool value) {
  if (is_x_axis) {
    if (i > j) {
      overlap_matrix_->at(0)[MAX_PARTICLES * j + i] = value;
    } else {
      overlap_matrix_->at(0)[MAX_PARTICLES * i + j] = value;
    }
  } else {
    if (i > j) {
      overlap_matrix_->at(1)[MAX_PARTICLES * j + i] = value;
    } else {
      overlap_matrix_->at(1)[MAX_PARTICLES * i + j] = value;
    }
  }
}

bool ParticleManager::GetOverlapMatrix(bool is_x_axis, size_t i, size_t j) const {
  if (is_x_axis) {
    if (i > j) {
      return overlap_matrix_->at(0)[MAX_PARTICLES * j + i];
    } else {
      return overlap_matrix_->at(0)[MAX_PARTICLES * i + j];
    }
  } else {
    if (i > j) {
      return overlap_matrix_->at(1)[MAX_PARTICLES * j + i];
    } else {
      return overlap_matrix_->at(1)[MAX_PARTICLES * i + j];
    }
  }
}

} // namespace synchrony

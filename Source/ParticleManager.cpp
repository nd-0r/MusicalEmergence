//
//  Physics.cpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#include "ParticleManager.h"

namespace synchrony {

void ParticleManager::AddParticle(Particle& new_particle) {
  if (!paused_) {
    new_particle.SetId(current_particle_id_++);
    particles_.push_back(std::make_unique<Particle>(new_particle));
    
    addAndMakeVisible(particles_.back().get());
    AxisAlignedBoundingBox* new_particle_box = particles_.back()->GetBoundingBox();
    positions_x_.push_back(new_particle_box->bounds_x.first);
    positions_x_.push_back(new_particle_box->bounds_x.second);
    positions_y_.push_back(new_particle_box->bounds_y.first);
    positions_y_.push_back(new_particle_box->bounds_y.second);
  }
}

void ParticleManager::update() {
  auto iter = particles_.begin();
  while (iter != particles_.end()) {
    auto& particle = *iter;
    
    if (particle->IsRemoved()) {
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
      particles_.erase(iter++);
      continue;
    }
    
    if (!paused_) {
      particle->UpdatePosition();
      
      // Keep particles in bounds
      float x_vel = particle->GetVelocity().x();
      float y_vel = particle->GetVelocity().y();
      
      if ((y_vel < 0 && particle->GetCurrentPosition().y() - particle->GetRadius() <= 0) ||
          (y_vel > 0 && particle->GetCurrentPosition().y() + particle->GetRadius() >= getHeight())) {
        y_vel *= -1;
        particle->SetVelocity(vmml::Vector2f(x_vel, y_vel));
      }
      
      if ((x_vel < 0 && particle->GetCurrentPosition().x() - particle->GetRadius() < 0) ||
          (x_vel > 0 && particle->GetCurrentPosition().x() + particle->GetRadius() > getWidth())) {
        x_vel *= -1;
        particle->SetVelocity(vmml::Vector2f(x_vel, y_vel));
      }
    }

    ++iter;
  }
  
  if (particles_.size() > 1 && !paused_) {
    FindCollisions();
    if (!collision_candidate_pairs_.empty()) {
      ResolveCollisions();
    }
  }
}

void ParticleManager::TogglePause() {
  paused_ = !paused_;
}

void ParticleManager::paint(juce::Graphics& g) {
  g.fillAll(juce::Colours::black);
  
//  // TODO - remove
//  for (auto& pair : collision_candidate_pairs_) {
//    g.setColour(juce::Colours::white);
//    g.drawLine(pair.first->GetCurrentPosition().x(), pair.first->GetCurrentPosition().y(), pair.second->GetCurrentPosition().x(), pair.second->GetCurrentPosition().y());
//  }
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
      vmml::Vector2f vel1_new = Particle::CalcCollisionVelocity(p1, p2);
      vmml::Vector2f vel2_new = Particle::CalcCollisionVelocity(p2, p1);
      
      p1->SetVelocity(vel1_new);
      p2->SetVelocity(vel2_new);
    }
    
    ++idx;
    ++iter;
  }
}

//void ParticleManager::RemoveParticle(const Particle& particle_to_remove) {
//
//}

void ParticleManager::Reset() {
  overlap_matrix_ = std::unique_ptr<OverlapMatrix>(new OverlapMatrix());
  current_particle_id_ = 0;
  particles_.clear();
  positions_x_.clear();
  positions_y_.clear();
  collision_candidate_pairs_.clear();
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

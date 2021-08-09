//
//  Physics.cpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#include "ParticleManager.h"

namespace synchrony {

void ParticleManager::AddParticle(Particle& new_particle) {
  new_particle.SetId(current_particle_id_++);
  particles_.push_back(std::make_unique<Particle>(new_particle));
  
  addAndMakeVisible(particles_.back().get());
  AxisAlignedBoundingBox* new_particle_box = particles_.back()->GetBoundingBox();
  InsertSorted(positions_x_, new_particle_box->bounds_x);
  InsertSorted(positions_y_, new_particle_box->bounds_y);
}

void ParticleManager::update() {
  for (auto& particle : particles_) {
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
  
  if (particles_.size() > 1) {
    FindCollisions();
    if (!collision_candidate_pairs_.empty()) {
      ResolveCollisions();
    }
  }
}

void ParticleManager::paint(juce::Graphics& g) {
  g.fillAll(juce::Colours::black);
  
  // TODO - remove
  for (auto& pair : collision_candidate_pairs_) {
    g.setColour(juce::Colours::white);
    g.drawLine(pair.first->GetCurrentPosition().x(), pair.first->GetCurrentPosition().y(), pair.second->GetCurrentPosition().x(), pair.second->GetCurrentPosition().y());
  }
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
    
    // Handle collision between particles
    if (Particle::DoParticlesCollide(p1, p2)) {
      vmml::Vector2f vel1_new = Particle::CalcCollisionVelocity(p1, p2);
      vmml::Vector2f vel2_new = Particle::CalcCollisionVelocity(p2, p1);
      
      p1->SetVelocity(vel1_new);
      p2->SetVelocity(vel2_new);
    }
    
    if (!(overlap_matrix_[0][p1->GetId()][p2->GetId()] &&
          overlap_matrix_[1][p1->GetId()][p2->GetId()])) {
      // particles no longer in striking range
      collision_candidate_pairs_.erase(iter++);
      ++idx;
      continue;
    }
    ++idx;
    ++iter;
  }
}

//void ParticleManager::RemoveParticle(const Particle& particle_to_remove) {
//
//}

void ParticleManager::Reset() {
  memset(overlap_matrix_, 0, sizeof(overlap_matrix_));
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
           axis[swapper_idx - 1]->GetValue() > axis[swapper_idx]->GetValue()) {
      EndPoint* to_swap = axis[swapper_idx - 1];
      int key_id = key->owner->GetId();
      int swapper_id = to_swap->owner->GetId();

      if (key->owner != to_swap->owner) {
        if (!to_swap->is_min && key->is_min) {
          // upper bound moves ahead or a lower bound or
          // lower bound moves ahead of lower bound
          if (is_x_axis) {
            overlap_matrix_[0][key_id][swapper_id] = true;
          } else {
            overlap_matrix_[1][key_id][swapper_id] = true;
          }
          
          if (overlap_matrix_[0][key_id][swapper_id] &&
              overlap_matrix_[1][key_id][swapper_id]) {
            collision_candidate_pairs_.push_back(
                std::pair<Particle*, Particle*>(key->owner, to_swap->owner));
          }
        }
        
        if (to_swap->is_min && !key->is_min) {
          // lower bound moves ahead of upper bound
          if (is_x_axis) {
            overlap_matrix_[0][key_id][swapper_id] = false;
          } else {
            overlap_matrix_[1][key_id][swapper_id] = false;
          }
        }
      }

      axis[swapper_idx - 1] = axis[swapper_idx];
      axis[swapper_idx] = to_swap;
      
      --swapper_idx;
    }
  }
}

void ParticleManager::InsertSorted(std::vector<EndPoint*>& end_points,
                                   const std::pair<EndPoint*, EndPoint*>& bounds,
                                   size_t start_idx) {
  EndPoint* point_to_insert = (start_idx == 0) ? bounds.first : bounds.second;
  
  std::vector<EndPoint*>::iterator iter;
  size_t idx = start_idx;
  for (iter = end_points.begin() + static_cast<long>(start_idx);
       iter != end_points.end();
       ++iter, ++idx) {
    if (point_to_insert->GetValue() >= (*iter)->GetValue()) {
      end_points.insert(iter, point_to_insert);
      
      if (start_idx == 0) {
        InsertSorted(end_points, bounds, idx + 1);
      }
      
      return;
    }
  }
  
  end_points.push_back(point_to_insert);
  
  if (start_idx == 0) {
    InsertSorted(end_points, bounds, 1);
  }
}

} // namespace synchrony

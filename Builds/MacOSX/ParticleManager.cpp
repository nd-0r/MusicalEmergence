//
//  Physics.cpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#include "ParticleManager.hpp"

namespace synchrony {

void ParticleManager::AddParticle(const Particle& new_particle) {
  particles_.push_back(new_particle);
  
  AxisAlignedBoundingBox* new_particle_box = new_particle.GetBoundingBox();
  InsertSorted(positions_x_, new_particle_box->bounds_x);
  InsertSorted(positions_y_, new_particle_box->bounds_y);
}

void ParticleManager::UpdateParticles() {
  for (Particle& particle : particles_) {
    particle.UpdatePosition();
  }
}

void ParticleManager::FindCollisions() {
  SortAxisAndFindCandidates(positions_x_);
  SortAxisAndFindCandidates(positions_y_);
}

void ParticleManager::InsertSorted(std::vector<EndPoint*>& end_points,
                                   const std::pair<EndPoint*, EndPoint*>& bounds,
                                   size_t start_idx = 0) {
  EndPoint* point_to_insert = (start_idx == 0) ? bounds.first : bounds.second;
  
  if (end_points.empty()) {
    end_points.push_back(point_to_insert);
    
    if (start_idx == 0) InsertSorted(end_points, bounds, 1);
  }
  
  std::vector<EndPoint*>::iterator iter;
  for (iter = end_points.begin() + start_idx;
       iter != end_points.end();
       ++iter) {
    if (point_to_insert->value >= (*iter)->value) {
      end_points.insert(iter, point_to_insert);
      
      if (start_idx == 0) InsertSorted(end_points, bounds, iter - end_points.begin());
    }
  }
}

void ParticleManager::SortAxisAndFindCandidates(std::vector<EndPoint*>& axis) {
  for (size_t key_idx = 0; key_idx < axis.size(); ++key_idx) {
    EndPoint* key = axis.at(key_idx);
    
    size_t swapper_idx = key_idx - 1;
    while (axis[swapper_idx]->value > key->value && swapper_idx > 0) {
      EndPoint* to_swap = axis[swapper_idx];
      
      if (key->is_min && !to_swap->is_min) { // lower bound moves ahead another lower bound
        if (!(key->owner->is_collision_candidate &&
            key->owner->is_collision_candidate)) {
          // these particle bounds haven't overlapped yet
          collision_candidate_pairs_.push_back(
              std::pair<Particle*, Particle*>(key->owner, to_swap->owner));
          key->owner->is_collision_candidate = true;
          to_swap->owner->is_collision_candidate = true;
        }
      }
      
      if (!key->is_min && to_swap->is_min) { // lower bound moves ahead of upper bound
        key->owner->is_collision_candidate = false;
        to_swap->owner->is_collision_candidate = false;
      }
      
      axis[swapper_idx] = axis[key_idx];
      axis[key_idx] = to_swap;
    }
  }
}

} // namespace synchrony

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

} // namespace synchrony

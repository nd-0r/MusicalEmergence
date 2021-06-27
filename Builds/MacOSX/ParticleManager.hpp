//
//  Physics.hpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#ifndef Physics_hpp
#define Physics_hpp

#include <stdio.h>
#include "Particle.hpp"

namespace synchrony {

struct AxisAlignedBoundingBox;

struct EndPoint {
  EndPoint(Particle* to_owner, float to_value, bool to_is_min) :
    owner(to_owner), value(to_value), is_min(to_is_min) {}
  
  Particle* owner;
  float value;
  bool is_min;
};

struct AxisAlignedBoundingBox {
  std::pair<EndPoint*, EndPoint*> bounds_x;
  std::pair<EndPoint*, EndPoint*> bounds_y;
};

class ParticleManager {
public:
  void AddParticle(const Particle& new_particle);
  void UpdateParticles();
  void FindCollisions();
  void ResolveCollisions();
  void RemoveParticle(const Particle& particle_to_remove);
  
private:
  void SortAxisAndFindCandidates(std::vector<EndPoint*>& axis);
  
  static void InsertSorted(std::vector<EndPoint*>& end_points,
                           const std::pair<EndPoint*, EndPoint*>& bounds,
                           size_t start_idx = 0);
  
  size_t screen_size_x_;
  size_t screen_size_y_;
  
  std::vector<Particle> particles_;
  std::vector<EndPoint*> positions_x_;
  std::vector<EndPoint*> positions_y_;
  std::list<std::pair<Particle*, Particle*>> collision_candidate_pairs_;
};

} // namespace synchrony

#endif /* Physics_hpp */

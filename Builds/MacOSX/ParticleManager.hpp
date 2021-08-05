//
//  Physics.hpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#pragma once

#include <JuceHeader.h>
#include "Particle.hpp"
#include "Utilities.hpp"

namespace synchrony {

class Particle;

class ParticleManager: public juce::AnimatedAppComponent {
public:
  void AddParticle(const Particle& new_particle);
  void update() override;
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

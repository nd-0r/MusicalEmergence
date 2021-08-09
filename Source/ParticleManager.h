//
//  Physics.hpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#pragma once

#define MAX_PARTICLES 600

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Particle.h"
#include "Utilities.h"

namespace synchrony {

class Particle;

class ParticleManager: public juce::AnimatedAppComponent {
public:
  ParticleManager(SynchronyAudioProcessor& ap) : audio_processor_(ap) {
    setFramesPerSecond(60);
  }
  
  void AddParticle(Particle& new_particle);
  void update() override;
  void paint(juce::Graphics& g) override;
  void FindCollisions();
  void ResolveCollisions();
  void RemoveParticle(const Particle& particle_to_remove);
  
private:
  void SortAxisAndFindCandidates(std::vector<EndPoint*>& axis, bool is_x_axis);

  static void InsertSorted(std::vector<EndPoint*>& end_points,
                           const std::pair<EndPoint*, EndPoint*>& bounds,
                           size_t start_idx = 0);

  bool overlap_matrix_[2][MAX_PARTICLES][MAX_PARTICLES];
  int current_particle_id_ = 0;
  std::vector<std::unique_ptr<Particle>> particles_;
  std::vector<EndPoint*> positions_x_;
  std::vector<EndPoint*> positions_y_;
  std::list<std::pair<Particle*, Particle*>> collision_candidate_pairs_;
  
  SynchronyAudioProcessor& audio_processor_;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParticleManager)
};

} // namespace synchrony

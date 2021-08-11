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

#ifdef TEST_PLUGIN
  class TestParticleManager;
#endif

namespace synchrony {

class Particle;

typedef std::array<std::array<bool, MAX_PARTICLES * (MAX_PARTICLES + 1) / 2>, 2> OverlapMatrix;

class ParticleManager: public juce::AnimatedAppComponent {
#ifdef TEST_PLUGIN
  friend TestParticleManager;
#endif
public:
  ParticleManager(SynchronyAudioProcessor& ap) : audio_processor_(ap) {
    setFramesPerSecond(60);
  }

  bool AddParticle(Particle& new_particle);

  void update() override;
  
  void mouseDown(const juce::MouseEvent& event) override;
  
  void mouseUp(const juce::MouseEvent& event) override;

  void TogglePause();

  void Reset();

  void paint(juce::Graphics& g) override;

private:
  void FindCollisions();

  void ResolveCollisions();

  void SortAxisAndFindCandidates(std::vector<EndPoint*>& axis, bool is_x_axis);
  
  void RemoveParticle(std::unique_ptr<Particle>& particle);

  void SetOverlapMatrix(bool is_x_axis, size_t i, size_t j, bool value);

  bool GetOverlapMatrix(bool is_x_axis, size_t i, size_t j) const;

  std::unique_ptr<OverlapMatrix> overlap_matrix_{new OverlapMatrix};
  size_t current_particle_id_ = 0;
  std::list<std::unique_ptr<Particle>> particles_;
  std::vector<EndPoint*> positions_x_;
  std::vector<EndPoint*> positions_y_;
  std::list<std::pair<Particle*, Particle*>> collision_candidate_pairs_;

  bool paused_ = false;
  bool adding_particle_ = false;

  SynchronyAudioProcessor& audio_processor_;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParticleManager)
};

} // namespace synchrony

//
//  Physics.hpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#pragma once

#define MAX_PARTICLES 600
#define MAX_RADIUS 128

#include <random>
#include <chrono>

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
  ParticleManager(SynchronyAudioProcessor& ap) : ap_(ap) {
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
  const std::vector<juce::Colour> kParticleColors = {
    juce::Colour(242, 131, 103),
    juce::Colour(246, 156, 110),
    juce::Colour(251, 186, 119),
    juce::Colour(255, 248, 136),
    juce::Colour(184, 218, 137),
    juce::Colour(148, 204, 138),
    juce::Colour(113, 193, 139),
    juce::Colour(106, 196, 187),
    juce::Colour(94, 197, 244),
    juce::Colour(108, 149, 207),
    juce::Colour(113, 127, 190),
    juce::Colour(116, 107, 175),
    juce::Colour(142, 113, 177),
    juce::Colour(173, 119, 178),
    juce::Colour(240, 132, 180),
    juce::Colour(241, 130, 140),
  };

  bool AddParticlesFromMidiMessages();

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

  std::default_random_engine random_generator_{(unsigned)
    std::chrono::system_clock::now().time_since_epoch().count()};
  SynchronyAudioProcessor& ap_;
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParticleManager)
};

} // namespace synchrony

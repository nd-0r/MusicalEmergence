//
//  Particle.hpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#pragma once

#include <stdio.h>
#include <JuceHeader.h>
#include <vector.hpp>
#include "ParticleManager.hpp"
#include "Utilities.hpp"

namespace synchrony {

class Particle: public juce::Component {
public:
  Particle(const juce::Point<float>& init_pos,
           const juce::Point<float>& init_vel,
           float radius,
           float mass,
           const juce::Colour& color);
  Particle(const Particle& other);
  ~Particle() override;
  void paint(juce::Graphics& g) override;
  void UpdatePosition();
  void SetVelocity(vmml::Vector2f new_velocity);
  
  bool operator==(const Particle& other_particle) const;
  
  bool operator!=(const Particle& other_particle) const;
  
  const vmml::Vector2f& GetInitialPosition() const;
  const vmml::Vector2f& GetCurrentPosition() const;
  const vmml::Vector2f& GetVelocity() const;
  float GetRadius() const;
  float GetMass() const;
  const juce::Colour& GetColor() const;
  const AxisAlignedBoundingBox* GetBoundingBox() const;
  
  static bool DoParticlesCollide(const Particle& particle1,
                                 const Particle& particle2);
  static vmml::Vector2f CalcCollisionVelocity(const Particle& particle1,
                                              const Particle& particle2);
  
  bool is_collision_candidate = false;
  
private:
  vmml::Vector2f CalculateCurrentPosition() const;
  
  static bool AreParticlesApproaching(const Particle& particle1,
                                      const Particle& particle2);
  
  unsigned int time_ = 0;
  vmml::Vector2f initial_position_;
  vmml::Vector2f current_position_;
  vmml::Vector2f velocity_;
  float mass_;
  float radius_;
  AxisAlignedBoundingBox bounding_box_;
  juce::Colour color_;
  
};

} // namespace synchrony

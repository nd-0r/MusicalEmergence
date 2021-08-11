//
//  Particle.hpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#pragma once

#include <JuceHeader.h>
#include <vector.hpp>
#include "ParticleManager.h"
#include "Utilities.h"

namespace synchrony {

class Particle: public juce::Component {
public:
  Particle(const juce::Point<int>& init_pos,
           const juce::Point<int>& init_vel,
           int radius=4,
           const juce::Colour& color=juce::Colour(255, 255, 255));

  Particle(const juce::Point<float>& init_pos,
           const juce::Point<float>& init_vel,
           int radius=4,
           float mass=10,
           const juce::Colour& color=juce::Colour(255, 255, 255));
  Particle(const Particle& other);
  ~Particle() override;
  void CreateBoundingBox();
  
  void paint(juce::Graphics& g) override;
  void mouseDown(const juce::MouseEvent& event) override;
  void UpdatePosition();
  void SetId(size_t to_id);
  void SetVelocity(const juce::Point<float>& new_velocity);
  
  bool operator==(const Particle& other_particle) const;
  
  bool operator!=(const Particle& other_particle) const;
  
  size_t GetId() const;
  const vmml::Vector2f& GetInitialPosition() const;
  const vmml::Vector2f& GetCurrentPosition() const;
  const vmml::Vector2f& GetVelocity() const;
  int GetRadius() const;
  float GetMass() const;
  const juce::Colour& GetColor() const;
  AxisAlignedBoundingBox* GetBoundingBox();
  bool IsRemoved() const;
  
  static bool DoParticlesCollide(const Particle* particle1,
                                 const Particle* particle2);
  static void SetCollisionVelocity(Particle* particle1,
                                   Particle* particle2);
  
private:
  void SetVelocity(const vmml::Vector2f& new_velocity);
  
  static bool AreParticlesApproaching(const Particle* particle1,
                                      const Particle* particle2);
  
  size_t id_;
  unsigned int time_ = 0;
  vmml::Vector2f initial_position_;
  vmml::Vector2f current_position_;
  vmml::Vector2f velocity_;
  float mass_;
  int radius_;
  bool removed_ = false;
  EndPoint low_x_;
  EndPoint high_x_;
  EndPoint low_y_;
  EndPoint high_y_;
  AxisAlignedBoundingBox bounding_box_;
  juce::Colour color_;
};

} // namespace synchrony

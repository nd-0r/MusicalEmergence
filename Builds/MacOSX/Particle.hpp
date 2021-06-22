//
//  Particle.hpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#ifndef Particle_hpp
#define Particle_hpp

#include <stdio.h>
#include <juce_graphics/juce_graphics.h>

namespace synchrony {

class Particle {
public:
  Particle(const juce::Point<float>& init_pos,
           const juce::Point<float>& init_vel,
           float radius,
           const juce::Colour& color);
  void Draw(juce::Graphics& g) const;
  void UpdatePosition();
  void UpdateVelocity(juce::Point<float> new_velocity);
  
  bool operator==(const Particle& other_particle) const;
  
  bool operator!=(const Particle& other_particle) const;
  
  const juce::Point<float>& GetInitialPosition() const;
  const juce::Point<float>& GetCurrentPosition() const;
  const juce::Point<float>& GetVelocity() const;
  float GetRadius() const;
  const juce::Colour& GetColor() const;
    
private:
  juce::Point<float> CalculateCurrentPosition() const;
  
  unsigned int time_;
  juce::Point<float> initial_position_;
  juce::Point<float> current_position_;
  juce::Point<float> velocity_;
  float radius_;
  juce::Colour color_;
  
};

} // namespace synchrony

#endif /* Particle_hpp */

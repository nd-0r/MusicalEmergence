//
//  Particle.cpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#include "Particle.hpp"

namespace synchrony {

Particle::Particle(const juce::Point<float>& init_pos,
                   const juce::Point<float>& init_vel,
                   float radius=1,
                   const juce::Colour& color=juce::Colour(255, 255, 255)) :
                    initial_position_(init_pos),
                    velocity_(init_vel),
                    current_position_(init_pos),
                    radius_(radius),
                    color_(color) {}

void Particle::Draw(juce::Graphics& g) {
  g.fillEllipse(current_position_.getX(),
                current_position_.getY(),
                radius_ * 2, radius_ * 2);
}

void Particle::UpdatePosition() {
  current_position_ = velocity_ * time_;
}

void Particle::UpdateVelocity(juce::Point<float> new_velocity) {
  velocity_ = new_velocity;
}

bool Particle::operator==(const Particle& other_particle) {
  return initial_position_ == other_particle.initial_position_ &&
         radius_ == other_particle.radius_ &&
         color_ == other_particle.color_;
}

bool Particle::operator!=(const Particle& other_particle) {
  return !operator==(other_particle);
}

const juce::Point<float>& Particle::GetInitialPosition() {
  return initial_position_;
}

const juce::Point<float>& Particle::GetCurrentPosition() {
  return current_position_;
}

const juce::Point<float>& Particle::GetVelocity() {
  return velocity_;
}

float Particle::GetRadius() {
  return radius_;
}

const juce::Colour& Particle::GetColor() {
  return color_;
}

} // namespace synchrony

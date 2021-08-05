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
                   float mass=10,
                   const juce::Colour& color=juce::Colour(255, 255, 255)) :
                    radius_(radius),
                    color_(color) {
  initial_position_ = vmml::Vector2f(init_pos.x, init_pos.y);
  current_position_ = vmml::Vector2f(init_pos.x, init_pos.y);
  velocity_ = vmml::Vector2f(init_vel.x, init_vel.y);

  EndPoint low_x(this, current_position_.x() - radius_, true);
  EndPoint high_x(this, current_position_.x() + radius_, false);
  EndPoint low_y(this, current_position_.y() - radius_, true);
  EndPoint high_y(this, current_position_.y() + radius_, false);
  
  bounding_box_.bounds_x = std::pair<EndPoint*, EndPoint*>(&low_x, &high_x);
  bounding_box_.bounds_y = std::pair<EndPoint*, EndPoint*>(&low_y, &high_y);
}

Particle::Particle(const Particle& other) {
  time_ = other.time_;
  initial_position_ = other.initial_position_;
  current_position_ = other.current_position_;
  velocity_ = other.velocity_;
  mass_ = other.mass_;
  radius_ = other.radius_;
  bounding_box_ = other.bounding_box_;
  color_ = other.color_;
}

Particle::~Particle() {
  // No dynamic storage to manage
}

void Particle::paint(juce::Graphics& g) {
  g.fillEllipse(current_position_.x(),
                current_position_.y(),
                radius_ * 2, radius_ * 2);
}

void Particle::UpdatePosition() {
  current_position_ = velocity_ * time_;
  
  bounding_box_.bounds_x.first->value = current_position_.x() - radius_;
  bounding_box_.bounds_x.second->value = current_position_.x() + radius_;
  bounding_box_.bounds_y.first->value = current_position_.y() - radius_;
  bounding_box_.bounds_y.second->value = current_position_.y() + radius_;
  
  ++time_;
}

void Particle::SetVelocity(vmml::Vector2f new_velocity) {
  velocity_ = new_velocity;
  time_ = 0;
}

bool Particle::DoParticlesCollide(const Particle& particle1,
                                  const Particle& particle2) {
  if (AreParticlesApproaching(particle1, particle1)) {
    vmml::Vector2f particle1_pos = particle1.GetCurrentPosition();
    float particle1_rad = particle1.GetRadius();
    vmml::Vector2f particle2_pos = particle2.GetCurrentPosition();
    float particle2_rad = particle2.GetRadius();

    float distance = particle1_pos.distance(particle2_pos);

    // Collide if distance between particles is less than sum of their radii
    return distance <= particle1_rad + particle2_rad;
  }
}

vmml::Vector2f Particle::CalcCollisionVelocity(const Particle& particle1,
                                               const Particle& particle2) {
  float m1 = particle1.GetMass();
  float m2 = particle2.GetMass();
  auto v1 = particle1.GetVelocity();
  auto v2 = particle2.GetVelocity();
  auto x1 = particle1.GetCurrentPosition();
  auto x2 = particle2.GetCurrentPosition();
  
  const float mass_ratio = 2 * m2 / (m1 + m2);
  const float scalar = vmml::dot(v1 - v2, x1 - x2) / ((x1 - x2).length() * (x1 - x2).length());

  return v1 - mass_ratio * scalar * (x1 - x2);
}

bool Particle::operator==(const Particle& other_particle) const {
  return initial_position_ == other_particle.initial_position_ &&
         radius_ == other_particle.radius_ &&
         color_ == other_particle.color_;
}

bool Particle::operator!=(const Particle& other_particle) const {
  return !operator==(other_particle);
}

const vmml::Vector2f& Particle::GetInitialPosition() const {
  return initial_position_;
}

const vmml::Vector2f& Particle::GetCurrentPosition() const {
  return current_position_;
}

const vmml::Vector2f& Particle::GetVelocity() const {
  return velocity_;
}

float Particle::GetRadius() const {
  return radius_;
}

float Particle::GetMass() const {
  return mass_;
}

const juce::Colour& Particle::GetColor() const {
  return color_;
}

const AxisAlignedBoundingBox* Particle::GetBoundingBox() const {
  return &bounding_box_;
}

bool Particle::AreParticlesApproaching(const Particle& particle1,
                                       const Particle& particle2) {
  const vmml::Vector2f v1 = particle1.GetVelocity();
  const vmml::Vector2f v2 = particle2.GetVelocity();
  const vmml::Vector2f x1 = particle1.GetCurrentPosition();
  const vmml::Vector2f x2 = particle2.GetCurrentPosition();

  return vmml::dot(v1 - v2, x1 - x2) < 0;
}

} // namespace synchrony


//
//  Particle.cpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#include "Particle.h"

namespace synchrony {

Particle::Particle(const juce::Point<float>& init_pos,
                   const juce::Point<float>& init_vel,
                   int radius,
                   float mass,
                   const juce::Colour& color) :
                    mass_(mass),
                    radius_(radius),
                    color_(color) {
  initial_position_ = vmml::Vector2f(init_pos.x, init_pos.y);
  current_position_ = vmml::Vector2f(init_pos.x, init_pos.y);
  velocity_ = vmml::Vector2f(init_vel.x, init_vel.y);
  CreateBoundingBox();
}

Particle::Particle(const Particle& other) {
  id_ = other.id_;
  time_ = other.time_;
  initial_position_ = other.initial_position_;
  current_position_ = other.current_position_;
  velocity_ = other.velocity_;
  mass_ = other.mass_;
  radius_ = other.radius_;
  color_ = other.color_;
  CreateBoundingBox();
}

Particle::~Particle() {
  // No dynamic storage to manage
}

void Particle::CreateBoundingBox() {
  low_x_ = EndPoint(this, current_position_.x() - 2 * radius_, true);
  high_x_ = EndPoint(this, current_position_.x() + 2 * radius_, false);
  low_y_ = EndPoint(this, current_position_.y() - 2 * radius_, true);
  high_y_ = EndPoint(this, current_position_.y() + 2 * radius_, false);
                      
  bounding_box_ = AxisAlignedBoundingBox(&low_x_, &high_x_, &low_y_, &high_y_);
}

void Particle::paint(juce::Graphics& g) {
  g.setColour(color_);
  g.fillEllipse(radius_, radius_, radius_ * 2, radius_ * 2);

//  g.drawRect(0, 0, static_cast<int>(bounding_box_.bounds_x.second->GetValue() - bounding_box_.bounds_x.first->GetValue()), static_cast<int>(bounding_box_.bounds_y.second->GetValue() - bounding_box_.bounds_y.first->GetValue()));
}

void Particle::UpdatePosition() {
  ++time_;

  current_position_ = initial_position_ + (velocity_ * time_);
  setBounds((int) current_position_.x() - 2 * radius_,
            (int) current_position_.y() - 2 * radius_,
            radius_ * 4,
            radius_ * 4);
  
  bounding_box_.bounds_x.first->value = current_position_.x() - 2 * radius_;
  bounding_box_.bounds_x.second->value = current_position_.x() + 2 * radius_;
  bounding_box_.bounds_y.first->value = current_position_.y() - 2 * radius_;
  bounding_box_.bounds_y.second->value = current_position_.y() + 2 * radius_;
}

void Particle::SetId(size_t to_id) {
  id_ = to_id;
}

void Particle::SetVelocity(vmml::Vector2f new_velocity) {
  initial_position_ = current_position_;
  velocity_ = new_velocity;
  time_ = 0;
}

bool Particle::DoParticlesCollide(const Particle* particle1,
                                  const Particle* particle2) {
  if (AreParticlesApproaching(particle1, particle2)) {
    vmml::Vector2f particle1_pos = particle1->GetCurrentPosition();
    float particle1_rad = particle1->GetRadius();
    vmml::Vector2f particle2_pos = particle2->GetCurrentPosition();
    float particle2_rad = particle2->GetRadius();

    float distance = particle1_pos.distance(particle2_pos);

    // Collide if distance between particles is less than sum of their radii
    return distance <= particle1_rad + particle2_rad;
  }
  
  return false;
}

vmml::Vector2f Particle::CalcCollisionVelocity(const Particle* particle1,
                                               const Particle* particle2) {
  float m1 = particle1->GetMass();
  float m2 = particle2->GetMass();
  auto v1 = particle1->GetVelocity();
  auto v2 = particle2->GetVelocity();
  auto x1 = particle1->GetCurrentPosition();
  auto x2 = particle2->GetCurrentPosition();
  
  const float mass_ratio = 2 * m2 / (m1 + m2);
  const float scalar = vmml::dot(v1 - v2, x1 - x2) / ((x1 - x2).length() * (x1 - x2).length());

  return v1 - mass_ratio * scalar * (x1 - x2);
}

bool Particle::operator==(const Particle& other_particle) const {
  return initial_position_ == other_particle.initial_position_ &&
         current_position_ == other_particle.current_position_ &&
         radius_ == other_particle.radius_ &&
         color_ == other_particle.color_;
}

bool Particle::operator!=(const Particle& other_particle) const {
  return !operator==(other_particle);
}

size_t Particle::GetId() const {
  return id_;
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

int Particle::GetRadius() const {
  return radius_;
}

float Particle::GetMass() const {
  return mass_;
}

const juce::Colour& Particle::GetColor() const {
  return color_;
}

AxisAlignedBoundingBox* Particle::GetBoundingBox() {
  return &bounding_box_;
}

bool Particle::AreParticlesApproaching(const Particle* particle1,
                                       const Particle* particle2) {
  const vmml::Vector2f v1 = particle1->GetVelocity();
  const vmml::Vector2f v2 = particle2->GetVelocity();
  const vmml::Vector2f x1 = particle1->GetCurrentPosition();
  const vmml::Vector2f x2 = particle2->GetCurrentPosition();

  return vmml::dot(v1 - v2, x1 - x2) < 0;
}

} // namespace synchrony


//
//  Particle.cpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#include "Particle.h"

namespace synchrony {

Particle::Particle(SynchronyAudioProcessor& ap,
                   const juce::Point<int>& init_pos,
                   const juce::Point<int>& init_vel,
                   int radius,
                   const juce::Colour& color,
                   const MidiData& midi_data) :
                    mass_(radius * kMassProportion),
                    radius_(radius),
                    midi_data_(midi_data),
                    color_(color),
                    ap_(ap) {
  initial_position_ = vmml::Vector2f(static_cast<float>(init_pos.x),
                                     static_cast<float>(init_pos.y));
  current_position_ = initial_position_;
  velocity_ = vmml::Vector2f(init_vel.x, init_vel.y);
  CreateBoundingBox();
}

Particle::Particle(SynchronyAudioProcessor& ap,
                   const juce::Point<float>& init_pos,
                   const juce::Point<float>& init_vel,
                   int radius,
                   float mass,
                   const juce::Colour& color,
                   const MidiData& midi_data) :
                    mass_(mass),
                    radius_(radius),
                    midi_data_(midi_data),
                    color_(color),
                    ap_(ap) {
  initial_position_ = vmml::Vector2f(init_pos.x, init_pos.y);
  current_position_ = initial_position_;
  velocity_ = vmml::Vector2f(init_vel.x, init_vel.y);
  CreateBoundingBox();
}

Particle::Particle(const Particle& other) : midi_data_(other.midi_data_),
                                            ap_(other.ap_) {
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

void Particle::paint(juce::Graphics& g) {
  g.setColour(color_);
  g.fillEllipse(radius_,
                radius_,
                radius_ * kBoundingBoxOfRadius,
                radius_ * kBoundingBoxOfRadius);

  if (SynchronySettings::ShouldShowAABBsAndPairs()) {
    g.drawRect(0, 0,
               static_cast<int>(bounding_box_.bounds_x.second->GetValue() -
                                bounding_box_.bounds_x.first->GetValue()),
               static_cast<int>(bounding_box_.bounds_y.second->GetValue() -
                                bounding_box_.bounds_y.first->GetValue()));
  }
}

void Particle::mouseDown(const juce::MouseEvent& event) {
  if (event.eventComponent == this) {
    removed_ = true;
  }
}

void Particle::UpdatePosition() {
  KeepInBounds();
  if (!removed_) {
    ++time_;
    time_ %= SynchronySettings::GetClockStepSize();

    current_position_ += (velocity_ * SynchronySettings::GetVelocityMultiplier());
    setBounds((int) current_position_.x() - 2 * radius_,
              (int) current_position_.y() - 2 * radius_,
              radius_ * 4,
              radius_ * 4);
    
    bounding_box_.bounds_x.first->value = current_position_.x() - 2 * radius_;
    bounding_box_.bounds_x.second->value = current_position_.x() + 2 * radius_;
    bounding_box_.bounds_y.first->value = current_position_.y() - 2 * radius_;
    bounding_box_.bounds_y.second->value = current_position_.y() + 2 * radius_;
  } else {
    // Move the bounds to the end of each axis so they can be easily popped
    bounding_box_.bounds_x.first->value = std::numeric_limits<float>::max();
    bounding_box_.bounds_x.second->value = std::numeric_limits<float>::max();
    bounding_box_.bounds_y.first->value = std::numeric_limits<float>::max();
    bounding_box_.bounds_y.second->value = std::numeric_limits<float>::max();
  }
}

void Particle::SetId(size_t to_id) {
  id_ = to_id;
}

void Particle::SetVelocity(const juce::Point<float>& new_velocity) {
  velocity_ = vmml::Vector2f(new_velocity.getX(), new_velocity.getY());
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

void Particle::SetCollisionVelocity(Particle* particle1,
                                    Particle* particle2) {
  float m1 = particle1->GetMass();
  float m2 = particle2->GetMass();
  auto v1 = particle1->GetVelocity();
  auto v2 = particle2->GetVelocity();
  auto x1 = particle1->GetCurrentPosition();
  auto x2 = particle2->GetCurrentPosition();
  
  const float mass_ratio_1 = kMomentumConstant * m2 / (m1 + m2);
  const float mass_ratio_2 = kMomentumConstant * m1 / (m1 + m2);
  const float scalar_1 = vmml::dot(v1 - v2, x1 - x2) /
                         ((x1 - x2).length() * (x1 - x2).length());
  const float scalar_2 = vmml::dot(v2 - v1, x2 - x1) /
                         ((x2 - x1).length() * (x2 - x1).length());

  auto new_vel_1 = v1 - mass_ratio_1 * scalar_1 * (x1 - x2);
  auto new_vel_2 = v2 - mass_ratio_2 * scalar_2 * (x2 - x1);
  particle1->SetVelocity(new_vel_1);
  particle2->SetVelocity(new_vel_2);
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

void Particle::CreateBoundingBox() {
  low_x_ = EndPoint(this,
                    current_position_.x() - kBoundingBoxOfRadius * radius_,
                    true);
  high_x_ = EndPoint(this,
                     current_position_.x() + kBoundingBoxOfRadius * radius_,
                     false);
  low_y_ = EndPoint(this,
                    current_position_.y() - kBoundingBoxOfRadius * radius_,
                    true);
  high_y_ = EndPoint(this,
                     current_position_.y() + kBoundingBoxOfRadius * radius_,
                     false);
                      
  bounding_box_ = AxisAlignedBoundingBox(&low_x_, &high_x_, &low_y_, &high_y_);
}

void Particle::KeepInBounds() {
//  if (current_position_.y() >= getParentHeight()) {
//    current_position_ = vmml::Vector2f(current_position_.x(),
//                                       getParentHeight() - radius_ - 1);
//    SetVelocity(velocity_);
//  }
//  
//  if (current_position_.x() >= getParentWidth()) {
//    current_position_ = vmml::Vector2f(getParentWidth() - radius_ - 1,
//                                       current_position_.y());
//    SetVelocity(velocity_);
//  }

  if ((velocity_.y() < 0 && current_position_.y() - radius_ <= 0) ||
      (velocity_.y() > 0 && current_position_.y() + radius_ >= getParentHeight())) {
    SetVelocity(velocity_ * vmml::Vector2f(1, -1));
  }

  if ((velocity_.x() < 0 && current_position_.x() - radius_ < 0) ||
      (velocity_.x() > 0 && current_position_.x() + radius_ > getParentWidth())) {
    SetVelocity(velocity_ * vmml::Vector2f(-1, 1));
  }
}

void Particle::SetVelocity(const vmml::Vector2f& new_velocity) {
  initial_position_ = current_position_;
  velocity_ = new_velocity;
  time_ = 0;
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


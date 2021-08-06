//
//  Physics.cpp
//  synchrony - All
//
//  Created by Andrew Orals on 6/20/21.
//

#include "ParticleManager.h"

namespace synchrony {

void ParticleManager::AddParticle(const Particle& new_particle) {
  particles_.push_back(new_particle);
  
  const AxisAlignedBoundingBox* new_particle_box =
    new_particle.GetBoundingBox();
  InsertSorted(positions_x_, new_particle_box->bounds_x);
  InsertSorted(positions_y_, new_particle_box->bounds_y);
}

void ParticleManager::update() {
  for (Particle& particle : particles_) {
    particle.UpdatePosition();
  }
  
  FindCollisions();
  ResolveCollisions();
}

void ParticleManager::paint(juce::Graphics& g) {
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void ParticleManager::FindCollisions() {
  SortAxisAndFindCandidates(positions_x_);
  SortAxisAndFindCandidates(positions_y_);
}

void ParticleManager::ResolveCollisions() {
  std::list<std::pair<Particle*, Particle*>>::iterator iter =
      collision_candidate_pairs_.begin();
  
  // do while candidates is populated and iterator not exhausted
  while (!collision_candidate_pairs_.empty() ||
         iter != collision_candidate_pairs_.end()) {
    Particle* p1 = (*iter).first;
    Particle* p2 = (*iter).second;
    
    // Handle collision between particles
    if (Particle::DoParticlesCollide(*p1, *p2)) {
      vmml::Vector2f vel1_new = Particle::CalcCollisionVelocity(*p1, *p2);
      vmml::Vector2f vel2_new = Particle::CalcCollisionVelocity(*p2, *p1);
      
      p1->SetVelocity(vel1_new);
      p2->SetVelocity(vel2_new);
    }
    
    // Handle collision between particle and wall
    for (Particle* p : {p1, p2}) {
      float x_vel = p->GetVelocity().x();
      float y_vel = p->GetVelocity().y();
      
      if ((y_vel < 0 && p->GetCurrentPosition().y() - p->GetRadius() < 0) ||
          (y_vel > 0 && p->GetCurrentPosition().y() + p->GetRadius() > screen_size_y_)) {
        y_vel *= -1;
        p->SetVelocity(vmml::Vector2f(x_vel, y_vel));
      }
      
      if ((x_vel < 0 && p->GetCurrentPosition().x() - p->GetRadius() < 0) ||
          (x_vel > 0 && p->GetCurrentPosition().x() + p->GetRadius() > screen_size_x_)) {
        x_vel *= -1;
        p->SetVelocity(vmml::Vector2f(x_vel, y_vel));
      }
    }
    
    if (!p1->is_collision_candidate && !p2->is_collision_candidate) {
      // particles no longer in striking range
      collision_candidate_pairs_.erase(iter);
      continue;
    }
    
    ++iter;
  }
}

void ParticleManager::SortAxisAndFindCandidates(std::vector<EndPoint*>& axis) {
  for (size_t key_idx = 0; key_idx < axis.size(); ++key_idx) {
    EndPoint* key = axis.at(key_idx);
    
    size_t swapper_idx = key_idx - 1;
    while (axis[swapper_idx]->value > key->value && swapper_idx > 0) {
      EndPoint* to_swap = axis[swapper_idx];
      
      if (key->is_min && !to_swap->is_min) {
        // lower bound moves ahead another lower bound
        if (!(key->owner->is_collision_candidate &&
            key->owner->is_collision_candidate)) {
          // these particle bounds haven't overlapped yet
          key->owner->is_collision_candidate = true;
          to_swap->owner->is_collision_candidate = true;
        } else {
          // particle bounds overlapped on the other axis
          collision_candidate_pairs_.push_back(
              std::pair<Particle*, Particle*>(key->owner, to_swap->owner));
        }
      }
      
      if (!key->is_min && to_swap->is_min) { // lower bound moves ahead of upper bound
        key->owner->is_collision_candidate = false;
        to_swap->owner->is_collision_candidate = false;
      }
      
      axis[swapper_idx] = axis[key_idx];
      axis[key_idx] = to_swap;
    }
  }
}

void ParticleManager::InsertSorted(std::vector<EndPoint*>& end_points,
                                   const std::pair<EndPoint*, EndPoint*>& bounds,
                                   size_t start_idx) {
  EndPoint* point_to_insert = (start_idx == 0) ? bounds.first : bounds.second;
  
  if (end_points.empty()) {
    end_points.push_back(point_to_insert);
    
    if (start_idx == 0) InsertSorted(end_points, bounds, 1);
  }
  
  std::vector<EndPoint*>::iterator iter;
  for (iter = end_points.begin() + start_idx;
       iter != end_points.end();
       ++iter) {
    if (point_to_insert->value >= (*iter)->value) {
      end_points.insert(iter, point_to_insert);
      
      if (start_idx == 0) InsertSorted(end_points, bounds, iter - end_points.begin());
    }
  }
}

} // namespace synchrony

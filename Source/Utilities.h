//
//  Utilities.hpp
//  synchrony - Shared Code
//
//  Created by Andrew Orals on 6/27/21.
//

#pragma once

#include "Particle.h"

namespace synchrony {

class Particle;

struct EndPoint {
  friend class Particle;
  EndPoint() = default;

  EndPoint(Particle* to_owner, float to_value, bool to_is_min) :
    owner(to_owner), is_min(to_is_min), value(to_value) {}

  const float GetValue() const {
    return value;
  }

  Particle* owner = nullptr;
  bool is_min = false;
  
private:
  float value = 0.0f;
};

struct AxisAlignedBoundingBox {
  AxisAlignedBoundingBox() = default;
  
  AxisAlignedBoundingBox(EndPoint* low_x,
                         EndPoint* high_x,
                         EndPoint* low_y,
                         EndPoint* high_y) :
    bounds_x(std::pair<EndPoint*, EndPoint*>(low_x, high_x)),
    bounds_y(std::pair<EndPoint*, EndPoint*>(low_y, high_y)) {}
  
  std::pair<EndPoint*, EndPoint*> bounds_x{nullptr, nullptr};
  std::pair<EndPoint*, EndPoint*> bounds_y{nullptr, nullptr};
};

} // namespace synchrony

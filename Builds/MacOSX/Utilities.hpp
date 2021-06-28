//
//  Utilities.hpp
//  synchrony - Shared Code
//
//  Created by Andrew Orals on 6/27/21.
//

#pragma once

#include "Particle.hpp"
#include <stdio.h>

namespace synchrony {

class Particle;

struct AxisAlignedBoundingBox;

struct EndPoint {
  EndPoint(Particle* to_owner, float to_value, bool to_is_min) :
    owner(to_owner), value(to_value), is_min(to_is_min) {}
  
  Particle* owner;
  float value;
  bool is_min;
};

struct AxisAlignedBoundingBox {
  std::pair<EndPoint*, EndPoint*> bounds_x;
  std::pair<EndPoint*, EndPoint*> bounds_y;
};

} // namespace synchrony

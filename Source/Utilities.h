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

struct AxisAlignedBoundingBox;

struct EndPoint {
  friend class Particle;
  EndPoint(Particle* to_owner, float to_value, bool to_is_min) :
    owner(to_owner), is_min(to_is_min), value(to_value)  {}
  
  Particle* const owner;
  bool is_min;
  
  const float GetValue() const {
    float tmp = value;
    return tmp;
  }
  
private:
  float value;
};

struct AxisAlignedBoundingBox {
  AxisAlignedBoundingBox() = default;
  
  AxisAlignedBoundingBox(EndPoint* low_x,
                         EndPoint* high_x,
                         EndPoint* low_y,
                         EndPoint* high_y) : bounds_x(std::pair<EndPoint*, EndPoint*>(low_x, high_x)),
                                             bounds_y(std::pair<EndPoint*, EndPoint*>(low_y, high_y)) {
    x_low = std::shared_ptr<EndPoint>(low_x);
    x_high = std::shared_ptr<EndPoint>(high_x);
    y_low = std::shared_ptr<EndPoint>(low_y);
    y_high = std::shared_ptr<EndPoint>(high_y);
  }
  
  std::pair<EndPoint*, EndPoint*> bounds_x{nullptr, nullptr};
  std::pair<EndPoint*, EndPoint*> bounds_y{nullptr, nullptr};
  
private:
  std::shared_ptr<EndPoint> x_low{nullptr};
  std::shared_ptr<EndPoint> x_high{nullptr};
  std::shared_ptr<EndPoint> y_low{nullptr};
  std::shared_ptr<EndPoint> y_high{nullptr};
};

} // namespace synchrony

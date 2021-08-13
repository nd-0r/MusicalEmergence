/*
  ==============================================================================

    SynchronySettings.h
    Created: 13 Aug 2021 12:22:59am
    Author:  Andrew Orals

  ==============================================================================
*/

#pragma once

namespace synchrony {

class SynchronySettings {
public:
  static float GetVelocityMultiplier() { return velocityMultiplier; }
  static bool IsCollisionMode() { return collisionMode; }
  static bool ShouldShowAABBsAndPairs() { return showAABBsAndPairs; }
  static int GetClockSize() { return clockSize; }
  static unsigned int GetClockStepSize() { return clockStepSize; }
protected:
  constexpr static float kMinVelocityMultiplier = 0.1f;
  constexpr static float kMaxVelocityMultiplier = 2.0f;
  constexpr static int kMinClockSize = 2;
  constexpr static int kMaxClockSize = 64;
  constexpr static int kMinClockStepSize = 1;
  constexpr static int kMaxClockStepSize = 32;

  static float velocityMultiplier;
  static bool showAABBsAndPairs;
  static bool collisionMode;
  static int clockSize;
  static unsigned int clockStepSize;
};

} // namespace synchrony

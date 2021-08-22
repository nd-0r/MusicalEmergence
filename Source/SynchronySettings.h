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
  static unsigned int GetClockSize() { return clockSize; }
  static unsigned int GetClockStepSize() { return clockStepSize; }
  static double GetNoteLength() { return noteLength; }
  static double GetMaxNoteLength() { return kMaxNoteLength; }
protected:
  constexpr static float kMinVelocityMultiplier = 0.1f;
  constexpr static float kMaxVelocityMultiplier = 2.0f;
  constexpr static int kMinClockSize = 2;
  constexpr static int kMaxClockSize = 64;
  constexpr static int kMinClockStepSize = 1;
  constexpr static int kMaxClockStepSize = 32;
  constexpr static double kMinNoteLength = 10;
  constexpr static double kMaxNoteLength = 2000;

  static float velocityMultiplier;
  static bool showAABBsAndPairs;
  static bool collisionMode;
  static unsigned int clockSize;
  static unsigned int clockStepSize;
  static double noteLength;
};

} // namespace synchrony

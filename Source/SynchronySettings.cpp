/*
  ==============================================================================

    SynchronySettings.cpp
    Created: 13 Aug 2021 12:45:31am
    Author:  Andrew Orals

  ==============================================================================
*/

#include "SynchronySettings.h"

namespace synchrony {

float SynchronySettings::velocityMultiplier = 1.0f;
bool SynchronySettings::showAABBsAndPairs = false;
bool SynchronySettings::collisionMode = true;
unsigned int SynchronySettings::clockSize = 12;
int SynchronySettings::clockStepSize = 1;
double SynchronySettings::noteLength = 250;

} // namespace synchrony

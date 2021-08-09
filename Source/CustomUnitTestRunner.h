/*
  ==============================================================================

    CustomUnitTestRunner.h
    Created: 9 Aug 2021 1:35:48am
    Author:  Andrew Orals

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class CustomUnitTestRunner : public juce::UnitTestRunner {
    void logMessage (const juce::String& message) override {
        juce::Logger::outputDebugString(message);
    }
};

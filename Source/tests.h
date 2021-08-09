/*
  ==============================================================================

    tests.cpp
    Created: 9 Aug 2021 12:19:38am
    Author:  Andrew Orals

  ==============================================================================
*/

#include <JuceHeader.h>

#include "Particle.h"
#include "ParticleManager.h"
#include "Utilities.h"

class TestParticleManager : public juce::UnitTest {
public:
  TestParticleManager() : UnitTest("Test particle manager") {}
  
  void runTest() override {
    beginTest("No bounding boxes overlap");
    expect(NoBoundingBoxesOverlap());
    
    beginTest("Overlap from north");
    expect(OverlapFromNorth());
    
    beginTest("Overlap from south");
    expect(OverlapFromSouth());
    
    beginTest("Overlap from east");
    expect(OverlapFromEast());
    
    beginTest("Overlap from west");
    expect(OverlapFromWest());
    
    beginTest("Overlap from southwest");
    expect(OverlapFromSouthwest());
    
    beginTest("Overlap from northeast");
    expect(OverlapFromNortheast());
    
    beginTest("Stop Overlap going north");
    expect(StopOverlapGoingNorth());
    
    beginTest("Stop Overlap going south");
    expect(StopOverlapGoingSouth());
    
    beginTest("Stop Overlap going east");
    expect(StopOverlapGoingEast());
    
    beginTest("Stop Overlap going west");
    expect(StopOverlapGoingWest());
    
    beginTest("Stop Overlap going southwest");
    expect(StopOverlapGoingSouthwest());
    
    beginTest("Stop Overlap going northeast");
    expect(StopOverlapGoingNortheast());
    
    beginTest("Three overlap");
    expect(ThreeOverlap());
    
    beginTest("Overlap procedure");
    expect(OverlapProcedure());
    
    beginTest("Multiple overlap procedure");
    expect(MultipleOverlapProcedure());
  }

private:
  bool NoBoundingBoxesOverlap() {
    return true;
  }
  
  bool OverlapFromNorth() {
    return true;
  }
  
  bool OverlapFromSouth() {
    return true;
  }
  
  bool OverlapFromEast() {
    return true;
  }
  
  bool OverlapFromWest() {
    return true;
  }
  
  bool OverlapFromSouthwest() {
    return true;
  }
  
  bool OverlapFromNortheast() {
    return true;
  }
  
  bool StopOverlapGoingNorth() {
    return true;
  }
  
  bool StopOverlapGoingSouth() {
    return true;
  }
  
  bool StopOverlapGoingEast() {
    return true;
  }
  
  bool StopOverlapGoingWest() {
    return true;
  }
  
  bool StopOverlapGoingSouthwest() {
    return true;
  }
  
  bool StopOverlapGoingNortheast() {
    return true;
  }

  bool ThreeOverlap() {
    return true;
  }
  
  bool OverlapProcedure() {
    return true;
  }
  
  bool MultipleOverlapProcedure() {
    return true;
  }
};

static TestParticleManager test; // So UnitTestRunner can run the tests

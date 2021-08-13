/*
  ==============================================================================

    tests.cpp
    Created: 9 Aug 2021 12:19:38am
    Author:  Andrew Orals

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "Particle.h"
#include "ParticleManager.h"
#include "Utilities.h"

using namespace synchrony;

class TestParticleManager : public juce::UnitTest {
public:
  TestParticleManager() : UnitTest("Test particle manager") {}
  
  void initialise() override {
    pm_to_test_.setTopLeftPosition(0, 0);
    pm_to_test_.setSize(100, 100);
  }
  
  void runTest() override {
    beginTest("No bounding boxes overlap");
    expect(NoBoundingBoxesOverlap());
    pm_to_test_.Reset();
    
    beginTest("Overlap from north");
    expect(OverlapFromNorth());
    pm_to_test_.Reset();
    
    beginTest("Overlap from south");
    expect(OverlapFromSouth());
    pm_to_test_.Reset();
    
    beginTest("Overlap from east");
    expect(OverlapFromEast());
    pm_to_test_.Reset();
    
    beginTest("Overlap from west");
    expect(OverlapFromWest());
    pm_to_test_.Reset();
    
    beginTest("Overlap from southwest");
    expect(OverlapFromSouthwest());
    pm_to_test_.Reset();
    
    beginTest("Overlap from northeast");
    expect(OverlapFromNortheast());
    pm_to_test_.Reset();
    
    beginTest("Stop Overlap going north");
    expect(StopOverlapGoingNorth());
    pm_to_test_.Reset();
    
    beginTest("Stop Overlap going south");
    expect(StopOverlapGoingSouth());
    pm_to_test_.Reset();
    
    beginTest("Stop Overlap going east");
    expect(StopOverlapGoingEast());
    pm_to_test_.Reset();
    
    beginTest("Stop Overlap going west");
    expect(StopOverlapGoingWest());
    pm_to_test_.Reset();
    
    beginTest("Stop Overlap going southwest");
    expect(StopOverlapGoingSouthwest());
    pm_to_test_.Reset();
    
    beginTest("Stop Overlap going northeast");
    expect(StopOverlapGoingNortheast());
    pm_to_test_.Reset();
    
    beginTest("Three overlap");
    expect(ThreeOverlap());
    pm_to_test_.Reset();
    
    beginTest("Overlap procedure");
    expect(OverlapProcedure());
    pm_to_test_.Reset();
    
    beginTest("Multiple overlap procedure");
    expect(MultipleOverlapProcedure());
    pm_to_test_.Reset();
  }

private:
  // Radius 4, AABB's are 4 radii wide, particle at center of AABB
  
  bool NoBoundingBoxesOverlap() {
    auto p1_pos = juce::Point<float>(16, 16);
    auto p1_vel = juce::Point<float>(0, 0);
    auto p2_pos = juce::Point<float>(64, 64);
    auto p2_vel = juce::Point<float>(0, 0);
    const std::vector<std::pair<Particle, Particle>> kExpected = {};
    
    Particle p1 = Particle(sap_, p1_pos, p1_vel);
    Particle p2 = Particle(sap_, p2_pos, p2_vel);
    
    pm_to_test_.AddParticle(p1);
    pm_to_test_.AddParticle(p2);
    
    pm_to_test_.update();
    
    return CheckCollisionPairs(&kExpected);
  }
  
  bool OverlapFromNorth() {
    auto p1_pos = juce::Point<float>(16, 15);
    auto p1_vel = juce::Point<float>(0, 1);
    auto p2_pos = juce::Point<float>(16, 32);
    auto p2_vel = juce::Point<float>(0, 0);

    Particle p1 = Particle(sap_, p1_pos, p1_vel);
    Particle p2 = Particle(sap_, p2_pos, p2_vel);
    
    pm_to_test_.AddParticle(p1);
    pm_to_test_.AddParticle(p2);
    
    p1.UpdatePosition();
    const std::vector<std::pair<Particle, Particle>> kExpected = {
      std::pair<Particle, Particle>(p2, p1)
    };

    pm_to_test_.update();

    return CheckCollisionPairs(&kExpected);
  }
  
  bool OverlapFromSouth() {
    auto p1_pos = juce::Point<float>(16, 49);
    auto p1_vel = juce::Point<float>(0, -2);
    auto p2_pos = juce::Point<float>(16, 32);
    auto p2_vel = juce::Point<float>(0, 0);

    Particle p1 = Particle(sap_, p1_pos, p1_vel);
    Particle p2 = Particle(sap_, p2_pos, p2_vel);
    
    pm_to_test_.AddParticle(p1);
    pm_to_test_.AddParticle(p2);
    
    p1.UpdatePosition();
    const std::vector<std::pair<Particle, Particle>> kExpected = {
      std::pair<Particle, Particle>(p2, p1)
    };

    pm_to_test_.update();

    return CheckCollisionPairs(&kExpected);
  }
  
  bool OverlapFromEast() {
    auto p1_pos = juce::Point<float>(49, 16);
    auto p1_vel = juce::Point<float>(-2, 0);
    auto p2_pos = juce::Point<float>(32, 16);
    auto p2_vel = juce::Point<float>(0, 0);

    Particle p1 = Particle(sap_, p1_pos, p1_vel);
    Particle p2 = Particle(sap_, p2_pos, p2_vel);
    
    pm_to_test_.AddParticle(p1);
    pm_to_test_.AddParticle(p2);
    
    p1.UpdatePosition();
    const std::vector<std::pair<Particle, Particle>> kExpected = {
      std::pair<Particle, Particle>(p2, p1)
    };

    pm_to_test_.update();

    return CheckCollisionPairs(&kExpected);
  }
  
  bool OverlapFromWest() {
    auto p1_pos = juce::Point<float>(15, 16);
    auto p1_vel = juce::Point<float>(1, 0);
    auto p2_pos = juce::Point<float>(32, 16);
    auto p2_vel = juce::Point<float>(0, 0);

    Particle p1 = Particle(sap_, p1_pos, p1_vel);
    Particle p2 = Particle(sap_, p2_pos, p2_vel);
    
    pm_to_test_.AddParticle(p1);
    pm_to_test_.AddParticle(p2);
    
    p1.UpdatePosition();
    const std::vector<std::pair<Particle, Particle>> kExpected = {
      std::pair<Particle, Particle>(p2, p1)
    };

    pm_to_test_.update();

    return CheckCollisionPairs(&kExpected);
  }
  
  bool OverlapFromSouthwest() {
    auto p1_pos = juce::Point<float>(15, 49);
    auto p1_vel = juce::Point<float>(1, -2);
    auto p2_pos = juce::Point<float>(32, 32);
    auto p2_vel = juce::Point<float>(0, 0);

    Particle p1 = Particle(sap_, p1_pos, p1_vel);
    Particle p2 = Particle(sap_, p2_pos, p2_vel);
    
    pm_to_test_.AddParticle(p1);
    pm_to_test_.AddParticle(p2);
    
    p1.UpdatePosition();
    const std::vector<std::pair<Particle, Particle>> kExpected = {
      std::pair<Particle, Particle>(p2, p1)
    };

    pm_to_test_.update();

    return CheckCollisionPairs(&kExpected);
  }
  
  bool OverlapFromNortheast() {
    auto p1_pos = juce::Point<float>(49, 15);
    auto p1_vel = juce::Point<float>(-2, 2);
    auto p2_pos = juce::Point<float>(32, 32);
    auto p2_vel = juce::Point<float>(0, 0);

    Particle p1 = Particle(sap_, p1_pos, p1_vel);
    Particle p2 = Particle(sap_, p2_pos, p2_vel);
    
    pm_to_test_.AddParticle(p1);
    pm_to_test_.AddParticle(p2);
    
    p1.UpdatePosition();
    const std::vector<std::pair<Particle, Particle>> kExpected = {
      std::pair<Particle, Particle>(p2, p1)
    };

    pm_to_test_.update();

    return CheckCollisionPairs(&kExpected);
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
  
  // Helper
  
  bool CheckCollisionPairs(const std::vector<std::pair<Particle, Particle>>* expected) {
    std::list<std::pair<Particle*, Particle*>> pm_collision_pairs =
      pm_to_test_.collision_candidate_pairs_;
    
    for (auto& expected_pair: *expected) {
      bool found = false;
      for (auto iter = pm_collision_pairs.begin();
           !found && iter != pm_collision_pairs.end();
           ++iter) {
        
        if (*(iter->first) == expected_pair.first &&
            *(iter->second) == expected_pair.second) {
          found = true;
        }
        
      }
      if (!found) return false;
    }

    return true;
  }

  SynchronyAudioProcessor sap_;
  ParticleManager pm_to_test_{sap_};
};

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#define MAX_WIDTH 3200
#define MAX_HEIGHT 2400
#define MIN_WIDTH 600
#define MIN_HEIGHT 450

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "SynchronySettings.h"
#include "ParticleManager.h"
#include "Particle.h"

#ifdef TEST_PLUGIN
  #include "CustomUnitTestRunner.h"
  #include "TestParticleManager.h"
#endif

//==============================================================================
/**
*/
class SynchronyAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                              juce::TextButton::Listener,
                                              juce::Slider::Listener,
                                       private synchrony::SynchronySettings {
public:
  SynchronyAudioProcessorEditor (SynchronyAudioProcessor&);
  ~SynchronyAudioProcessorEditor() override;

  void paint (juce::Graphics&) override;
  void resized() override;
                                                
  void buttonClicked(juce::Button* button) override;
  void sliderValueChanged(juce::Slider* slider) override;

private:
  const float kMargin = 0.02f;
  const float kParticleBoxWidth = 0.7f;
  const float kParticleBoxHeight = 0.96f;
  const float kSettingsBoxWidth = 0.24f;
  const int kSettingsSpacer = 5;
  const int kAddParticleBtnHeight = 30;
  const int kTextBoxHeight = 20;
  const int kSliderHeight = 30;

  SynchronyAudioProcessor& audioProcessor;

  juce::TextButton clear_{"Clear"};
  juce::TextButton pause_{"Pause"};
  juce::TextButton show_aabbs_{"Show AABBs"};

  juce::Label particle_speed_label_{"Particle Speed", "Particle Speed"};
  juce::Slider particle_speed_{juce::Slider::SliderStyle::LinearHorizontal,
                               juce::Slider::TextEntryBoxPosition::TextBoxBelow};
  juce::Label clock_size_label_{"Clock Speed", "Clock Speed"};
  juce::Slider clock_size_{juce::Slider::SliderStyle::LinearHorizontal,
                            juce::Slider::TextEntryBoxPosition::TextBoxBelow};
//  juce::ToggleButton use_tempo_{"Use Tempo"};
  juce::Label nudge_amount_label_{"Nudge Amount", "Nudge Amount"};
  juce::Slider nudge_amount_{juce::Slider::SliderStyle::LinearHorizontal,
                             juce::Slider::TextEntryBoxPosition::TextBoxBelow};
  juce::Label note_length_label_{"Note Length", "Note Length"};
  juce::Slider note_length_{juce::Slider::SliderStyle::LinearHorizontal,
                            juce::Slider::TextEntryBoxPosition::NoTextBox};
  
  synchrony::ParticleManager pm_{audioProcessor};
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynchronyAudioProcessorEditor)
};

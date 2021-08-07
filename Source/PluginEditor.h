/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ParticleManager.h"
#include "Particle.h"

//==============================================================================
/**
*/
class SynchronyAudioProcessorEditor  : public juce::AudioProcessorEditor,
                                              juce::TextButton::Listener,
                                              juce::Label::Listener,
                                              juce::Slider::Listener {
public:
  SynchronyAudioProcessorEditor (SynchronyAudioProcessor&);
  ~SynchronyAudioProcessorEditor() override;

  void paint (juce::Graphics&) override;
  void resized() override;
                                                
  void buttonClicked(juce::Button* button) override;
  void labelTextChanged (juce::Label* labelThatHasChanged) override;
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

  juce::TextButton add_particle_{"Add Particle"};
  juce::Label particle_speed_label_{"Particle Speed", "Particle Speed"};
  juce::Slider particle_speed_{juce::Slider::SliderStyle::LinearHorizontal,
                               juce::Slider::TextEntryBoxPosition::TextBoxBelow};
  juce::Label clock_speed_label_{"Clock Speed", "Clock Speed"};
  juce::Slider clock_speed_{juce::Slider::SliderStyle::LinearHorizontal,
                            juce::Slider::TextEntryBoxPosition::TextBoxBelow};
//  juce::ToggleButton use_tempo_{"Use Tempo"};
  juce::Label nudge_amount_label_{"Nudge Amount", "Nudge Amount"};
  juce::Slider nudge_amount_{juce::Slider::SliderStyle::LinearHorizontal,
                             juce::Slider::TextEntryBoxPosition::TextBoxBelow};
  
  synchrony::ParticleManager pm_{audioProcessor};
  
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynchronyAudioProcessorEditor)
};

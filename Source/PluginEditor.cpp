/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynchronyAudioProcessorEditor::SynchronyAudioProcessorEditor (SynchronyAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
  setSize (400, 300);
  setResizable(true, true);
  setResizeLimits(400, 300, 3200, 2400);
  
  addAndMakeVisible(add_particle_);
  add_particle_.addListener(this);
  
  addAndMakeVisible(particle_speed_label_);
  particle_speed_label_.setColour(juce::Label::ColourIds::textColourId,
                                  getLookAndFeel().findColour(juce::Label::ColourIds::textColourId));
  particle_speed_label_.addListener(this);
  
  addAndMakeVisible(particle_speed_);
  particle_speed_.setTextBoxIsEditable(false);
  particle_speed_.addListener(this);
  
  addAndMakeVisible(clock_speed_label_);
  clock_speed_label_.setColour(juce::Label::ColourIds::textColourId,
                               getLookAndFeel().findColour(juce::Label::ColourIds::textColourId));
  clock_speed_label_.addListener(this);
  
  addAndMakeVisible(clock_speed_);
  clock_speed_.setTextBoxIsEditable(false);
  clock_speed_.addListener(this);
  
//  addAndMakeVisible(use_tempo_);
//  use_tempo_.addListener(this);
  
  addAndMakeVisible(nudge_amount_label_);
  nudge_amount_label_.setColour(juce::Label::ColourIds::textColourId,
                                getLookAndFeel().findColour(juce::Label::ColourIds::textColourId));
  nudge_amount_label_.addListener(this);
  
  addAndMakeVisible(nudge_amount_);
  nudge_amount_.setTextBoxIsEditable(false);
  nudge_amount_.addListener(this);
  
  addAndMakeVisible(pm_);
}

SynchronyAudioProcessorEditor::~SynchronyAudioProcessorEditor() {}

void SynchronyAudioProcessorEditor::paint (juce::Graphics& g) {
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void SynchronyAudioProcessorEditor::resized()
{
  float width = static_cast<float>(getWidth());
  float height = static_cast<float>(getHeight());
  
  pm_.setTopLeftPosition(static_cast<int>(width * (2 * kMargin + kSettingsBoxWidth)),
                         static_cast<int>(height * kMargin));
  pm_.setSize(static_cast<int>(width * kParticleBoxWidth),
              static_cast<int>(height * kParticleBoxHeight));
  
  add_particle_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                   static_cast<int>(height * kMargin));
  add_particle_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                        kAddParticleBtnHeight);
  
  clock_speed_label_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                        static_cast<int>(0.3 * height));
  clock_speed_label_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                             kTextBoxHeight);
  
  clock_speed_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                  clock_speed_label_.getY() + kTextBoxHeight + 5);
  clock_speed_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                       kSliderHeight);
  
  nudge_amount_label_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                         clock_speed_.getY() + kSliderHeight + kSettingsSpacer);
  nudge_amount_label_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                              kTextBoxHeight);
  
  nudge_amount_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                   nudge_amount_label_.getY() + kTextBoxHeight + 5);
  nudge_amount_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                        kSliderHeight);
  
  particle_speed_label_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                           nudge_amount_.getY() + kSliderHeight + kSettingsSpacer);
  particle_speed_label_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                                kTextBoxHeight);
  
  particle_speed_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                     particle_speed_label_.getY() + kTextBoxHeight + 5);
  particle_speed_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                          kSliderHeight);
}

void SynchronyAudioProcessorEditor::buttonClicked(juce::Button* button) {
  // TODO - implement
  if (button == &add_particle_) {
    synchrony::Particle to_add = synchrony::Particle(juce::Point<float>(32, 56),
                                                     juce::Point<float>(1, 1),
                                                     10,
                                                     10,
                                                     juce::Colours::white);
    pm_.AddParticle(to_add);
  }
}

void SynchronyAudioProcessorEditor::labelTextChanged (juce::Label* labelThatHasChanged) {
  // TODO - implement
}

void SynchronyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
  // TODO - implement
}

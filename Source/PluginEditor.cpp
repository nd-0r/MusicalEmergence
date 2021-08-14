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
#ifdef TEST_PLUGIN
  static TestParticleManager test; // So UnitTestRunner can run the tests
  CustomUnitTestRunner runner;
  runner.runAllTests();
#endif
  setSize (MIN_WIDTH, MIN_HEIGHT);
  setResizable(true, true);
  setResizeLimits(MIN_WIDTH, MIN_HEIGHT, MAX_WIDTH, MAX_HEIGHT);
  
  addAndMakeVisible(clear_);
  clear_.addListener(this);
  
  addAndMakeVisible(pause_);
  pause_.addListener(this);
  
  addAndMakeVisible(show_aabbs_);
  show_aabbs_.addListener(this);
  
  addAndMakeVisible(toggle_emergence_);
  toggle_emergence_.addListener(this);
  
  addAndMakeVisible(particle_speed_label_);
  particle_speed_label_.setColour(juce::Label::ColourIds::textColourId,
                                  getLookAndFeel().findColour(juce::Label::ColourIds::textColourId));
  
  addAndMakeVisible(particle_speed_);
  particle_speed_.setTextBoxIsEditable(false);
  particle_speed_.addListener(this);
  particle_speed_.setRange(SynchronySettings::kMinVelocityMultiplier,
                           SynchronySettings::kMaxVelocityMultiplier);
  particle_speed_.setDoubleClickReturnValue(true, SynchronySettings::velocityMultiplier);
  particle_speed_.setValue(SynchronySettings::velocityMultiplier);
  
  addAndMakeVisible(clock_size_label_);
  clock_size_label_.setColour(juce::Label::ColourIds::textColourId,
                               getLookAndFeel().findColour(juce::Label::ColourIds::textColourId));
  
  addAndMakeVisible(clock_size_);
  clock_size_.setTextBoxIsEditable(false);
  clock_size_.addListener(this);
  clock_size_.setRange(SynchronySettings::kMinClockSize,
                       SynchronySettings::kMaxClockSize,
                       1);
  clock_size_.setDoubleClickReturnValue(true, SynchronySettings::clockSize);
  clock_size_.setValue(SynchronySettings::clockSize);
  
//  addAndMakeVisible(use_tempo_);
//  use_tempo_.addListener(this);
  
  addAndMakeVisible(nudge_amount_label_);
  nudge_amount_label_.setColour(juce::Label::ColourIds::textColourId,
                                getLookAndFeel().findColour(juce::Label::ColourIds::textColourId));
  
  addAndMakeVisible(nudge_amount_);
  nudge_amount_.setTextBoxIsEditable(false);
  nudge_amount_.addListener(this);
  nudge_amount_.setRange(SynchronySettings::kMinClockStepSize,
                         SynchronySettings::kMaxClockStepSize,
                         1);
  nudge_amount_.setDoubleClickReturnValue(true, SynchronySettings::clockStepSize);
  nudge_amount_.setValue(SynchronySettings::clockStepSize);
  
  addAndMakeVisible(note_length_label_);
  note_length_label_.setColour(juce::Label::ColourIds::textColourId,
                               getLookAndFeel().findColour(juce::Label::ColourIds::textColourId));
  
  addAndMakeVisible(note_length_);
  note_length_.addListener(this);
  note_length_.setRange(SynchronySettings::kMinNoteLength,
                        SynchronySettings::kMaxNoteLength,
                        10);
  note_length_.setDoubleClickReturnValue(true, SynchronySettings::noteLength);
  note_length_.setValue(SynchronySettings::noteLength);
  
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
  
  clear_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                   static_cast<int>(height * kMargin));
  clear_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                        kAddParticleBtnHeight);
  
  pause_.setTopLeftPosition(static_cast<int>(width * kMargin),
                            static_cast<int>(height * kMargin +
                                             kAddParticleBtnHeight +
                                             kSettingsSpacer));
  pause_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                 kAddParticleBtnHeight);
  
  show_aabbs_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                 pause_.getY() + kSettingsSpacer +
                                 kAddParticleBtnHeight);
  show_aabbs_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                      kAddParticleBtnHeight);

  toggle_emergence_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                       show_aabbs_.getY() + kSettingsSpacer +
                                       kAddParticleBtnHeight);
  toggle_emergence_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                            kAddParticleBtnHeight);

  clock_size_label_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                        static_cast<int>(0.3 * height));
  clock_size_label_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                             kTextBoxHeight);
  
  clock_size_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                  clock_size_label_.getY() + kTextBoxHeight + 5);
  clock_size_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                       kSliderHeight);
  
  nudge_amount_label_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                         clock_size_.getY() + kSliderHeight + kSettingsSpacer);
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
  
  note_length_label_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                        particle_speed_.getY() + kSliderHeight + kSettingsSpacer);
  note_length_label_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                             kTextBoxHeight);
  
  note_length_.setTopLeftPosition(static_cast<int>(width * kMargin),
                                  note_length_label_.getY() + kTextBoxHeight + 5);
  note_length_.setSize(static_cast<int>(width * kSettingsBoxWidth),
                       kSliderHeight);
}

void SynchronyAudioProcessorEditor::buttonClicked(juce::Button* button) {
  if (button == &clear_) {
    pm_.Reset();
  } else if (button == &pause_) {
    pm_.TogglePause();
  } else if (button == &show_aabbs_) {
    SynchronySettings::showAABBsAndPairs = !SynchronySettings::showAABBsAndPairs;
  } else if (button == &toggle_emergence_) {
    SynchronySettings::collisionMode = !SynchronySettings::collisionMode;
  }
}

void SynchronyAudioProcessorEditor::sliderValueChanged(juce::Slider* slider) {
  if (slider == &particle_speed_) {
    SynchronySettings::velocityMultiplier = static_cast<float>(slider->getValue());
  } else if (slider == &clock_size_) {
    SynchronySettings::clockSize = static_cast<unsigned int>(slider->getValue());
  } else if (slider == &nudge_amount_) {
    SynchronySettings::clockStepSize = static_cast<unsigned int>(slider->getValue());
  } else if (slider == &note_length_) {
    SynchronySettings::noteLength = slider->getValue();
  }
}

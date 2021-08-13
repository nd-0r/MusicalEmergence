/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

struct MidiData {
  MidiData(int note_num, int vel) :
    note_num_(note_num),
    velocity_(vel) {}

  int note_num_;
  int velocity_;
  double off_timestamp_ = 0;
  bool sent_note_on_ = false;
};

//==============================================================================
/**
*/
class SynchronyAudioProcessor  : public juce::AudioProcessor
{
public:
  SynchronyAudioProcessor();
  ~SynchronyAudioProcessor() override;

  //==============================================================================
  void prepareToPlay (double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
  bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

  void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

  //==============================================================================
  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  //==============================================================================
  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  //==============================================================================
  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram (int index) override;
  const juce::String getProgramName (int index) override;
  void changeProgramName (int index, const juce::String& newName) override;

  //==============================================================================
  void getStateInformation (juce::MemoryBlock& destData) override;
  void setStateInformation (const void* data, int sizeInBytes) override;
  
  //==============================================================================
  void OutputMidiMessage(const MidiData& data);
  double GetStartTime() const { return startTime; }
  
  std::list<MidiData> midi_in_message_queue_;
private:
  std::list<MidiData> midi_out_message_queue_;
  double startTime{juce::Time::getMillisecondCounterHiRes() * 0.001};
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynchronyAudioProcessor)
};

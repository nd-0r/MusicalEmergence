/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SynchronyAudioProcessor::SynchronyAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

SynchronyAudioProcessor::~SynchronyAudioProcessor()
{
}

//==============================================================================
const juce::String SynchronyAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SynchronyAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SynchronyAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SynchronyAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SynchronyAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SynchronyAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SynchronyAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SynchronyAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SynchronyAudioProcessor::getProgramName (int index)
{
    return {};
}

void SynchronyAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SynchronyAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void SynchronyAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SynchronyAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SynchronyAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels  = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
      buffer.clear (i, 0, buffer.getNumSamples());
  
  for (const auto midi_metadata : midiMessages) {
    const auto message = midi_metadata.getMessage();
    if (message.isNoteOn()) {
      midi_in_message_queue_.push_back(
        MidiData(message.getNoteNumber(),
                 message.getVelocity())
      );
    }
  }

  for (MidiData& midi_data : midi_out_message_queue_) {
    if (!midi_data.sent_note_on_) {
      auto on_message = juce::MidiMessage::noteOn(1, midi_data.note_num_,
                                                  juce::uint8(midi_data.velocity_));
      on_message.setTimeStamp(juce::Time::getMillisecondCounterHiRes() *
                              0.001 - startTime);
      midiMessages.addEvent(on_message, 0);

      midi_data.off_timestamp_ = juce::Time::getMillisecondCounterHiRes() -
                                 startTime + SynchronySettings::GetNoteLength();
      midi_data.sent_note_on_ = true;
    }
  }

  auto iter = midi_out_message_queue_.begin();
  while (iter != midi_out_message_queue_.end()) {
    MidiData& midi_data = *iter;

    if (midi_data.off_timestamp_ < juce::Time::getMillisecondCounterHiRes() - startTime) {
      auto off_message = juce::MidiMessage::noteOff(1, midi_data.note_num_,
                                                    juce::uint8(midi_data.velocity_));
      off_message.setTimeStamp(juce::Time::getMillisecondCounterHiRes() *
                               0.001 - startTime);
      midiMessages.addEvent(off_message, 0);
      
      midi_out_message_queue_.erase(iter++);
      continue;
    }

    ++iter;
  }
}

//==============================================================================
bool SynchronyAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SynchronyAudioProcessor::createEditor()
{
    return new SynchronyAudioProcessorEditor (*this);
}

//==============================================================================
void SynchronyAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SynchronyAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void SynchronyAudioProcessor::OutputMidiMessage(const MidiData& data) {
  midi_out_message_queue_.push_back(data);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SynchronyAudioProcessor();
}

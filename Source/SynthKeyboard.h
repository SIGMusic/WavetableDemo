/*
  ==============================================================================

    SynthKeyboard.h
    Created: 18 Sep 2022 2:53:48am
    Author:  Andrew Orals

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WavetableSynth.h"

//==============================================================================
/*
*/
class SynthKeyboard  : public juce::Component,
                       public juce::MidiKeyboardState::Listener,
                       public juce::AudioSource
{
public:
    SynthKeyboard()
    {
        midi_keyboard_state_.addListener(this);
        midi_keyboard_.reset(new juce::MidiKeyboardComponent(midi_keyboard_state_,
                            juce::KeyboardComponentBase::Orientation::horizontalKeyboard));
        addAndMakeVisible(midi_keyboard_.get());
    }

    virtual ~SynthKeyboard() = default;

    //==========================================================================
    // AudioSource
 
    virtual void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
    wavetable_synth_.prepareToPlay(samplesPerBlockExpected, sampleRate);
    }
 
    virtual void releaseResources() override { /* Nothing */ }

    virtual void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        wavetable_synth_.getNextAudioBlock(bufferToFill);
    }

    //==========================================================================
    // MidiKeyboardState::Listener

    static constexpr inline float midiToFreq(juce::uint8 midi_note)
    {
        return 440.0 * std::powf(2.0, (midi_note - 69) / 12.0);
    }

    virtual void handleNoteOn(juce::MidiKeyboardState *source,
                              int midiChannel,
                              int midiNoteNumber,
                              float velocity) override
    {
        wavetable_synth_.setFrequency(midiToFreq((juce::uint8) midiNoteNumber));
        wavetable_synth_.setAmplitude(0.5);
    }

    virtual void handleNoteOff(juce::MidiKeyboardState *source,
                               int midiChannel,
                               int midiNoteNumber,
                               float velocity) override
    {
        wavetable_synth_.setAmplitude(0.0);
    }

    //==========================================================================
    // Component

    void paint (juce::Graphics& g) override { /* Nothing */ }

    void resized() override
    {
        midi_keyboard_->setBounds(getLocalBounds());
    }

private:
    WavetableSynth wavetable_synth_;
    juce::MidiKeyboardState midi_keyboard_state_;
    std::unique_ptr<juce::MidiKeyboardComponent> midi_keyboard_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthKeyboard)
};

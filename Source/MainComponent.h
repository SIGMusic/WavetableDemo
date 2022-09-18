#pragma once

#include <JuceHeader.h>

#include "SynthKeyboard.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
private:
    //==============================================================================
    static const size_t kNumSynths = 5;
    static const int kKeyboardHeight = 100; // pixels
    juce::OwnedArray<SynthKeyboard> synths_;
    juce::MixerAudioSource mixer_;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

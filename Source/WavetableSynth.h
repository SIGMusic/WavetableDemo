/*
  ==============================================================================

    WavetableSynth.h
    Created: 13 Sep 2022 11:40:18pm
    Author:  Andrew Orals

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WavetableSynth  : public juce::ToneGeneratorAudioSource
{
public:
    WavetableSynth()
    {
        buildWavetable();
    }

    ~WavetableSynth() override { /* Nothing */ }
    
    void setAmplitude(float amp)
    {
        amplitude_ = amp;
    }

    void setFrequency(float frequency)
    {
        frequency_ = frequency;
        prepareToPlay(0, sample_rate_);
    }

    /**
    Calculates the number of "steps" to take through the table per sample.
    TODO
    */
    virtual void prepareToPlay(
        int /* parameter not needed */, double sampleRate) override
    {
        sample_rate_ = sampleRate;
        // Ratio of samples per table run to samples per second
        // TODO
        // Calculate number of steps to take through the table per sample
        // TODO
    }

    /**
    This fills the audio buffer with the samples that the synth generates.
    */
    virtual void getNextAudioBlock(
        const juce::AudioSourceChannelInfo &bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
        auto* buf0 = bufferToFill.buffer->getWritePointer(0);

        // >>> Here's where the buffer is filled with samples <<<
        for (unsigned int idx = 0; idx < bufferToFill.numSamples; ++idx)
        {
            buf0[idx] = amplitude_ * getNextSample();
        }

        // Duplicate signal across all channels
        for (unsigned int chan_idx = 1;
             chan_idx < bufferToFill.buffer->getNumChannels();
             ++chan_idx)
        {
            auto* buf = bufferToFill.buffer->getWritePointer(chan_idx);
            for (unsigned int idx = 0; idx < bufferToFill.numSamples; ++idx)
            {
                buf[idx] = buf0[idx];
            }
        }
    }

    /**
    Get the next sample in the wavetable based on the current index and the step size.
     TODO
    */
    forcedinline float getNextSample() noexcept
    {
        jassert(table_size_ > 0);

        auto index0 = (unsigned int) current_index_;
        auto index1 = index0 + 1;

        // Get error between truncated current current angle index and truncated index

        // Read the values in the table at current and next indices

        // Interpolate between the current value and the next value in the table

        // Wrap around back to the beginning of the table
        
        return 0.0;
    }
    
    /**
        Build the wavetable here using the table size.
        TODO
    */
    void buildWavetable()
    {
        wavetable_.setSize(1, table_size_ + 1);
        auto* samples = wavetable_.getWritePointer(0);
 
        // Fill with one period of sine
        // TODO
    }

private:
    // Begin wavetable data
    juce::AudioSampleBuffer wavetable_;
    int table_size_ = 4096;
    double sample_rate_ = 48000.0;
    float amplitude_ = 0.0f;
    float frequency_ = 440.0f;
    float current_index_ = 0.0f, table_delta_ = 0.0f;
    // End wavetable data

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableSynth)
};

//========================Misc Graphics Stuff=================================//


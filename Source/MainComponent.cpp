#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
 
    midi_keyboard_state_one_.addListener(this);
    midi_keyboard_state_two_.addListener(this);
    midi_keyboard_one_.reset(new juce::MidiKeyboardComponent(midi_keyboard_state_one_,
                             juce::KeyboardComponentBase::Orientation::horizontalKeyboard));
    midi_keyboard_two_.reset(new juce::MidiKeyboardComponent(midi_keyboard_state_two_,
                             juce::KeyboardComponentBase::Orientation::horizontalKeyboard));
    addAndMakeVisible(midi_keyboard_one_.get());
    addAndMakeVisible(midi_keyboard_two_.get());

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 300);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    wavetable_synth_.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    wavetable_synth_.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    auto local_bounds = getLocalBounds();
    midi_keyboard_one_->setBounds(local_bounds.removeFromTop(100));
    midi_keyboard_two_->setBounds(local_bounds.removeFromTop(100));
}

void MainComponent::handleNoteOn(juce::MidiKeyboardState* source, int, int midiNoteNumber, float)
{
    wavetable_synth_.setFrequency(midiToFreq((juce::uint8) midiNoteNumber));
    wavetable_synth_.setAmplitude(0.5);
}

void MainComponent::handleNoteOff(juce::MidiKeyboardState* source, int, int midiNoteNumber, float)
{
    wavetable_synth_.setAmplitude(0.0);
}


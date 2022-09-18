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

    for (size_t synth_idx = 0; synth_idx < kNumSynths; ++synth_idx)
    {
        auto* new_keyboard = new SynthKeyboard();
        addAndMakeVisible(new_keyboard);
        mixer_.addInputSource(new_keyboard, false);
        synths_.add(new_keyboard);
    }

    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, kNumSynths * kKeyboardHeight);
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    mixer_.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixer_.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    mixer_.releaseResources();
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
    for (auto* keyboard : synths_)
    {
        keyboard->setBounds(local_bounds.removeFromTop(kKeyboardHeight));
    }
}

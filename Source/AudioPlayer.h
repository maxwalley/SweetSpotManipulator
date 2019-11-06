/*
  ==============================================================================

    AudioPlayer.h
    Created: 3 Nov 2019 1:00:25pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class AudioPlayer    : public Component,
                       public AudioSource,
                       public Button::Listener
{
public:
    AudioPlayer();
    ~AudioPlayer();

    void paint (Graphics&) override;
    void resized() override;
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    void setGain(float gain);

private:
    FileChooser fileChooser;
    File selectedFile;
    AudioFormatManager audioFormatManager;
    std::unique_ptr<AudioFormatReaderSource> audioFormatReaderSource;
    AudioTransportSource audioTransportSource;
    
    Label timeLabel;
    
    TextButton playButton;
    TextButton pauseButton;
    TextButton stopButton;
    TextButton openFileButton;
    
    
    void buttonClicked(Button* button) override;
    
    enum TransportState
    {
        playing,
        stopped,
        paused,
    };
    uint64_t pausePosition;
    
    TransportState transportState;
    
    void timeCalculations();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayer)
};

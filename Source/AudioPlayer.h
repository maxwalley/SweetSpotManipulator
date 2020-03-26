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
/**
    A component class for an audio player
*/
class AudioPlayer    : public Component,
                       public AudioSource,
                       public Button::Listener,
                       public Timer
{
public:
    /**Contructor*/
    AudioPlayer();
    
    /**Destructor*/
    ~AudioPlayer();

    /**Implementation of the JUCE Component method*/
    void paint (Graphics&) override;
    
    /**Implementation of the JUCE Component method*/
    void resized() override;
    
    /**Implementation of the JUCE AudioSource method*/
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    
    /**Implementation of the JUCE AudioSource method*/
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    
    /**Implementation of the JUCE AudioSource method*/
    void releaseResources() override;
    
    /**Sets the gain to apply to audio going through the player
     
     @param gain            the amount of gain to apply (between 0 and1)
     */
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
    
    /**Implementation of the JUCE Button::Listener method*/
    void buttonClicked(Button* button) override;
    
    /**Implementation of the JUCE Timer method*/
    void timerCallback() override;
    
    enum TransportState
    {
        playing,
        stopped,
        paused,
    };
    uint64_t pausePosition;
    
    TransportState transportState;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPlayer)
};

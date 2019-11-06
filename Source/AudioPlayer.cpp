/*
  ==============================================================================

    AudioPlayer.cpp
    Created: 3 Nov 2019 1:00:25pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioPlayer.h"

//==============================================================================
AudioPlayer::AudioPlayer() : fileChooser("Pick a file", File(), "*.wav", true, true, nullptr), playButton("Play"), pauseButton("Pause"), stopButton("Stop"), openFileButton("Open File")
{
    setSize (200, 150);
    
    audioFormatManager.registerBasicFormats();
       
    addAndMakeVisible(playButton);
    playButton.addListener(this);
    playButton.setClickingTogglesState(true);
    playButton.setToggleState(false, dontSendNotification);
       
    addAndMakeVisible(pauseButton);
    pauseButton.addListener(this);
    pauseButton.setClickingTogglesState(true);
    pauseButton.setToggleState(false, dontSendNotification);
       
    addAndMakeVisible(stopButton);
    stopButton.addListener(this);
    stopButton.setClickingTogglesState(true);
    stopButton.setToggleState(true, dontSendNotification);
       
    addAndMakeVisible(openFileButton);
    openFileButton.addListener(this);

    addAndMakeVisible(timeLabel);
    timeLabel.setText("0", dontSendNotification);
       
    transportState = stopped;
    pausePosition = 0;
}

AudioPlayer::~AudioPlayer()
{
}

void AudioPlayer::paint (Graphics& g)
{
    playButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::darkred);
    pauseButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::darkred);
    stopButton.setColour(TextButton::ColourIds::buttonOnColourId, Colours::darkred);
}

void AudioPlayer::resized()
{
    openFileButton.setBounds(0, 0, 150, 50);
    playButton.setBounds(0, 50, 50, 50);
    pauseButton.setBounds(50, 50, 50, 50);
    stopButton.setBounds(100, 50, 50, 50);
    timeLabel.setBounds(0, 110, 150, 40);
}

void AudioPlayer::buttonClicked(Button* button)
{
    if(button == &openFileButton)
    {
        if(fileChooser.browseForFileToOpen() == true)
        {
            selectedFile = fileChooser.getResult();
            
            AudioFormatReader* audioFormatReader = audioFormatManager.createReaderFor(selectedFile);
            
            if(audioFormatReader != nullptr)
            {
                std::unique_ptr<AudioFormatReaderSource> tempAudioFormatReaderSource (new AudioFormatReaderSource (audioFormatReader, true));
            
                audioTransportSource.setSource(tempAudioFormatReaderSource.get(), 0, nullptr, audioFormatReader->sampleRate, audioFormatReader->numChannels);
            
                audioFormatReaderSource.reset(tempAudioFormatReaderSource.release());
            }
            else
            {
                DBG("No readers for that file");
            }
        }
    }
    
    else if(button == &playButton)
    {
        if(transportState == stopped)
        {
            audioTransportSource.setPosition(0);
        }
        else if(transportState == paused)
        {
            audioTransportSource.setPosition(pausePosition);
        }
        audioTransportSource.start();
        playButton.setEnabled(false);
        stopButton.setToggleState(false, dontSendNotification);
        stopButton.setEnabled(true);
        pauseButton.setToggleState(false, dontSendNotification);
        pauseButton.setEnabled(true);
        transportState = playing;
    }
    
    else if(button == &stopButton)
    {
        audioTransportSource.stop();
        stopButton.setEnabled(false);
        pauseButton.setToggleState(false, dontSendNotification);
        pauseButton.setEnabled(false);
        playButton.setToggleState(false, dontSendNotification);
        playButton.setEnabled(true);
        transportState = stopped;
    }
    
    else if (button == &pauseButton)
    {
        pausePosition = audioTransportSource.getCurrentPosition();
        audioTransportSource.stop();
        pauseButton.setEnabled(false);
        playButton.setToggleState(false, dontSendNotification);
        playButton.setEnabled(true);
        stopButton.setToggleState(false, dontSendNotification);
        stopButton.setEnabled(true);
        transportState = paused;
    }
    
}

void AudioPlayer::setGain(float gain)
{
    audioTransportSource.setGain(gain);
}

void AudioPlayer::timeCalculations()
{
    int64_t numMins = floor(audioTransportSource.getCurrentPosition() / 60);
    double numSecs = fmod(audioTransportSource.getCurrentPosition(), 60);
    double decPoint = fmod(numSecs, 1);
    int64_t numSecsInt = numSecs - decPoint;
    
    std::string numMinsString = std::to_string(numMins);
    std::string numSecsString = std::to_string(numSecsInt);
    
    std::string fullTime = numMinsString + ":" + numSecsString;
    
    //DBG("Time is: " << fullTime);
    
    const MessageManagerLock labelLock;
    timeLabel.setText(fullTime, dontSendNotification);
}

void AudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    audioTransportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void AudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
    if(audioFormatReaderSource.get() == nullptr)
    {
        bufferToFill.clearActiveBufferRegion();
    }
    else
    {
        audioTransportSource.getNextAudioBlock(bufferToFill);
        timeCalculations();
    }
}

void AudioPlayer::releaseResources()
{
    audioTransportSource.releaseResources();
}

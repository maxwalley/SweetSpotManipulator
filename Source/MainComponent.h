/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ChannelArray.h"
#include "LightButtonArray.h"
#include "Kinect.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...
    
    void buttonClickedEvent();
    
    ChannelArray Channels;
    LightButtonArray Lights;
    AudioDeviceManager UserSelectedDevice;
    AudioDeviceSelectorComponent UserSelectedDeviceSettings;
    
    float CMultiplier;
    float LFMultiplier;
    float RFMultiplier;
    float LRMultiplier;
    float RRMultiplier;
    float LFEMultiplier;
    enum MultiplierState
    {
        up,
        down
    };
    MultiplierState CMultiState;
    MultiplierState LFMultiState;
    MultiplierState RFMultiState;
    MultiplierState LRMultiState;
    MultiplierState RRMultiState;
    MultiplierState LFEMultiState;
    
    int16_t LFsamplecount;
    int16_t RFsamplecount;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    
    AudioFormatManager FormatManager;
    FileChooser chooser;
    TextButton OpenFileButton;
    TextButton PlayPauseButton;
    TextButton rewindButton;
    File playFile;
    
    Kinect kin;
    
    Image kinPic;
};

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
    
    float channel1Multiplier;
    float channel2Multiplier;
    
    int channel1SampleCount;
    int channel2SampleCount;
    
    enum multiplierState
    {
        up = 0,
        down = 1
    };
    multiplierState channel1State;
    multiplierState channel2State;
    
    Slider masterSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    
    AudioFormatManager FormatManager;
    FileChooser chooser;
    TextButton OpenFileButton;
    TextButton PlayPauseButton;
    TextButton rewindButton;
    File playFile;
    
    Kinect kin;
    
    Image kinPic;
    Image rgbPic;
    void setDepthPixels();
    void setRGBPixels();
    void paintImage();
    
    int audioBlockCount;
    
    //Bool that returns false if any kinect error code is detected
    bool kinectErrorCodeTriggered;

};

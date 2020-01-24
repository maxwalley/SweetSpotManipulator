/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ChannelArray.h"
#include "LightControls.h"
#include "Kinect.h"
#include "BalanceControls.h"
#include <math.h>
#include "AudioPlayer.h"
#include "KinImage.h"
#include "Delay.h"
#include <opencv2/opencv.hpp>
#include "HaarCascade.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public Button::Listener,
                        public Slider::Listener,
                        public Timer
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
    
    //Bool that returns true if any kinect error code is detected
    bool kinectErrorCodeTriggered;
    
    void buttonClicked(Button* button) override;
    
    AudioDeviceManager UserSelectedDevice;
    AudioDeviceSelectorComponent UserSelectedDeviceSettings;
    
    AudioPlayer audioPlayer;
    
    Slider masterSlider;
    Label masterSliderLabel;
    float masterSliderValue;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    
    BalanceControls balance;
    
    void sliderValueChanged(Slider* slider) override;
    
    TextButton CVWindowButton;
    TextButton closeCVWindow;
    void timerCallback() override;
    
    //Takes image scans it for data and prints where it finds data. Has to start x axis at 10 since there seems to be a border around the image
    void workOutDepthAtPosition();
    
    Delay delay;
    double currentSampleRate;
    
    HaarCascade haarCascade;
    int16_t depthAtUserPos;
    int16_t userPosX;
    
    Kinect kinect;
    
    Slider speakerLineDistanceSlider;
    
    Slider leftChannelGainSlider;
    Slider rightChannelGainSlider;
    
    void repaintSliders();
    
};

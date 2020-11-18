/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "KinectController.h"
#include "DistanceBasedAttenuationCalculator.h"
#include <math.h>
#include "AudioPlayer.h"
#include "Delay.h"
#include <opencv2/opencv.hpp>
#include "HaarCascade.h"

//==============================================================================
/**
    The main window of the application. Holding all GUI components and allowing objects to communicate with one another.
*/
class MainComponent   : public AudioAppComponent,
                        public Slider::Listener,
                        public Timer
{
public:
    //==============================================================================
    /**Constructor*/
    MainComponent();
    
    /**Destructor*/
    ~MainComponent();

    //==============================================================================
    /**Implementation of the JUCE AudioSource method*/
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    
    /**Implementation of the JUCE AudioSource method*/
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    
    /**Implementation of the JUCE AudioSource method*/
    void releaseResources() override;

    //==============================================================================
    /**Implementation of the JUCE Component method*/
    void paint (Graphics& g) override;
    
    /**Implementation of the JUCE Component method*/
    void resized() override;
    
private:
    //==============================================================================
    // Your private member variables go here...
    
    AudioDeviceManager UserSelectedDevice;
    AudioDeviceSelectorComponent UserSelectedDeviceSettings;
    
    AudioPlayer audioPlayer;
    
    Slider masterSlider;
    Label masterSliderLabel;
    float masterSliderValue;
    
    DistanceBasedAttenuationCalculator attenuationCalculator;
    
    /**Implementation of the JUCE Slider::Listener method*/
    void sliderValueChanged(Slider* slider) override;
    
    /**Implementation of the JUCE Timer method*/
    void timerCallback() override;
    
    /**Sends an image to the HaarCascade for human detection. Stores the pixel location of detection and the depth data at that pixel location*/
    void workOutDepthAtPosition();
    
    Delay delay;
    double currentSampleRate;
    
    HaarCascade haarCascade;
    int16_t depthAtUserPos;
    int16_t userPosX;
    
    KinectController kinect;
    
    Label leftChannelGainLabel;
    Slider leftChannelGainSlider;
    Label rightChannelGainLabel;
    Slider rightChannelGainSlider;
    
    Label speaker0xCoOrdSliderLabel;
    Slider speaker0xCoOrdSlider;
    Label speaker0zCoOrdSliderLabel;
    Slider speaker0zCoOrdSlider;
    
    Label speaker1xCoOrdSliderLabel;
    Slider speaker1xCoOrdSlider;
    Label speaker1zCoOrdSliderLabel;
    Slider speaker1zCoOrdSlider;
    
    Slider distanceToSimulateSlider;
    Label distanceToSimulateLabel;
    
    /**Refreshes the gain sliders to show the current gains for each channel*/
    void repaintSliders();
  
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};

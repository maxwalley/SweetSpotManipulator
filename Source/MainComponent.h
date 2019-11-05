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
//#include "PanningControls.h"
#include "BalanceControls.h"
#include <math.h>
#include "AudioPlayer.h"
#include <opencv2/opencv.hpp>
#include "KinImage.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent,
                        public Button::Listener,
                        public Slider::Listener
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
    
    void buttonClickedEvent();
    void buttonClicked(Button* button) override;
    
    ChannelArray Channels;
    LightControls Lights;
    AudioDeviceManager UserSelectedDevice;
    AudioDeviceSelectorComponent UserSelectedDeviceSettings;
    
    AudioPlayer audioPlayer;
    
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
    Label masterSliderLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
    
    //Kinect kin;
    
    Image kinPic;
    Image rgbPic;
    void setDepthPixels();
    void setRGBPixels();
    void paintImage();
    
    //PanningControls panningLaw;
    BalanceControls balance;
    
    int audioBlockCount;

    float workOutValue(float multiplier, int channel);
    
    TextButton kinUpButton;
    TextButton kinDownButton;
    
    ComboBox audioOutSelector;
    
    void sliderValueChanged(Slider* slider) override;
    
    void displayDepthImageCV();
    TextButton CVWindowButton;
    
    KinImage kinectImage;
    
    int kinRefreshRate;
};

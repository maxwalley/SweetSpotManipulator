/*
  ==============================================================================

    Kinect.h
    Created: 21 Jun 2019 12:58:51pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <stdio.h>
#include <libfreenect.h>
#include <opencv2/opencv.hpp>

//==============================================================================
/*
*/
class Kinect : public Timer
{
public:
    Kinect();
    ~Kinect();
    
    int kinInit();
    int kinTiltUp();
    int kinTiltDown();
    int RunVidandDepth() const;
    int End() const;
    int checkLed(freenect_led_options ledState);
    
    static unsigned short depthArray[480][640];
    
    static unsigned short colourArray[3][480][640];

    int16_t getNumberDevices() const;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Kinect)
    
    freenect_device* dev;
    freenect_context* ctx;
    int16_t numDev;
    freenect_raw_tilt_state* state;
    freenect_frame_mode DepthMode;
    freenect_frame_mode VideoMode;
    
    bool deviceConnected;
    
    int8_t currentTilt;
    
    static void depthCallback(freenect_device* dev, void* data, uint32_t timestamp);
    static void videoCallback(freenect_device* dev, void* data, uint32_t timestamp);
    
    void timerCallback() override;
};




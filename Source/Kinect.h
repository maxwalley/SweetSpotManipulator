/*
  ==============================================================================

    Kinect.h
    Created: 21 Jun 2019 12:58:51pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "Callbacks.hpp"
#include "../JuceLibraryCode/JuceHeader.h"
#include <../Builds/MacOSX/Dependencies/libfreenect/include/libfreenect/libfreenect.h>
#include <stdio.h>

//==============================================================================
/*
*/
class Kinect
{
public:
    Kinect();
    ~Kinect();
    
    void InitandMove();
    void RunVidandDepth() const;
    void End() const;
    
    void checkLed(freenect_led_options ledState);
    
    void rebuildArray();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Kinect)
    
    freenect_device* dev;
    freenect_context* ctx;
    int16_t numDev;
    freenect_raw_tilt_state* state;
    freenect_loglevel loglvl = FREENECT_LOG_FLOOD;
    freenect_frame_mode DepthMode;
    freenect_frame_mode VideoMode;
    
    Callbacks CB;
    //static void DepthCB(freenect_device* dev, void* data, uint32_t timestamp);
    
    //static uint16_t depthArray[640][480];
};

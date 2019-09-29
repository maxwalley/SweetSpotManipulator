/*
  ==============================================================================

    Kinect.h
    Created: 21 Jun 2019 12:58:51pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Callbacks.hpp"

//==============================================================================
/*
*/
class Kinect    : public Component
{
public:
    Kinect();
    ~Kinect();

    void paint (Graphics&) override;
    void resized() override;
    
    void InitandMove();
    void RunVidandDepth();
    void End();
    
    void checkLed(freenect_led_options ledState);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Kinect)
    
    freenect_device* dev;
    freenect_context* ctx;
    int16_t numDev;
    freenect_raw_tilt_state* state;
    freenect_loglevel loglvl = FREENECT_LOG_FLOOD;
    freenect_device_flags CamFlag = FREENECT_DEVICE_CAMERA;
    freenect_device_flags MotFlag = FREENECT_DEVICE_MOTOR;
    freenect_frame_mode DepthMode;
    freenect_frame_mode VideoMode;
};

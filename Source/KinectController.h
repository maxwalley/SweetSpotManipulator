/*
  ==============================================================================

    KinectController.h
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
/**
    A class that initialises and runs a Microsoft Kinect
*/
class KinectController : public Timer
{
public:
    /**Constructor*/
    KinectController();
    
    /**Destructor*/
    ~KinectController();
    
    /**Initialises the kinect
     
     @returns 0 if initialisation is successful
     */
    int kinInit();
    
    //int kinTiltUp();
    //int kinTiltDown();
    
    /**Causes the kinect to start returning colour and depth images
     
     @returns 0 if successful
     */
    int RunVidandDepth() const;
    
    /**Closes down the kinect
     
     @returns 0 if successful
     */
    int End() const;
    
    //int checkLed(freenect_led_options ledState);
    
    static unsigned short depthArray[480][640];
    
    static unsigned short colourArray[3][480][640];

    /**Returns the number of connected Kinect devices
     
     @returns the number of connected Kinect devices
     */
    int16_t getNumberDevices() const;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KinectController)
    
    freenect_device* dev;
    freenect_context* ctx;
    int16_t numDev;
    freenect_raw_tilt_state* state;
    freenect_frame_mode DepthMode;
    freenect_frame_mode VideoMode;
    
    bool deviceConnected;
    
    int8_t currentTilt;
    
    /**The callback that the kinect returns depth image data into
     
     @param dev         the device the data is being returned from
     @param data            the depth image data
     @param timestamp           the time the data was received into the callback
     */
    static void depthCallback(freenect_device* dev, void* data, uint32_t timestamp);
    
    /**The callback that the kinect returns colour image data into
    
    @param dev         the device the data is being returned from
    @param data            the colour image data
    @param timestamp           the time the data was received into the callback
    */
    static void videoCallback(freenect_device* dev, void* data, uint32_t timestamp);
    
    /**Implementation of the JUCE Timer method*/
    void timerCallback() override;
};




/*
  ==============================================================================

    Kinect.cpp
    Created: 21 Jun 2019 12:58:51pm
    Author:  Maximillian Walley

  ==============================================================================
*/
#include "Kinect.h"

uint16_t Kinect::depthArray[640][480];

uint16_t Kinect::redArray[640][480];
uint16_t Kinect::greenArray[640][480];
uint16_t Kinect::blueArray[640][480];

cv::Mat Kinect::depthImageCV;


//==============================================================================
Kinect::Kinect()
{
    depthImageCV.create(480, 640, CV_16UC2);
}

Kinect::~Kinect()
{
}

int Kinect::kinInit()
{
    if(freenect_init(&ctx, NULL) != 0)
    {
        DBG("Freenect Init failed");
        return 1;
    }
    
    numDev = freenect_num_devices(ctx);
    
    if(numDev >= 1)
    {
        DBG("Number of Connected Devices is: " << numDev);
        
        if(freenect_open_device(ctx, &dev, 0) != 0)
        {
            DBG("Error Opening Device");
            return 2;
        }
        else
        {
            DBG("Device Opened Successfully");
        }
        
        state = freenect_get_tilt_state(dev);
        
        DepthMode = freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT);
        VideoMode = freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB);
        
        if(freenect_set_depth_mode(dev, DepthMode) != 0)
        {
            DBG("Error Setting Depth Mode");
            return 3;
        }
        else
        {
            DBG("Depth Mode Set Correctly");
        }
        
        if(freenect_set_video_mode(dev, VideoMode) != 0)
        {
            DBG("Error Setting Video Mode");
            return 4;
        }
        else
        {
            DBG("Video Mode Set Correctly");
        }
        
        DBG("Required Depth Buffer Size is: " << DepthMode.bytes);
        DBG("Depth Width is: " << DepthMode.width);
        DBG("Depth Height is: " << DepthMode.height);
        DBG("Bits Per Pixel of Depth Frame is: " << DepthMode.data_bits_per_pixel);
        
        freenect_set_depth_callback(dev, depthCallback);
        //freenect_set_video_callback(dev, videoCallback);
        
        if(freenect_start_depth(dev) != 0)
        {
            DBG("Error Starting Depth Stream");
            return 5;
        }
        else
        {
            DBG("Depth Stream Started");
        }
        
        if(freenect_start_video(dev) != 0)
        {
            DBG("Error Starting Video Stream");
            return 6;
        }
        else
        {
            DBG("Video Stream Started");
        }
    }
    
    else if(numDev == 0)
    {
        DBG("No Devices Found");
        return 7;
    }
    return 0;
}

int Kinect::kinTiltUp()
{
    if(state->tilt_angle >= 30)
    {
        DBG("Kinect tilt at limit");
    }
    else
    {
        if(freenect_set_tilt_degs(dev, state->tilt_angle + 5) != 0)
        {
            DBG("Error setting tilt angle");
            return 11;
        }
        do
        {
            if(freenect_update_tilt_state(dev) != 0)
            {
                DBG("Error updating tilt state");
                return 12;
            }
            state = freenect_get_tilt_state(dev);
        }
        while(state->tilt_status == TILT_STATUS_MOVING);
    }
    return 0;
}

int Kinect::kinTiltDown()
{
    if(state->tilt_angle <= -30)
    {
        DBG("Kinect tilt at limit");
    }
    else
    {
        if(freenect_set_tilt_degs(dev, state->tilt_angle - 5) != 0)
        {
            DBG("Error setting tilt angle");
            return 13;
        }
        do
        {
            if(freenect_update_tilt_state(dev) != 0)
            {
                DBG("Error updating tilt state");
                return 14;
            }
            state = freenect_get_tilt_state(dev);
        }
        while(state->tilt_status == TILT_STATUS_MOVING);
    }
    return 0;
}

int Kinect::RunVidandDepth() const
{
    if(freenect_process_events(ctx) != 0)
    {
        DBG("Error Processing Events");
        return 21;
    }
    else
    {
        DBG("Processing Events");
    }
    return 0;
}

int Kinect::End() const
{
    if(freenect_stop_depth(dev) != 0)
    {
        DBG("Depth stream stop failed");
        return 31;
    }
    
    if(freenect_stop_video(dev) != 0)
    {
        DBG("Video stream stop failed");
        return 32;
    }
    
    if(freenect_close_device(dev) != 0)
    {
        DBG("Error closing freenect device");
        return 33;
    }
    
    if(freenect_shutdown(ctx) != 0)
    {
        DBG("Error freeing the context");
        return 34;
    }
    
    return 0;
}

int Kinect::checkLed(freenect_led_options ledState)
{
    if(freenect_set_led(dev, ledState) != 0)
    {
        DBG("Error setting kinect LED");
        return 41;
    }
    
    return 0;
}

void Kinect::depthCallback(freenect_device* dev, void* data, uint32_t timestamp)
{
    uint16_t* castedData = static_cast<uint16_t*>(data);
    
    DBG("Depth data received");
    
    //cv::Mat tempMat(480, 640, CV_16UC2, data);
    //depthImageCV = tempMat;
//    ScopedLock
//    isBusy = true;
    for(int yCount = 0; yCount < 480; yCount++)
    {
        for(int xCount = 0; xCount < 640; xCount++)
        {
            depthArray[xCount][yCount] = *castedData;
            //Uncomment this if you want to inverse the colours
            //depthArray[xCount][yCount] = (depthArray[xCount][yCount] - 255) * -1;
            castedData++;
        }
    }
//    isBusy = false;
}

void Kinect::videoCallback(freenect_device* dev, void* data, uint32_t timestamp)
{
    uint16_t* castedData = static_cast<uint16_t*>(data);
    
    for(int yCount = 0; yCount < 480; yCount++)
    {
        for(int xCount = 0; xCount < 640; xCount++)
        {
            printf("vid Data = %d\n", *castedData);
            redArray[xCount][yCount] = *castedData;
            //redArray[xCount][yCount] = (redArray[xCount][yCount] - 255) * -1;
            castedData++;
            greenArray[xCount][yCount] = *castedData;
            //greenArray[xCount][yCount] = (greenArray[xCount][yCount] - 255) * -1;
            castedData++;
            blueArray[xCount][yCount] = *castedData;
            //blueArray[xCount][yCount] = (blueArray[xCount][yCount] - 255) * -1;
            castedData++;
        }
    }
}

cv::Mat Kinect::getDepthImageCV()
{
    return depthImageCV;
}


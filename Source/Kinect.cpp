/*
  ==============================================================================

    Kinect.cpp
    Created: 21 Jun 2019 12:58:51pm
    Author:  Maximillian Walley

  ==============================================================================
*/
#include "Kinect.h"

//==============================================================================
Kinect::Kinect()
{
    
}

Kinect::~Kinect()
{
}

void Kinect::InitandMove()
{
    freenect_init(&ctx, NULL);
    numDev = freenect_num_devices(ctx);
    
    if(numDev >= 1)
    {
        printf("Number of Connected Devices is: %d\n", numDev);
        
        if(freenect_open_device(ctx, &dev, 0) < 0)
        {
            printf("Error Opening Device");
            freenect_shutdown(ctx);
        }
        else
        {
            printf("Device Opened Successfully");
        }
        
        freenect_set_tilt_degs(dev, 30);
        do
        {
            freenect_update_tilt_state(dev);
            state = freenect_get_tilt_state(dev);
            printf("%d\n", state->tilt_status);
        }
        while(state->tilt_status == TILT_STATUS_MOVING);
    
        freenect_set_tilt_degs(dev, 10);
        do
        {
            freenect_update_tilt_state(dev);
            state = freenect_get_tilt_state(dev);
            //printf("%d\n", state->tilt_status);
        }
        while(state->tilt_status == TILT_STATUS_MOVING);
        
        DepthMode = freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_11BIT);
        VideoMode = freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB);
        
        if(freenect_set_depth_mode(dev, DepthMode) < 0)
        {
            printf("Error Setting Depth Mode\n");
            freenect_shutdown(ctx);
        }
        else
        {
            printf("Depth Mode Set Correctly\n");
        }
        
        if(freenect_set_video_mode(dev, VideoMode) < 0)
        {
            printf("Error Setting Video Mode\n");
            freenect_shutdown(ctx);
        }
        else
        {
            printf("Video Mode Set Correctly\n");
        }
        
        printf("Required Depth Buffer Size is: %d\n", DepthMode.bytes);
        printf("Depth Width is: %d\n", DepthMode.width);
        printf("Depth Height is: %d\n", DepthMode.height);
        printf("Bits Per Pixel of Depth Frame is: %d\n", DepthMode.data_bits_per_pixel);
        
        //Cb.openRawDataFiles();
        
        
        // maybe as part of some extention - we could do this
        //uint16_t depthArray[640][480];
        //DepthCallback(0, depthArray, -1111); // tell our callback where our external DepthArray is
        
        freenect_set_depth_callback(dev, CB.DepthCallback);
        //freenect_set_video_callback(dev, VideoCallback);
        freenect_set_log_level(ctx, loglvl);
        //freenect_set_log_callback(ctx, *LogCallback);
        
        if(freenect_start_depth(dev) == 0)
        {
            printf("Depth Stream Started\n");
        }
        else
        {
            printf("Error Starting Depth Stream\n");
            freenect_shutdown(ctx);
        }
        
        if(freenect_start_video(dev) == 0)
        {
            printf("Video Stream Started\n");
        }
        else
        {
            printf("Error Starting Video Stream\n");
            freenect_shutdown(ctx);
        }
    }
    
    else if(numDev == 0)
    {
        printf("No Devices Found");
        freenect_shutdown(ctx);
    }
}

void Kinect::RunVidandDepth() const
{
    if(freenect_process_events(ctx) != 0)
    {
        printf("Error Processing Events");
        End();
    }
    else
    {
    }
}

void Kinect::End() const
{
    if(freenect_stop_depth(dev) == 0)
    {
        printf("Depth Stream Ended Successfully\n");
    }
    
    if(freenect_stop_video(dev) == 0)
    {
        printf("Video Stream Ended Successfully\n");
    }
    
    if(freenect_close_device(dev) == 0)
    {
        printf("Freenect Device Closed Successfully\n");
    }
    
    if(freenect_shutdown(ctx) == 0)
    {
        printf("Freenect Shutdown Successfully\n");
    }
    //closeRawDataFiles();
}

void Kinect::checkLed(freenect_led_options ledState)
{
    freenect_set_led(dev, ledState);
}

//void Kinect::LogCallback(freenect_context* ctx, freenect_loglevel level, const char* msg)
//{
//    fprintf(Logtxt, "%c\n", &msg);
//}

/*void Kinect::DepthCB(freenect_device* dev, void* data, uint32_t timestamp)
{
    uint16_t* castedData = static_cast<uint16_t*>(data);
    
    for(int xCount = 0; xCount < 640; xCount++)
    {
        for(int yCount = 0; yCount < 480; yCount++)
        {
            depthArray[xCount][yCount] = *castedData;
            
            castedData++;
        }
    }
}*/

void Kinect::rebuildArray()
{
    uint16_t wholeArray[640][480];
    uint16_t* currentPixel;
    
    for(int i = 0; i < 480; i++)
    {
        currentPixel = CB.getDepthData(i);
        for(int c = 0; c < 640; c++)
        {
            wholeArray[c][i] = *currentPixel;
            currentPixel++;
        }
    }
}

//
//  Callbacks.hpp
//  NewProject - App
//
//  Created by Maximillian Walley on 02/07/2019.
//

#ifndef Callbacks_hpp
#define Callbacks_hpp

#include "../JuceLibraryCode/JuceHeader.h"
#include <../Builds/MacOSX/Dependencies/libfreenect/include/libfreenect/libfreenect.h>
#include <stdio.h>

#pragma once

class Callbacks
{
private:
    FILE* RawDepthDatatxt;
    FILE* RawVidDatatxt;
    FILE* Logtxt;
    static uint16_t depthArray[640][480];
    //static bool isBusy;
    
public:
    Callbacks();
    void openRawDataFiles();
    void closeRawDataFiles();
    static void DepthCallback(freenect_device*, void* data, uint32_t timestamp);
    void VideoCallback(freenect_device* dev, void* data, uint32_t timestamp);
    void LogCallback(freenect_context* ctx, freenect_loglevel level, const char* msg);
    uint16_t* getDepthData(int column);
};
#endif /* Callbacks_hpp */

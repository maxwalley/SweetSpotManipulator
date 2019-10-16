//
//  Callbacks.cpp
//  NewProject - App
//
//  Created by Maximillian Walley on 02/07/2019.
//

#include "Callbacks.hpp"

uint16_t Callbacks::depthArray[640][480];

Callbacks::Callbacks()
{
    //isBusy = false;
}

void Callbacks::openRawDataFiles()
{
    RawDepthDatatxt = fopen("RawDepth.txt", "w");
    if(RawDepthDatatxt == NULL)
    {
        printf("\nError - Opening Raw Depth Data File\n");
    }
    else
    {
        printf("\nRaw Depth Data File Opened Successfully\n");
        fprintf(RawDepthDatatxt, "\nRaw Depth Data File Opened Successfully\n");
    }
    
    RawVidDatatxt = fopen("RawText.txt", "w");
    if(RawVidDatatxt == NULL)
    {
        printf("Error - Opening Raw Vid Data File\n");
    }
    else
    {
        printf("Raw Vid Data File Opened Successfully\n");
    }
    
    Logtxt = fopen("Log.txt", "w");
    if(Logtxt == NULL)
    {
        printf("\nError - Opening Log File\n");
    }
    else
    {
        printf("\nLog File Opened Successfully\n");
        fprintf(RawDepthDatatxt, "\nLog File Opened Successfully\n");
    }
}

void Callbacks::closeRawDataFiles()
{
    if(fclose(RawDepthDatatxt) == 0)
    {
        printf("\nRaw Depth Data File Closed Successfully\n");
    }
    else
    {
        printf("\nError - Closing Raw Depth Data File\n");
    }
    
    if(fclose(RawVidDatatxt) == 0)
    {
        printf("Raw Vid Data File Closed Successfully\n");
    }
    else
    {
        printf("Error - Closing Raw Vid Data File\n");
    }
    
    if(fclose(Logtxt) == 0)
    {
        printf("Log File Closed Successfully\n");
    }
    else
    {
        printf("Error - Closing Log File\n");
    }
}

void Callbacks::DepthCallback(freenect_device*, void* data, uint32_t timestamp)
{
    uint16_t* castedData = static_cast<uint16_t*>(data);
    
//    ScopedLock
//    isBusy = true;
    for(int xCount = 0; xCount < 640; xCount++)
    {
        for(int yCount = 0; yCount < 480; yCount++)
        {
            depthArray[xCount][yCount] = *castedData;
            castedData++;
        }
    }
//    isBusy = false;
}

void Callbacks::VideoCallback(freenect_device*, void* data, uint32_t timestamp)
{
    
}

/*void DepthCallback(freenect_device* dev, void* data, uint32_t timestamp)
{
// maybe as part of some extention we could do this
// set up a pointer to the EXTERNAL depthArray - only when the timestamp is the special value
//    static uint16_t* depthArray = nullptr;
//    if( timestamp = -1111 )
//    {
//        depthArray = static_cast<uint16_t*>data;
//    }
//
    
    uint16_t depthArray[640][480];
    //uint8_t shortDepthArray[640][480];
    
    fprintf(RawDepthDatatxt, "Depth Data Received at %u\n", timestamp);
    
    uint16_t* castedData = static_cast<uint16_t*>(data);
    //fprintf(RawDepthDatatxt, "Data: %hu\n", *castedData);
    
    for(int xCount = 0; xCount < 640; xCount++)
    {
        for(int yCount = 0; yCount < 480; yCount++)
        {
            depthArray[xCount][yCount] = *castedData;
            fprintf(RawDepthDatatxt, "%d\n", depthArray[xCount][yCount]);
            
            //shortDepthArray[xCount][yCount] = depthArray[xCount][yCount];
            //fprintf(RawDepthDatatxt, "%d\n", shortDepthArray[xCount][yCount]);
            
            castedData++;
        }
    }
}*/

//void VideoCallback(freenect_device* dev, void* data, uint32_t timestamp)
//{
    //fprintf(RawVidDatatxt, "Video Data Received at %u:\n", timestamp);
    //fprintf(RawVidDatatxt, "Size of data: %lu\n", sizeof(&data));
    
    //DataInfo* VidPtr = static_cast<DataInfo*>(data);

    //fprintf(RawVidDatatxt, "%d\n", VidPtr->data);
//}

uint16_t* Callbacks::getDepthData(int column)
{
    uint16_t depthLine[640];
    
    for(int i = 0; i < 640; i++)
    {
        depthLine[i] = depthArray[i][column];
    }
    return depthLine;
}

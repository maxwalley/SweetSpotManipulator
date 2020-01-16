/*
  ==============================================================================

    KinImage.cpp
    Created: 17 Oct 2019 3:46:21pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "KinImage.h"

//==============================================================================
KinImage::KinImage()
{
    setSize(320, 240);
    setBufferedToImage(true);
    setFramesPerSecond(25);
}

KinImage::~KinImage()
{
}

void KinImage::paint(Graphics &g)
{
    
}

void KinImage::update()
{
    kinect.RunVidandDepth();
}

void KinImage::kinectInit()
{
    int initErrorCode = kinect.kinInit();
    
    if(initErrorCode != 0)
    {
        DBG("Kinect init failed with error code: " << initErrorCode);
        kinectErrorCodeTriggered = true;
    }
    else if (initErrorCode == 0)
    {
        DBG("Kinect init completed");
        kinectErrorCodeTriggered = false;
    }
}

void KinImage::kinectEnd() const
{
    int kinEndErrorCode = kinect.End();
    if(kinEndErrorCode != 0)
    {
        DBG("Kinect closing sequence failed with error code: " << kinEndErrorCode);
    }
}

bool KinImage::active() const
{
    if(kinect.getNumberDevices() != 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

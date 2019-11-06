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
KinImage::KinImage() : kinPic(Image::PixelFormat::RGB, 640, 480, true)
{
    setSize(320, 240);
    setBufferedToImage(true);
    setFramesPerSecond(25);
}

KinImage::~KinImage()
{
}

void KinImage::paint (Graphics& g)
{
    g.drawImage(kinPic, 0, 0, 320, 240, 0, 0, 640, 480, false);
}

void KinImage::resized()
{
    
}

void KinImage::setImage(int row, int col, uint16_t pixel)
{
    kinPic.setPixelAt(row, col, Colour(pixel, pixel, pixel));
}

void KinImage::repaintImage()
{
    const MessageManagerLock paintLock;
    repaint();
}

void KinImage::update()
{
    kinect.RunVidandDepth();
    setDepthPixels();
}

void KinImage::setDepthPixels()
{
    //uint8_t shortArray[480][640];
    
    for(int i = 0; i < 640; i++)
    {
        for(int c = 0; c < 480; c++)
        {
            //shortArray[c][i] = kin.depthArray[c][i]/8.02745;
            //printf("Depth data = %hu\n", shortArray[c][i]);
            kinPic.setPixelAt(i, c, Colour(kinect.depthArray[i][c], kinect.depthArray[i][c], kinect.depthArray[i][c]));
        }
    }
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

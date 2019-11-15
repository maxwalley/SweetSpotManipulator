/*
  ==============================================================================

    KinImage.h
    Created: 17 Oct 2019 3:46:21pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Kinect.h"
#include "opencv2/opencv.hpp"

//==============================================================================
/*
*/
class KinImage    : public AnimatedAppComponent
{
public:
    KinImage();
    ~KinImage();

    void paint (Graphics&) override;
    void resized() override;

    void setImage(int row, int col, uint16_t pixel);
    void repaintImage();
    void kinectInit();
    void kinectEnd() const;
    
    bool active() const;
    
    cv::Mat depthImageCV;
    cv::Mat getCVImage();
    Kinect kinect;
    //unsigned short cvPtr*;
private:
    Image kinPic;
    void update() override;
    void setDepthPixels();
    
    bool kinectErrorCodeTriggered;
    
    //OpenCV openCV;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KinImage)
};

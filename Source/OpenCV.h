/*
  ==============================================================================

    OpenCV.h
    Created: 15 Nov 2019 12:13:23am
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <opencv2/opencv.hpp>

//==============================================================================
/*
*/
class OpenCV : public Thread, public Component
{
public:
    OpenCV();
    ~OpenCV();

    void getDepthImage(cv::Mat image);
private:
    void run() override;
    void showImage();
    cv::Mat testMat;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenCV)
};

/*
  ==============================================================================

    OpenCV.cpp
    Created: 15 Nov 2019 12:13:23am
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "OpenCV.h"

//==============================================================================
OpenCV::OpenCV() : Thread("openCV")
{
    startThread(1);
    testMat.create(480, 640, CV_16UC1);
    cv::namedWindow("Cv Image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Cv Image", testMat);
}

OpenCV::~OpenCV()
{
    stopThread(500);
}

void OpenCV::run()
{
    while (!threadShouldExit())
    {
        int time = Time::getMillisecondCounter();
        DBG("running");
        showImage();
        Time::waitForMillisecondCounter(time + 100);
    }
}

void OpenCV::getDepthImage(cv::Mat image)
{
    testMat = image;
}

void OpenCV::showImage()
{
    cv::imshow("Cv Image", testMat);
    cv::waitKey(25);
    cv::destroyAllWindows();
}

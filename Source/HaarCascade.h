/*
  ==============================================================================

    HaarCascade.h
    Created: 2 Dec 2019 9:10:13pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <opencv2/opencv.hpp>

//==============================================================================
/*
*/
class HaarCascade : public Timer
{
public:
    HaarCascade();
    ~HaarCascade();
    
    cv::Mat performCascade(cv::Mat inputFrame);
    int getPersonPointX() const;
    int getPersonPointY() const;

private:
    
    cv::String fullBodyModelFilePath;
    cv::CascadeClassifier fullBodyClassifier;
    
    cv::String faceModelFilePath;
    cv::CascadeClassifier faceClassifier;
    
    cv::String altFaceModelFilePathOne;
    cv::CascadeClassifier altFaceClassifierOne;
    
    cv::String altFaceModelFilePathTwo;
    cv::CascadeClassifier altFaceClassifierTwo;
    
    
    cv::String altFaceModelFilePathThree;
    cv::CascadeClassifier altFaceClassifierThree;
    
    cv::String altFaceModelFilePathFour;
    cv::CascadeClassifier altFaceClassifierFour;
    
    cv::CascadeClassifier* currentClassifier;
    
    bool personFound;
    void timerCallback() override;
    int timerCount;
    
    void switchCascadeClassifier();
    enum Classifier
    {
        fullBody = 1,
        face = 2,
        altFaceOne = 3,
        altFaceTwo = 4,
        altFaceThree = 5,
        altFaceFour = 6
    };
    Classifier classifier;
    
    cv::Point personPoint;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HaarCascade)
};

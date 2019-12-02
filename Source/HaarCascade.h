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

private:
    
    cv::String fullBodyModelFilePath;
    cv::CascadeClassifier fullBodyClassifier;
    
    cv::String upperBodyModelFilePath;
    cv::CascadeClassifier upperBodyClassifier;
    
    cv::String lowerBodyModelFilePath;
    cv::CascadeClassifier lowerBodyClassifier;
    
    cv::String faceModelFilePath;
    cv::CascadeClassifier faceClassifier;
    
    cv::CascadeClassifier* currentClassifier;
    
    bool personFound;
    void timerCallback() override;
    int timerCount;
    
    void switchCascadeClassifier();
    enum Classifier
    {
        fullBody = 1,
        upperBody = 2,
        lowerBody = 3,
        face = 4
    };
    Classifier classifier;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HaarCascade)
};

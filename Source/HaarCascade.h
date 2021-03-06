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
/**
    A class that detects humans in an image through Haar Cascade object detection.
*/
class HaarCascade : public Timer
{
public:
    /**Constructor*/
    HaarCascade();
    
    /**Destructor*/
    ~HaarCascade();
    
    /**Performs the detection on an image and draws a ring around the detected object
     
     @param inputFrame          The image to perform the detection on
     
     @returns the image with any detected objects marked on it
     */
    cv::Mat performCascade(cv::Mat inputFrame);
    
    
    /**Returns the detected humans pixel location on the X axis of the image used for detection
    
    @see performCascade
            
    @returns the detected humans pixel location on the X axis of the image used for detection
    */
    int getPersonPointX() const;
    
    
    /**Returns the detected humans pixel location on the Y axis of the image used for detection
    
    @see performCascade
            
    @returns the detected humans pixel location on the Y axis of the image used for detection
    */
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
    
    /**Implementation of the JUCE Timer method*/
    void timerCallback() override;
    int timerCount;
    
    /**Switches the current classifier being used to a new classifier*/
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

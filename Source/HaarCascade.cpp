/*
  ==============================================================================

    HaarCascade.cpp
    Created: 2 Dec 2019 9:10:13pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "HaarCascade.h"

//==============================================================================
HaarCascade::HaarCascade() : fullBodyModelFilePath("/Users/maxwalley/Documents/FYP/fyp-application/Haar Cascades/haarcascade_fullbody.xml"), faceModelFilePath("/Users/maxwalley/Documents/FYP/fyp-application/Haar Cascades/haarcascade_profileface.xml"), altFaceModelFilePathOne("/Users/maxwalley/Documents/FYP/fyp-application/Haar Cascades/haarcascade_frontalface_alt_tree.xml"), altFaceModelFilePathTwo("/Users/maxwalley/Documents/FYP/fyp-application/Haar Cascades/haarcascade_frontalface_alt.xml"), altFaceModelFilePathThree("/Users/maxwalley/Documents/FYP/fyp-application/Haar Cascades/haarcascade_frontalface_alt2.xml"), altFaceModelFilePathFour("/Users/maxwalley/Documents/FYP/fyp-application/Haar Cascades/haarcascade_frontalface_default.xml"), personFound(false), timerCount(0), classifier(face)
{
    fullBodyClassifier.load(fullBodyModelFilePath);
    faceClassifier.load(faceModelFilePath);
    altFaceClassifierOne.load(altFaceModelFilePathOne);
    altFaceClassifierTwo.load(altFaceModelFilePathTwo);
    altFaceClassifierThree.load(altFaceModelFilePathThree);
    altFaceClassifierFour.load(altFaceModelFilePathFour);
    
    currentClassifier = &altFaceClassifierOne;
}

HaarCascade::~HaarCascade()
{
    
}

cv::Mat HaarCascade::performCascade(cv::Mat inputFrame)
{
    //Performs Haar cascade
    //See: https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html
    cv::Mat inputGrey;
    cv::cvtColor(inputFrame, inputGrey, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(inputGrey, inputGrey);
    
    std::vector<cv::Rect> people;
    currentClassifier->detectMultiScale(inputGrey, people);
    
    //If something is detected
    if(people.size() > 0)
    {
        personFound = true;
        if(Timer::isTimerRunning() == true)
        {
            //Stops the switcher timer
            Timer::stopTimer();
            //DBG("Ending finder timer");
            timerCount = 0;
        }
    }
    else
    {
        personFound = false;
        if(Timer::isTimerRunning() == false)
        {
            //Starts the switcher timer
            Timer::startTimer(100);
            //DBG("Starting finder timer");
        }
    }
    
    if(personFound == true)
    {
        //Gets central detected co-ordinates
        personPoint.x = people[0].x + people[0].width/2;
        personPoint.y = people[0].y + people[0].height/2;
    }
    
    for ( size_t i = 0; i < people.size(); i++ )
    {
        //Draws a circle around the detected area. See https://docs.opencv.org/3.4/db/d28/tutorial_cascade_classifier.html
        cv::Point center(people[i].x + people[i].width/2, people[i].y + people[i].height/2);
        //DBG("center x axis = " << center.x << " Center y axis = " << center.y);
        cv::ellipse( inputFrame, center, cv::Size( people[i].width/2, people[i].height/2 ), 0, 0, 360, cv::Scalar( 0, 0, 255 ), 4 );
    }
    
    if(people.size()>0)
    {
        //DBG("Num Detections = " << people.size());
    }
    
    return inputFrame;
}

void HaarCascade::timerCallback()
{
    timerCount++;
    
    //If the timer is 10 or a multiple of 10
    if(timerCount % 10 == 0)
    {
        //Changes the current classifier
        switchCascadeClassifier();
    }
}

void HaarCascade::switchCascadeClassifier()
{
    //Changes the current classifier
    switch(classifier)
    {
        case fullBody:
            classifier = face;
            currentClassifier = &faceClassifier;
            break;
            
        case face:
            classifier = altFaceOne;
            currentClassifier = &altFaceClassifierOne;
            break;
            
        case altFaceOne:
            classifier = altFaceTwo;
            currentClassifier = &altFaceClassifierTwo;
            break;
            
        case altFaceTwo:
            classifier = altFaceThree;
            currentClassifier = &altFaceClassifierThree;
            break;
            
        case altFaceThree:
            classifier = altFaceFour;
            currentClassifier = &altFaceClassifierFour;
            break;
            
        case altFaceFour:
            classifier = fullBody;
            currentClassifier = &fullBodyClassifier;
            break;
    }
    
    //DBG("Cascade classifier switched to: " << classifier);
}

int HaarCascade::getPersonPointX() const
{
    return personPoint.x;
}

int HaarCascade::getPersonPointY() const
{
    int yPos = personPoint.y;
    return yPos;
}

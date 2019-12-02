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
HaarCascade::HaarCascade() : fullBodyModelFilePath("/Users/maxwalley/Documents/Final Year Project/Application/Haar Cascades/haarcascade_fullbody.xml"), upperBodyModelFilePath("/Users/maxwalley/Documents/Final Year Project/Application/Haar Cascades/haarcascade_upperbody.xml"), lowerBodyModelFilePath("/Users/maxwalley/Documents/Final Year Project/Application/Haar Cascades/haarcascade_lowerbody.xml"), faceModelFilePath("/Users/maxwalley/Documents/Final Year Project/Application/Haar Cascades/haarcascade_profileface.xml"), personFound(false), timerCount(0), classifier(face)
{
    fullBodyClassifier.load(fullBodyModelFilePath);
    upperBodyClassifier.load(upperBodyModelFilePath);
    lowerBodyClassifier.load(lowerBodyModelFilePath);
    faceClassifier.load(faceModelFilePath);
    
    currentClassifier = &faceClassifier;
}

HaarCascade::~HaarCascade()
{
    
}

cv::Mat HaarCascade::performCascade(cv::Mat inputFrame)
{
    cv::Mat inputGrey;
    cv::cvtColor(inputFrame, inputGrey, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(inputGrey, inputGrey);
    
    std::vector<cv::Rect> people;
    currentClassifier->detectMultiScale(inputGrey, people);
    
    if(people.size() > 0)
    {
        personFound = true;
        if(Timer::isTimerRunning() == true)
        {
            Timer::stopTimer();
            DBG("Ending finder timer");
            timerCount = 0;
        }
    }
    else
    {
        personFound = false;
        if(Timer::isTimerRunning() == false)
        {
            Timer::startTimer(100);
            DBG("Starting finder timer");
        }
    }
    
    for ( size_t i = 0; i < people.size(); i++ )
    {
        cv::Point center(people[i].x + people[i].width/2, people[i].y + people[i].height/2);
        cv::ellipse( inputFrame, center, cv::Size( people[i].width/2, people[i].height/2 ), 0, 0, 360, cv::Scalar( 0, 0, 255 ), 4 );
    }
    
    return inputFrame;
}

void HaarCascade::timerCallback()
{
    timerCount++;
    
    if(timerCount % 10 == 0)
    {
        switchCascadeClassifier();
    }
}

void HaarCascade::switchCascadeClassifier()
{
    switch(classifier)
    {
        case fullBody:
            classifier = upperBody;
            currentClassifier = &upperBodyClassifier;
            break;
            
        case upperBody:
            classifier = lowerBody;
            currentClassifier = &lowerBodyClassifier;
            break;
            
        case lowerBody:
            classifier = face;
            currentClassifier = &faceClassifier;
            break;
            
        case face:
            classifier = fullBody;
            currentClassifier = &fullBodyClassifier;
            break;
    }
    
    DBG("Cascade classifier switched to: " << classifier);
}

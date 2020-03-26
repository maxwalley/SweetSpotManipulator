/*
  ==============================================================================

    BalanceControls.cpp
    Created: 31 Oct 2019 10:43:20am
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "BalanceControls.h"

//==============================================================================
BalanceControls::BalanceControls()
{
    setSize(200, 40);
    
    maxYDis = 4.0;
    
    addAndMakeVisible(idealSpotSlider);
    idealSpotSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    idealSpotSlider.setRange(1, 4);
    idealSpotSlider.setValue(1);
    idealSpotSlider.addListener(this);
    addAndMakeVisible(idealSpotLabel);
    idealSpotLabel.setText("Ideal Distance from Speaker", dontSendNotification);
    
    idealSpotDis = idealSpotSlider.getValue();
    dbAtSpotDis = -6.257 * log(idealSpotDis) - 6.3795;
    
}

BalanceControls::~BalanceControls()
{
}

void BalanceControls::paint (Graphics& g)
{
    
}

void BalanceControls::resized()
{
    idealSpotSlider.setBounds(0, 20, 200, 20);
    idealSpotLabel.setBounds(0, 0, 200, 20);
}

float BalanceControls::workOutLisDisXAxis(int speaker, int xPos, int valueAtXPos)
{
    float disAcross;
    float disFromCenter;
    float disFromSpeaker;
    
    //Work out the maximum distance the kinect can see at the users depth
    float maxKinView;
    
    maxKinView = workOutLisDisVerticalToSpeakerLine(speaker, valueAtXPos) * atan(31.35 * M_PI / 180);
    
    //Times it by two since its only working out half the triangle
    maxKinView = maxKinView * 2;
    
    //Work out the listeners position across the kinects view as a percentage of this
    disAcross = (xPos/640.0) * maxKinView;
    
    //Works out how far from centre user is
    if(disAcross > maxKinView/2)
    {
        disFromCenter = disAcross - (maxKinView/2);
    }
    else if(disAcross < maxKinView/2)
    {
        disFromCenter = ((maxKinView/2) - disAcross) * -1;
    }
    
    if(speakerPositions[speaker].x > 0)
    {
        if(speakerPositions[speaker].x > disFromCenter)
        {
            disFromSpeaker = speakerPositions[speaker].x - disFromCenter;
        }
        else if(speakerPositions[speaker].x < disFromCenter)
        {
            disFromSpeaker = disFromCenter - speakerPositions[speaker].x;
        }
        else if(speakerPositions[speaker].x == disFromCenter)
        {
            disFromSpeaker = 0;
        }
    }
    
    else if(speakerPositions[speaker].x < 0)
    {
        if(speakerPositions[speaker].x > disFromCenter)
        {
            disFromSpeaker = disFromCenter - speakerPositions[speaker].x;
        }
        else if(speakerPositions[speaker].x < disFromCenter)
        {
            disFromSpeaker = speakerPositions[speaker].x - disFromCenter;
            
            if(disFromSpeaker < 0)
            {
                disFromSpeaker = disFromSpeaker * -1;
            }
        }
        else if(speakerPositions[speaker].x == disFromCenter)
        {
            disFromSpeaker = 0;
        }
    }
    
    else if(speakerPositions[speaker].x == 0)
    {
        disFromSpeaker = disFromCenter;
        
        if(disFromSpeaker < 0)
        {
            disFromSpeaker = disFromSpeaker * -1;
        }
    }
    
    DBG("Horizontal Dis = " << disFromSpeaker);
    return disFromSpeaker;
}

float BalanceControls::workOutLisDisZAxis(int speaker, int valueAtXPos)
{
    float vertDis;
    float lisZ;

    //works it out in meters
    //Depth is out of cameras range
    if(valueAtXPos == 2047)
    {
        //Returns 1m
        lisZ = 1;
    }
    else
    {
        //Converts kinect depth value to meters
        //Equation found at: http://graphics.stanford.edu/~mdfisher/Kinect.html
        lisZ = (1.0 / (valueAtXPos * -0.0030711016 + 3.3309495161));
    }
    
    if(lisZ > 3)
    {
        lisZ = lisZ - 0.25;
    }
    
    //DBG(lisZ);
    
    if(speakerPositions[speaker].z > lisZ)
    {
        vertDis = speakerPositions[speaker].z - lisZ;
        //DBG("1");
    }
    else if(speakerPositions[speaker].z < lisZ)
    {
        vertDis = lisZ - speakerPositions[speaker].z;
        //DBG("2");
    }
    else if(speakerPositions[speaker].z == lisZ)
    {
        vertDis = 0;
        //DBG("3");
    }
    
    //DBG("Vert Distance = " << vertDis);
    return vertDis;
}

float BalanceControls::workOutListenerDistance(int speaker, int xPos, int valueAtXPos)
{
    float overallDis;
    
    float speakerToHorizontalListenerPos = workOutLisDisXAxis(speaker, xPos, valueAtXPos);
    float speakerLineToVerticalListenerPos = workOutLisDisZAxis(speaker, valueAtXPos);
    
    overallDis = sqrt((pow(speakerToHorizontalListenerPos, 2) + pow(speakerLineToVerticalListenerPos, 2)));
    
    //To Overide this and set a distance:
    //overallDis = 3;
    
    //Limiter if distance is over 4m stick to 4m
    if(overallDis > 4)
    {
        overallDis = 4;
    }
       
    //DBG("Overall distance" << overallDis);
    return overallDis;
}

float BalanceControls::workOutMultiplier(int speaker, int xPos, int valueAtXPos)
{
    float currentMultiplier;
    
    //InverseSquareLaw
    /*float propAway = workOutListenerDistance(speaker, xPos, valueAtXPos)/idealSpotDis;
    currentMultiplier = pow(propAway, 2);*/
    
    //Bedroom
    float currentMultiplierDB = dbAtSpotDis - (-6.257 * log(workOutListenerDistance(speaker, xPos, valueAtXPos)) - 6.3795);
        
    currentMultiplier = pow(10, (currentMultiplierDB/20));
    
    if(speaker == 0)
    {
        leftGain = currentMultiplier;
    }
    else if(speaker == 1)
    {
        rightGain = currentMultiplier;
    }
    
    //DBG("Multiplier is: " << currentMultiplier);
    return currentMultiplier;
}


float BalanceControls::getListenerDistance(int channel, int xPos, int valueAtXPos)
{
    return workOutListenerDistance(channel, xPos, valueAtXPos);
}

float BalanceControls::getMultiplier(int channel, int xPos, int valueAtXpos)
{
    return workOutMultiplier(channel, xPos, valueAtXpos);
}

float BalanceControls::getLeftGain()
{
    return leftGain;
}

float BalanceControls::getRightGain()
{
    return rightGain;
}

void BalanceControls::sliderValueChanged(Slider* slider)
{
    if(slider == &idealSpotSlider)
    {
        idealSpotDis = idealSpotSlider.getValue();
        dbAtSpotDis = -6.257 * log(idealSpotDis) - 6.3795;
    }
}

void BalanceControls::setSpeakerCoOrdinates(int speakerNum, float xCoOrdinate, float zCoOrdinate)
{
    Coordinates tempCoordinate;
    tempCoordinate.x = xCoOrdinate;
    tempCoordinate.z = zCoOrdinate;
    
    speakerPositions.set(speakerNum, tempCoordinate);
}

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
    setSize(200, 350);
    
    speakerLineDis = 3.0;
    maxYDis = 4.0;
    
    addAndMakeVisible(lawSelection);
    lawSelection.setEditableText(false);
    lawSelection.addItem("Linear", 1);
    lawSelection.addItem("Squared", 2);
    lawSelection.addItem("Cubed", 3);
    lawSelection.setSelectedId(1);
    
    addAndMakeVisible(comboBoxLabel);
    comboBoxLabel.setText("Selected Balance Law", dontSendNotification);
}

BalanceControls::~BalanceControls()
{
}

void BalanceControls::paint (Graphics& g)
{
    
}

void BalanceControls::resized()
{
    listenerXPosSlider.setBounds(0, 275, 200, 40);
    listenerYPosSlider.setBounds(100, 70, 100, 200);
    comboBoxLabel.setBounds(0, 0, 200, 25);
    lawSelection.setBounds(0, 30, 200, 30);
}

float BalanceControls::workOutLisDisHorizontalToSpeakers(int speaker, int xPos, int valueAtXPos)
{
    float xDis;
    
    //Work out the maximum distance the kinect can see at the users depth
    float maxKinView;
    maxKinView = (workOutLisDisVerticalToSpeakerLine(valueAtXPos) * 2) / (tan(61.5));
    
    //Work out the listeners position across the kinects view as a percentage of this
    if(speaker == 0)
    {
        xDis = (xPos/640.0) * maxKinView;
    }
    else if(speaker == 1)
    {
        xDis = (((xPos/640.0) * maxKinView) - maxKinView) * -1;
    }
    
    //If the kinect isnt viewing the entire speaker line this adds the extra on
    if(speakerLineDis > maxKinView)
    {
        xDis = xDis + (speakerLineDis - maxKinView);
    }
    
    //DBG("Horizontal Dis = " << xDis);
    return xDis;
}

float BalanceControls::workOutLisDisVerticalToSpeakerLine(int valueAtXPos)
{
    float vertDis;

    //works it out in meters
    //Depth is out of cameras range
    if(valueAtXPos == 2047)
    {
        //Returns 1m
        vertDis = 1;
    }
    else
    {
        //Converts kinect depth value to meters
        //Equation found at: http://graphics.stanford.edu/~mdfisher/Kinect.html
        vertDis = (1.0 / (valueAtXPos * -0.0030711016 + 3.3309495161));
    }
    
    //DBG("Vert Distance = " << vertDis);
    return vertDis;
}

float BalanceControls::workOutListenerDistance(int speaker, int xPos, int valueAtXPos)
{
    float overallDis;
    
    float speakerToHorizontalListenerPos = workOutLisDisHorizontalToSpeakers(speaker, xPos, valueAtXPos);
    float speakerLineToVerticalListenerPos = workOutLisDisVerticalToSpeakerLine(valueAtXPos);
    
    overallDis = sqrt((pow(speakerToHorizontalListenerPos, 2) + pow(speakerLineToVerticalListenerPos, 2)));
    
    return overallDis;
}

float BalanceControls::workOutMultiplier(int speaker, int xPos, int valueAtXPos)
{
    if(lawSelection.getSelectedId() == 1)
    {
       return workOutListenerDistance(speaker, xPos, valueAtXPos)/2.23;
    }
    else if(lawSelection.getSelectedId() == 2)
    {
         return pow(workOutListenerDistance(speaker, xPos, valueAtXPos)/2.23, 2);
    }
    else if(lawSelection.getSelectedId() == 3)
    {
         return pow(workOutListenerDistance(speaker, xPos, valueAtXPos)/2.23, 3);
    }
    //DBG("Multiplier is: " << currentMultiplier);
    
}


float BalanceControls::getListenerDistance(int channel, int xPos, int valueAtXPos)
{
    return workOutMultiplier(channel, xPos, valueAtXPos);
}

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

float BalanceControls::workOutLisDisHorizontalToSpeakers(int speaker, int xPos)
{
    float xDis;
    
    //Works out how far across the picture and then maps that between 0 and 3 for each speaker
    if(speaker == 0)
    {
        xDis = (xPos/640.0) * speakerLineDis;
        //DBG("User xPos on speaker 0 =" << xDis);
    }
    else if(speaker == 1)
    {
        xDis = (((xPos/640.0) * speakerLineDis)- speakerLineDis) * -1.0;
        //DBG("User xPos on speaker 1 = " << xDis);
    }
    
    return xDis;
}

float BalanceControls::workOutLisDisVerticalToSpeakerLine(int valueAtXPos)
{
    float vertDis;
    
    //Depth is out of cameras range
    if(valueAtXPos == 2047)
    {
        //returns 1
        vertDis = 1;
    }
    
    else
    {
        //Maps depth down to between 0 and maximum y dis
        vertDis = (valueAtXPos/2046) * maxYDis;
    }
    
    return vertDis;
}

float BalanceControls::workOutListenerDistance(int speaker, int xPos, int valueAtXPos)
{
    float speakerToHorizontalListenerPos = workOutLisDisHorizontalToSpeakers(speaker, xPos);
    float speakerLineToVerticalListenerPos = workOutLisDisVerticalToSpeakerLine(valueAtXPos);
    
    return sqrt((pow(speakerToHorizontalListenerPos, 2) + pow(speakerLineToVerticalListenerPos, 2)));
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

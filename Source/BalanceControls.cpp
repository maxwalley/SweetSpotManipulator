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
    
    maxYDis = 4.0;
    
    addAndMakeVisible(lawSelection);
    lawSelection.setEditableText(false);
    lawSelection.addItem("Linear", 1);
    lawSelection.addItem("-3dB", 2);
    lawSelection.addItem("Experimental", 3);
    lawSelection.addItem("Logarithmic", 4);
    lawSelection.addItem("Inverse Square Law", 5);
    lawSelection.setSelectedId(1);
    lawSelection.addListener(this);
    
    addAndMakeVisible(comboBoxLabel);
    comboBoxLabel.setText("Selected Balance Law", dontSendNotification);
    
    addAndMakeVisible(idealSpotSlider);
    idealSpotSlider.setVisible(false);
    idealSpotSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    idealSpotSlider.setRange(1, 4);
    idealSpotSlider.setValue(2);
    addAndMakeVisible(idealSpotLabel);
    idealSpotLabel.setVisible(false);
    idealSpotLabel.setText("Ideal Distance from Speaker", dontSendNotification);
}

BalanceControls::~BalanceControls()
{
}

void BalanceControls::paint (Graphics& g)
{
    
}

void BalanceControls::resized()
{
    comboBoxLabel.setBounds(0, 0, 200, 25);
    lawSelection.setBounds(0, 30, 200, 30);
    idealSpotSlider.setBounds(0, 90, 200, 20);
    idealSpotLabel.setBounds(0, 70, 200, 20);
}

float BalanceControls::workOutLisDisHorizontalToSpeakers(int speaker, int xPos, int valueAtXPos)
{
    float xDis;
    
    //Work out the maximum distance the kinect can see at the users depth
    float maxKinView;
    //maxKinView = (workOutLisDisVerticalToSpeakerLine(valueAtXPos) * 2) / (tan(61.5));
    
    maxKinView = workOutLisDisVerticalToSpeakerLine(valueAtXPos) * atan(30.75 * M_PI / 180);
    
    maxKinView = maxKinView * 2;
    
    //Look this up
    //maxKinView = tan(30.75 * M_PI / 180);
    
    //Work out the listeners position across the kinects view as a percentage of this
    if(speaker == 1)
    {
        xDis = (xPos/640.0) * maxKinView;
    }
    else if(speaker == 0)
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
    
    //Limiter if distance is over 4m stick to 4m
    if(overallDis > 4)
    {
        overallDis = 4;
    }
    
    return overallDis;
}

float BalanceControls::workOutMultiplier(int speaker, int xPos, int valueAtXPos)
{
    float currentMultiplier;
    
    //Linear
    if(lawSelection.getSelectedId() == 1)
    {
       currentMultiplier = workOutListenerDistance(speaker, xPos, valueAtXPos)/idealSpotSlider.getValue();
    }
    
    //-3dB
    else if(lawSelection.getSelectedId() == 2)
    {
        currentMultiplier = sin((workOutListenerDistance(speaker, xPos, valueAtXPos)/idealSpotSlider.getValue()) * 0.5 * M_PI);
    }
    
    //Experimental
    else if(lawSelection.getSelectedId() == 3)
    {
        float dis = workOutListenerDistance(speaker, xPos, valueAtXPos);
        
        if(dis > 2 || dis < 1)
        {
            dis = 0;
        }
        else
        {
            dis = dis-1;
        }
        
        currentMultiplier = dis;
    }
    
    //Logarithmic linear
    else if(lawSelection.getSelectedId() == 4)
    {
        currentMultiplier = log10(workOutListenerDistance(speaker, xPos, valueAtXPos)/4);
    }
    
    //Inverse Square Law
    else if(lawSelection.getSelectedId() == 5)
    {
        //Assumes speakers are angled at 30 degrees and works out dis to ideal sweet spot
        //float idealSpotDis = (speakerLineDis/2)/0.866;
        
        //Takes how far the user wants to simulate the speakers away from the slider
        float idealSpotDis = idealSpotSlider.getValue();
        
        float proportionAway = workOutListenerDistance(speaker, xPos, valueAtXPos)/idealSpotDis;
        
        //Inverse Square Law
        currentMultiplier = pow(proportionAway, 2);
    }
    
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
    return workOutMultiplier(channel, xPos, valueAtXPos);
}

void BalanceControls::setSpeakerLineDistance(float newDistance)
{
    speakerLineDis = newDistance;
}

float BalanceControls::getLeftGain()
{
    return leftGain;
}

float BalanceControls::getRightGain()
{
    return rightGain;
}

void BalanceControls::comboBoxChanged(ComboBox* comboBoxThatHasChanged)
{
    if(comboBoxThatHasChanged->getSelectedId() == 5)
    {
        idealSpotSlider.setVisible(true);
        idealSpotLabel.setVisible(true);
    }
    else
    {
        idealSpotSlider.setVisible(false);
        idealSpotLabel.setVisible(false);
    }
    repaint();
}

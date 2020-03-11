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
    setSize(200, 110);
    
    maxYDis = 4.0;
    
    addAndMakeVisible(lawSelection);
    lawSelection.setEditableText(false);
    lawSelection.addItem("Linear", 1);
    lawSelection.addItem("-3dB", 2);
    lawSelection.addItem("Experimental", 3);
    lawSelection.addItem("Logarithmic", 4);
    lawSelection.addItem("1N50B", 5);
    lawSelection.addItem("Inverse Square Law", 6);
    lawSelection.setSelectedId(1);
    lawSelection.addListener(this);
    
    addAndMakeVisible(comboBoxLabel);
    comboBoxLabel.setText("Selected Balance Law", dontSendNotification);
    
    addAndMakeVisible(idealSpotSlider);
    idealSpotSlider.setVisible(false);
    idealSpotSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    idealSpotSlider.setRange(1, 4);
    idealSpotSlider.setValue(2);
    idealSpotSlider.addListener(this);
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
    
    //DBG("Horizontal Dis = " << disFromSpeaker);
    return disFromSpeaker;
}

float BalanceControls::workOutLisDisVerticalToSpeakerLine(int speaker, int valueAtXPos)
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
    
    float speakerToHorizontalListenerPos = workOutLisDisHorizontalToSpeakers(speaker, xPos, valueAtXPos);
    float speakerLineToVerticalListenerPos = workOutLisDisVerticalToSpeakerLine(speaker, valueAtXPos);
    
    overallDis = sqrt((pow(speakerToHorizontalListenerPos, 2) + pow(speakerLineToVerticalListenerPos, 2)));
    
    //To Overide this and set a distance:
    //overallDis = 2.5 + comment out everything above
    
    //Limiter if distance is over 4m stick to 4m
    if(overallDis > 4)
    {
        overallDis = 4;
    }
       
    //DBG(overallDis);
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
    
    //1n50B
    else if(lawSelection.getSelectedId() == 5)
    {
        float dbAtSpotDis = -5.342 * log(idealSpotDis) - 4.2527;
        
        float currentMultiplierDB = dbAtSpotDis - (-5.342 * log(workOutMultiplier(speaker, xPos, valueAtXPos)) - 4.2527);
        
        currentMultiplier = pow(10, (currentMultiplierDB/20));
    }
    
    //Inverse Square Law
    else if(lawSelection.getSelectedId() == 6)
    {
        float proportionAway = workOutListenerDistance(speaker, xPos, valueAtXPos)/idealSpotDis;
        
        //Inverse Inverse Square Law
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

void BalanceControls::sliderValueChanged(Slider* slider)
{
    if(slider == &idealSpotSlider)
    {
        idealSpotDis = idealSpotSlider.getValue();
    }
}

void BalanceControls::setSpeakerCoOrdinates(int speakerNum, float xCoOrdinate, float zCoOrdinate)
{
    Coordinates tempCoordinate;
    tempCoordinate.x = xCoOrdinate;
    tempCoordinate.z = zCoOrdinate;
    
    speakerPositions.set(speakerNum, tempCoordinate);
}

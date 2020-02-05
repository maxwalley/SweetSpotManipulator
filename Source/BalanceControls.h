/*
  ==============================================================================

    BalanceControls.h
    Created: 31 Oct 2019 10:43:20am
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class BalanceControls    : public Component,
                           public ComboBox::Listener
{
public:
    BalanceControls();
    ~BalanceControls();

    void paint (Graphics&) override;
    void resized() override;
    
    /*Returns a multiplier to factor in distance
    speaker argument determines which speaker this distance is from
    0 for left, 1 for right*/
    float workOutMultiplier(int speaker, int xPos, int valueAtXPos);
    
    float getListenerDistance(int channel, int xPos, int valueAtXPos);
    
    void setSpeakerLineDistance(float newDistance);
    
    float getLeftGain();
    float getRightGain();
    
private:
    
    //Taken from left speaker to right speaker
    float speakerLineDis;
    
    float maxYDis;
    
    float currentDepthAtXPos;
    
    Slider idealSpotSlider;
    ComboBox lawSelection;
    Label comboBoxLabel;
    
    /*Returns listener distance along speaker line from equivilent user position
     speaker argument determines which speaker this distance is from
     0 for left, 1 for right*/
    float workOutLisDisHorizontalToSpeakers(int speaker, int xPos, int valueAtXPos);
    
    /*Returns listener distance to the speaker line*/
    float workOutLisDisVerticalToSpeakerLine(int valueAtXPos);
    
    /*Returns distance from speaker to user
     speaker argument determines which speaker this distance is from
     0 for left, 1 for right*/
    float workOutListenerDistance(int speaker, int xPos, int valueAtXPos);
    
    float leftGain;
    float rightGain;
    
    void comboBoxChanged(ComboBox* comboBoxThatHasChanged) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BalanceControls)
};

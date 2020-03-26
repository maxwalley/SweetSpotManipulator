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
                           public Slider::Listener
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
    
    float getMultiplier(int channel, int xPos, int valueAtXpos);
    
    void setSpeakerLineDistance(float newDistance);
    
    float getLeftGain();
    float getRightGain();
    
    /**Sets the speaker co-ordinates
     @param speakerNum = the speaker number to change the co-ordinates of
     @param xCoOrdnate = the speakers position on the x axis
     @param zCoOrdinate = the speakers position on the y axis
     */
    void setSpeakerCoOrdinates(int speakerNum, float xCoOrdinate, float zCoOrdinate);
    
    
private:
    
    //Taken from left speaker to right speaker
    float speakerLineDis;
    
    float maxYDis;
    
    float currentDepthAtXPos;
    
    Slider idealSpotSlider;
    Label idealSpotLabel;
    
    /*Returns listener distance along speaker line from equivilent user position
     speaker argument determines which speaker this distance is from
     0 for left, 1 for right*/
    float workOutLisDisHorizontalToSpeakers(int speaker, int xPos, int valueAtXPos);
    
    /*Returns listener distance to the speaker line*/
    float workOutLisDisVerticalToSpeakerLine(int speaker, int valueAtXPos);
    
    struct Coordinates
    {
        float x;
        float z;
    };
    
    Array<Coordinates> speakerPositions;
    
    /*Returns distance from speaker to user
     speaker argument determines which speaker this distance is from
     0 for left, 1 for right*/
    float workOutListenerDistance(int speaker, int xPos, int valueAtXPos);
    
    float leftGain;
    float rightGain;
    
    float idealSpotDis;
    float dbAtSpotDis;
    
    /**from the Slider::Listener Juce class*/
    void sliderValueChanged(Slider* slider) override;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BalanceControls)
};

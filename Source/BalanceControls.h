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
class BalanceControls    : public Component
{
public:
    BalanceControls();
    ~BalanceControls();

    void paint (Graphics&) override;
    void resized() override;
    
    /*Returns a multiplier to factor in distance
    speaker argument determines which speaker this distance is from
    0 for left, 1 for right*/
    float workOutMultiplier(bool speaker);
private:
    
    //Taken from left speaker to right speaker
    float speakerLineDis;
    
    float lpXLineLength;
    float lpYLineLength;
    float lpDisFromSpeakerLine;
    
    Slider listenerXPosSlider;
    Slider listenerYPosSlider;
    ComboBox lawSelection;
    Label comboBoxLabel;
    
    /*Returns listener distance along speaker line from equivilent user position
     speaker argument determines which speaker this distance is from
     0 for left, 1 for right*/
    float workOutLisDisHorizontalToSpeakers(bool speaker);
    
    /*Returns listener distance to the speaker line*/
    float workOutLisDisVerticalToSpeakerLine();
    
    /*Returns distance from speaker to user
     speaker argument determines which speaker this distance is from
     0 for left, 1 for right*/
    float workOutListenerDistance(bool speaker);
    
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BalanceControls)
};

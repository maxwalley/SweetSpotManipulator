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
    
    speakerLineDis = 2.0;
    lpXLineLength = 3.0;
    lpYLineLength = 4.0;
    lpDisFromSpeakerLine = 2.0;
    
    addAndMakeVisible(listenerXPosSlider);
    listenerXPosSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    listenerXPosSlider.setRange((lpXLineLength/2) * -1, lpXLineLength/2);
    listenerXPosSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 25);
    
    addAndMakeVisible(listenerYPosSlider);
    listenerYPosSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    listenerYPosSlider.setRange((lpYLineLength/2) * -1, lpYLineLength/2);
    listenerYPosSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxLeft, true, 100, 25);
    
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

float BalanceControls::workOutLisDisHorizontalToSpeakers(bool speaker)
{
    bool isMinus;
    float currentVal;
    
    if(listenerXPosSlider.getValue() < 0)
    {
        currentVal = listenerXPosSlider.getValue() * -1;
        isMinus = true;
    }
    else if (listenerXPosSlider.getValue() > 0)
    {
        currentVal = listenerXPosSlider.getValue();
        isMinus = false;
    }
    
    if(currentVal < speakerLineDis/2)
    {
        
        if((speaker == 0 && isMinus == true) || (speaker == 1 && isMinus == false))
        {
            return speakerLineDis/2 - currentVal;
        }
        else if((speaker == 0 && isMinus == false) || (speaker == 1 & isMinus == true))
        {
            return speakerLineDis/2 + currentVal;
        }
        
    }
    else if (currentVal > speakerLineDis/2)
    {
        if((speaker == 0 && isMinus == true) || (speaker == 1 && isMinus == false))
        {
            return currentVal - speakerLineDis/2;
        }
        else if ((speaker == 0 && isMinus == false) || (speaker == 1 && isMinus == true))
        {
            return (currentVal - speakerLineDis/2) + speakerLineDis;
        }
    }
}

float BalanceControls::workOutLisDisVerticalToSpeakerLine()
{
    if(listenerYPosSlider.getValue() > 0)
    {
        return (lpYLineLength/2) - listenerYPosSlider.getValue();
    }
    else if(listenerYPosSlider.getValue() <= 0)
    {
        return (listenerYPosSlider.getValue() * -1) + (lpYLineLength/2);
    }
}

float BalanceControls::workOutListenerDistance(bool speaker)
{
    float speakerToHorizontalListenerPos = workOutLisDisHorizontalToSpeakers(speaker);
    float speakerLineToVerticalListenerPos = workOutLisDisVerticalToSpeakerLine();
    
    return sqrt((pow(speakerToHorizontalListenerPos, 2) + pow(speakerLineToVerticalListenerPos, 2)));
}

float BalanceControls::workOutMultiplier(bool speaker)
{
    if(lawSelection.getSelectedId() == 1)
    {
       return workOutListenerDistance(speaker)/2.23;
    }
    else if(lawSelection.getSelectedId() == 2)
    {
        return pow(workOutListenerDistance(speaker)/2.23, 2);
    }
    else if(lawSelection.getSelectedId() == 3)
    {
        return pow(workOutListenerDistance(speaker)/2.23, 3);
    }
}

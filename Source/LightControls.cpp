/*
  ==============================================================================

    LightButtonArray.cpp
    Created: 10 Sep 2019 4:36:57pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LightControls.h"

//==============================================================================
LightControls::LightControls()
{
    setSize(200, 55);
    
    addAndMakeVisible(comboBoxLabel);
    comboBoxLabel.setText("Kinect LED Colour", dontSendNotification);
    
    addAndMakeVisible(lightSelectionBox);
    lightSelectionBox.setEditableText(false);
    lightSelectionBox.addItem("Off", 1);
    lightSelectionBox.addItem("Green", 2);
    lightSelectionBox.addItem("Red", 3);
    lightSelectionBox.addItem("Yellow", 4);
    lightSelectionBox.setSelectedId(1);
}

LightControls::~LightControls()
{
}

void LightControls::paint (Graphics& g)
{

}

void LightControls::resized()
{
    comboBoxLabel.setBounds(0, 0, 200, 25);
    lightSelectionBox.setBounds(0, 25, 200, 30);
}

freenect_led_options LightControls::getSelectedLed()
{
    if(lightSelectionBox.getSelectedIdAsValue() == 1)
    {
        selectedLed = LED_OFF;
        return selectedLed;
    }
    else if(lightSelectionBox.getSelectedIdAsValue() == 2)
    {
        selectedLed = LED_GREEN;
        return selectedLed;
    }
    else if(lightSelectionBox.getSelectedIdAsValue() == 3)
    {
        selectedLed = LED_RED;
        return selectedLed;
    }
    else if(lightSelectionBox.getSelectedIdAsValue() == 4)
    {
        selectedLed = LED_YELLOW;
        return selectedLed;
    }
}

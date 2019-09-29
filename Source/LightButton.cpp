/*
  ==============================================================================

    LightButton.cpp
    Created: 10 Sep 2019 4:30:36pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LightButton.h"

//==============================================================================
LightButton::LightButton()
{
}

LightButton::~LightButton()
{
}

void LightButton::init(String name)
{
    setButtonText(name);
    setToggleState(false, dontSendNotification);
    setClickingTogglesState(true);
}

void LightButton::paintInit()
{
    setColour(TextButton::ColourIds::buttonOnColourId, Colours::royalblue);
}

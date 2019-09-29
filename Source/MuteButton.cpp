/*
  ==============================================================================

    MuteButton.cpp
    Created: 12 May 2019 6:49:52pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MuteButton.h"

//==============================================================================
MuteButton::MuteButton()
{
}

MuteButton::~MuteButton()
{
}

void MuteButton::Initialize()
{
    setButtonText("Mute");
    setToggleState(false, dontSendNotification);
    setClickingTogglesState(true);
}

void MuteButton::paintInit()
{
    setColour(TextButton::ColourIds::buttonOnColourId, Colours::royalblue);
}

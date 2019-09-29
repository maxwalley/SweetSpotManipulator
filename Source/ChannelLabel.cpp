/*
  ==============================================================================

    ChannelLabel.cpp
    Created: 13 May 2019 5:11:48pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ChannelLabel.h"

//==============================================================================
ChannelLabel::ChannelLabel()
{
}

ChannelLabel::~ChannelLabel()
{
}

void ChannelLabel::Initialize(String ChannelName)
{
    setText(ChannelName, dontSendNotification);
    setJustificationType(Justification::horizontallyCentred);
}

void ChannelLabel::paintInit()
{
    setColour(Label::ColourIds::textColourId, Colours::black);
}

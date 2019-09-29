/*
  ==============================================================================

    ChannelArray.h
    Created: 13 May 2019 7:17:47pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Channel.h"

//==============================================================================
/*
*/
class ChannelArray    : public Component
{
public:
    ChannelArray();
    ~ChannelArray();

    void paint (Graphics&) override;
    void resized() override;
    
    Channel Center;
    Channel LeftFront;
    Channel RightFront;
    Channel LeftRear;
    Channel RightRear;
    Channel LFE;
    Channel Master;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelArray);
};

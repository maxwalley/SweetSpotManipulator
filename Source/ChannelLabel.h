/*
  ==============================================================================

    ChannelLabel.h
    Created: 13 May 2019 5:11:48pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ChannelLabel    : public Label
{
public:
    ChannelLabel();
    ~ChannelLabel();
    
    void Initialize(String ChannelName);
    void paintInit();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelLabel)
};

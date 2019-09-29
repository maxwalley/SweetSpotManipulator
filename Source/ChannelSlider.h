/*
  ==============================================================================

    ChannelSlider.h
    Created: 13 May 2019 5:21:44pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class ChannelSlider    : public Slider
{
public:
    ChannelSlider();
    ~ChannelSlider();
    
    void Initialize();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChannelSlider)
};

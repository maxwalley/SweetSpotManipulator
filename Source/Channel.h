/*
  ==============================================================================

    Channel.h
    Created: 13 May 2019 5:27:03pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "MuteButton.h"
#include "ChannelLabel.h"
#include "ChannelSlider.h"

//==============================================================================
/*
*/
class Channel    : public Component
{
public:
    Channel(String CHName);
    ~Channel();
    
    void paint (Graphics& g) override;
    void resized() override;
    
    ChannelLabel CHLabel;
    MuteButton Mute;
    ChannelSlider CHSlider;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Channel);
};

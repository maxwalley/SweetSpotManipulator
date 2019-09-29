/*
  ==============================================================================

    ChannelSlider.cpp
    Created: 13 May 2019 5:21:44pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ChannelSlider.h"

//==============================================================================
ChannelSlider::ChannelSlider()
{
}

ChannelSlider::~ChannelSlider()
{
}

void ChannelSlider::Initialize()
{
    setSliderStyle(Slider::SliderStyle::LinearVertical);
    setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
    setRange(0, 1);
}

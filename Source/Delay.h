/*
  ==============================================================================

    Delay.h
    Created: 6 Nov 2019 5:17:07pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class Delay    : public Component
{
public:
    Delay();
    ~Delay();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Delay)
};

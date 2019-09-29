/*
  ==============================================================================

    LightButton.h
    Created: 10 Sep 2019 4:30:36pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class LightButton    : public TextButton
{
public:
    LightButton();
    ~LightButton();

    void init(String name);
    void paintInit();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LightButton)
};

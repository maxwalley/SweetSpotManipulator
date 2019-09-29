/*
  ==============================================================================

    LightButtonArray.h
    Created: 10 Sep 2019 4:36:57pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LightButton.h"
#include "libfreenect.h"

//==============================================================================
/*
*/
class LightButtonArray    : public Component,
                            public Button::Listener
{
public:
    LightButtonArray();
    ~LightButtonArray();

    void paint (Graphics&) override;
    void resized() override;
    
    void buttonClicked(Button* button) override;
    freenect_led_options selectedLed;
    
    LightButton off;
    LightButton red;
    LightButton green;
    LightButton yellow;
    LightButton blinking;
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LightButtonArray)
};

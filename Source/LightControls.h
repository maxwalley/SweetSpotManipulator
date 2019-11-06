/*
  ==============================================================================

    LightButtonArray.h
    Created: 10 Sep 2019 4:36:57pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "libfreenect.h"

//==============================================================================
/*
*/
class LightControls    : public Component
{
public:
    LightControls();
    ~LightControls();

    void paint (Graphics&) override;
    void resized() override;
    
    freenect_led_options getSelectedLed();
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LightControls)
    
    Label comboBoxLabel;
    ComboBox lightSelectionBox;
    
    freenect_led_options selectedLed;
};

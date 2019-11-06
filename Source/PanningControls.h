/*
  ==============================================================================

    PanningButtonArray.h
    Created: 24 Oct 2019 8:19:28pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class PanningControls    : public Component
{
public:
    PanningControls();
    ~PanningControls();

    void paint (Graphics&) override;
    void resized() override;
    
    int getPanningLaw();
    float getPanSliderVal();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PanningControls)
    
    ComboBox panSelection;
    Slider panSlider;
    Label comboBoxLabel;
};

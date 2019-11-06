/*
  ==============================================================================

    PanningButtonArray.cpp
    Created: 24 Oct 2019 8:19:28pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PanningControls.h"

//==============================================================================
PanningControls::PanningControls()
{
    setSize(200, 100);
    
    addAndMakeVisible(panSelection);
    panSelection.setEditableText(false);
    panSelection.addItem("-3dB", 1);
    panSelection.addItem("-6dB", 2);
    panSelection.setSelectedId(1);
    
    addAndMakeVisible(panSlider);
    panSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    panSlider.setRange(0, 1);
    panSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 100, 20);
    
    addAndMakeVisible(comboBoxLabel);
    comboBoxLabel.setText("Selected Panning Law", dontSendNotification);
}

PanningControls::~PanningControls()
{
}

void PanningControls::paint (Graphics& g)
{

}

void PanningControls::resized()
{
    comboBoxLabel.setBounds(0, 0, 200, 25);
    panSelection.setBounds(0, 30, 200, 30);
    panSlider.setBounds(0, 70, 200, 30);
}


int PanningControls::getPanningLaw()
{
    return panSelection.getSelectedItemIndex();
}

float PanningControls::getPanSliderVal()
{
    return panSlider.getValue();
}

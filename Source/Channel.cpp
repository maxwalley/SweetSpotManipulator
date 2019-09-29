/*
  ==============================================================================

    Channel.cpp
    Created: 13 May 2019 5:27:03pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Channel.h"

//==============================================================================
Channel::Channel(String CHName)
{
    setSize(100, 340);
    addAndMakeVisible(CHLabel);
    addAndMakeVisible(Mute);
    addAndMakeVisible(CHSlider);
    
    CHLabel.Initialize(CHName);
    Mute.Initialize();
    CHSlider.Initialize();
    
}

Channel::~Channel()
{
}

void Channel::paint(Graphics& g)
{
    Mute.paintInit();
    CHLabel.paintInit();
}

void Channel::resized()
{
    CHLabel.setBounds(0, 20, 100, 20);
    Mute.setBounds(30, 120, 40, 20);
    CHSlider.setBounds(25, 170, 50, 150);
}

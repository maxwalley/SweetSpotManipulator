/*
  ==============================================================================

    LightButtonArray.cpp
    Created: 10 Sep 2019 4:36:57pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "LightButtonArray.h"

//==============================================================================
LightButtonArray::LightButtonArray()
{
    setSize(250, 60);
    addAndMakeVisible(off);
    addAndMakeVisible(red);
    addAndMakeVisible(green);
    addAndMakeVisible(yellow);
    addAndMakeVisible(blinking);
    
    off.init("Off");
    red.init("Red");
    green.init("Green");
    yellow.init("Yellow");
    blinking.init("Blink");
    
    off.addListener(this);
    red.addListener(this);
    green.addListener(this);
    yellow.addListener(this);
    blinking.addListener(this);
}

LightButtonArray::~LightButtonArray()
{
}

void LightButtonArray::paint (Graphics& g)
{
    off.paintInit();
    red.paintInit();
    green.paintInit();
    yellow.paintInit();
    blinking.paintInit();
    
    g.drawLine(5, 2, 245, 2);
    g.drawLine(5, 2, 5, 58);
    g.drawLine(5, 58, 245, 58);
    g.drawLine(245, 2, 245, 58);
}

void LightButtonArray::resized()
{
    off.setBounds(10, 10, 50, 20);
    red.setBounds(70, 10, 50, 20);
    green.setBounds(130, 10, 50, 20);
    yellow.setBounds(190, 10, 50, 20);
    blinking.setBounds(160, 35, 50, 20);
}

void LightButtonArray::buttonClicked(Button* button)
{
    if(button == &off)
    {
        if(off.getToggleState() == true)
        {
            red.setToggleState(false, dontSendNotification);
            green.setToggleState(false, dontSendNotification);
            yellow.setToggleState(false, dontSendNotification);
            blinking.setToggleState(false, dontSendNotification);
            selectedLed = LED_OFF;
        }
    }
    else if(button == &red)
    {
        if(red.getToggleState() == true)
        {
            off.setToggleState(false, dontSendNotification);
            green.setToggleState(false, dontSendNotification);
            yellow.setToggleState(false, dontSendNotification);
            blinking.setToggleState(false, dontSendNotification);
            selectedLed = LED_RED;
        }
    }
    else if(button == &green)
    {
        if(green.getToggleState() == true && blinking.getToggleState() == true)
        {
            off.setToggleState(false, dontSendNotification);
            red.setToggleState(false, dontSendNotification);
            yellow.setToggleState(false, dontSendNotification);
            selectedLed = LED_BLINK_GREEN;
        }
        else if(green.getToggleState() == true && blinking.getToggleState() == false)
        {
            off.setToggleState(false, dontSendNotification);
            red.setToggleState(false, dontSendNotification);
            yellow.setToggleState(false, dontSendNotification);
            selectedLed = LED_GREEN;
        }
    }
    else if(button == &yellow)
    {
        if(yellow.getToggleState() == true && blinking.getToggleState() == true)
        {
            off.setToggleState(false, dontSendNotification);
            red.setToggleState(false, dontSendNotification);
            green.setToggleState(false, dontSendNotification);
            selectedLed = LED_BLINK_RED_YELLOW;
        }
        else if (yellow.getToggleState() == true && blinking.getToggleState() == false)
        {
            off.setToggleState(false, dontSendNotification);
            red.setToggleState(false, dontSendNotification);
            green.setToggleState(false, dontSendNotification);
            selectedLed = LED_YELLOW;
        }
    }
    else if (button == &blinking)
    {
        if(blinking.getToggleState() == true && yellow.getToggleState() == true)
        {
            off.setToggleState(false, dontSendNotification);
            red.setToggleState(false, dontSendNotification);
            green.setToggleState(false, dontSendNotification);
            selectedLed = LED_BLINK_RED_YELLOW;
        }
        else if(blinking.getToggleState() == true && green.getToggleState() == true)
        {
            off.setToggleState(false, dontSendNotification);
            red.setToggleState(false, dontSendNotification);
            yellow.setToggleState(false, dontSendNotification);
            selectedLed = LED_BLINK_GREEN;
        }
        else if(blinking.getToggleState() == true && green.getToggleState() == false && yellow.getToggleState() == false)
        {
            off.setToggleState(false, dontSendNotification);
            red.setToggleState(false, dontSendNotification);
        }
    }
    else
    {
        
    }
}

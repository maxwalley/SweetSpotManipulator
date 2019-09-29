/*
  ==============================================================================

    ChannelArray.cpp
    Created: 13 May 2019 7:17:47pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "ChannelArray.h"

//==============================================================================
ChannelArray::ChannelArray() : Center("Center"), LeftFront("Left Front"), RightFront("Right Front"), LeftRear("Left Rear"),
RightRear("Right Rear"), LFE("LFE"), Master("Master")
{
    setSize(800, 340);
    addAndMakeVisible(Center);
    addAndMakeVisible(LeftFront);
    addAndMakeVisible(RightFront);
    addAndMakeVisible(LeftRear);
    addAndMakeVisible(RightRear);
    addAndMakeVisible(LFE);
    addAndMakeVisible(Master);
}

ChannelArray::~ChannelArray()
{
}

void ChannelArray::paint (Graphics& g)
{
    g.drawLine(25, 0, 25, 340);
    g.drawLine(125, 0, 125, 340);
    g.drawLine(225, 0, 225, 340);
    g.drawLine(325, 0, 325, 340);
    g.drawLine(425, 0, 425, 340);
    g.drawLine(525, 0, 525, 340);
    g.drawLine(625, 0, 625, 340);
    g.drawLine(675, 0, 675, 340);
    g.drawLine(775, 0, 775, 340);
    
    g.drawLine(25, 150, 625, 150);
    g.drawLine(675, 150, 775, 150);
    
    g.drawLine(25, 110, 625, 110);
    g.drawLine(675, 110, 775, 110);
}

void ChannelArray::resized()
{
    Center.setBounds(30, 0, 100, 340);
    LeftFront.setBounds(130, 0, 100, 340);
    RightFront.setBounds(230, 0, 100, 340);
    LeftRear.setBounds(330, 0, 100, 340);
    RightRear.setBounds(430, 0, 100, 340);
    LFE.setBounds(530, 0, 100, 340);
    Master.setBounds(670, 0, 100, 340);
}

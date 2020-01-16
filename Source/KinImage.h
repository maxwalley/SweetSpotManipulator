/*
  ==============================================================================

    KinImage.h
    Created: 17 Oct 2019 3:46:21pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Kinect.h"
#include "opencv2/opencv.hpp"

//==============================================================================
/*
*/
class KinImage    : public AnimatedAppComponent
{
public:
    KinImage();
    ~KinImage();

    void paint(Graphics &g) override;
    
    void kinectInit();
    void kinectEnd() const;
    
    bool active() const;
    
    Kinect kinect;

private:
    void update() override;
    
    bool kinectErrorCodeTriggered;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KinImage)
};

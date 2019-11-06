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

//==============================================================================
/*
*/
class KinImage    : public AnimatedAppComponent
{
public:
    KinImage();
    ~KinImage();

    void paint (Graphics&) override;
    void resized() override;

    void setImage(int row, int col, uint16_t pixel);
    void repaintImage();
    void kinectInit();
    void kinectEnd() const;
    
    bool active() const;
    
private:
    Image kinPic;
    void update() override;
    void setDepthPixels();
    
    Kinect kinect;
    bool kinectErrorCodeTriggered;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KinImage)
};

/*
  ==============================================================================

    MuteButton.h
    Created: 12 May 2019 6:49:52pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class MuteButton    : public TextButton
{
public:
    MuteButton();
    ~MuteButton();
    
    void Initialize();
    void paintInit();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MuteButton);
};

/*
  ==============================================================================

    Delay.h
    Created: 6 Nov 2019 5:17:07pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define MAXIMUM_DELAY 1058
//==============================================================================
/*
*/
class Delay
{
public:
    Delay();
    
    /**Works out delay time and returns it in samples
     
     listenerDistance = The distance the listener is from the source in meters
     sampleRate = the sample rate the audio is being played at
     
     Returns delay time in samples*/
    int calculateDelayTime(float listenerDistance, int sampleRate);

    /**Outputs delayed value
     Do not run past 88200Hz since buffer size may not allow for it
     
     listenerDistance = The distance the listener is from the source in meters
     sampleRate = the sample rate the audio is being played at
     currentInputSample = non delayed sample that would have been played*/
    void getDelayValues(float listenerDistance, int sampleRate, int blockSize, const float* inputBlock);
    
    void pushDelayToBuffer(float* outputBlock, int blockSize);
    
    /**Resets the write position back to the start of the buffer and fills the buffer with null to stop artefacts appearing*/
    void audioSourceChanged();
private:
    
    float buffer[MAXIMUM_DELAY];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Delay)
};

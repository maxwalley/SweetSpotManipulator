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

    void performDelay(AudioBuffer<float>& inputBuffer, float listenerDistance, int sampleRate, int channelNum);
private:
    
    //AudioBuffer<float> delayBuffer;
    int delayBufferWritePosition[2];
    float delayBuffer[2][44100];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Delay)
};

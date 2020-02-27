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
    
    /**Works out delay time in samples
     
     @param listenerDistance = The distance the listener is from the source in meters
     @param sampleRate = the sample rate the audio is being played at
     
     @returns delay time in samples
     */
    int calculateDelayTime(float listenerDistance, int sampleRate);

    /**Performs delay based upon listener distance on an audio buffer
     
     @param inputBuffer = the buffer to take samples from and return delayed samples to
     @param listenerDistance = the listeners distance from the source in meters
     @param sampleRate = sample rate of the audio
     @param channelNum = which stereo channel the buffer is from either 0 or 1
     */
    void performDelay(AudioBuffer<float>& inputBuffer, float listenerDistance, int sampleRate, int channelNum);
private:
    
    int delayBufferWritePosition[2];
    float delayBuffer[2][44100];
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Delay)
};

/*
  ==============================================================================

    Delay.cpp
    Created: 6 Nov 2019 5:17:07pm
    Author:  Maximillian Walley

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Delay.h"

#define SPEED_OF_SOUND 343

//==============================================================================
Delay::Delay()
{
    for(int channel = 0; channel < 2; channel++)
    {
        delayBufferWritePosition[channel] = 0;
        
        for (int sample = 0; sample < 44100; sample++)
        {
            delayBuffer[channel][sample] = 0;
        }
    }
}

int Delay::calculateDelayTime(float listenerDistance, int sampleRate)
{
    float delayTimeInSecs = listenerDistance/SPEED_OF_SOUND;
    
    //Rounds the number of samples of delay as in David Creaseys book pg. 234
    return floor(delayTimeInSecs * sampleRate);
}

void Delay::performDelay(AudioBuffer<float>& inputBuffer, float listenerDistance, int sampleRate, int channelNum)
{
    int delayTimeInSamples = calculateDelayTime(listenerDistance, sampleRate);
    
    const float* pIn = inputBuffer.getReadPointer(channelNum);
    
    float* pOut = inputBuffer.getWritePointer(channelNum);
    
    int nbSamples = inputBuffer.getNumSamples();
    
    float fIn, fOut = 0;
    
    while (nbSamples--)
    {
        //Puts value from input buffer write pointer into variable
        fIn = *pIn++;
        
        //Moves the write position of the delay buffer on one
        delayBufferWritePosition[channelNum]++;
        
        //Checks we're not at the end of the delay buffer.
        if(delayBufferWritePosition[channelNum] == 44099)
        {
            //If we are sets write position back to 0
            delayBufferWritePosition[channelNum] = 0;
        }
        
        //Writes the value from the input buffer into the delay buffer
        delayBuffer[channelNum][delayBufferWritePosition[channelNum]] = fIn;
        
        //Finds the delayed sample to return
        int delayBufferReadPosition = delayBufferWritePosition[channelNum] - delayTimeInSamples;
        
        //If this sample to return is meant to be in a position less than 0
        if(delayBufferReadPosition < 0)
        {
            //Finds the sample at the end of the buffer
            delayBufferReadPosition += 44100;
        }
        
        //Puts the delayed sample into a variable
        fOut = delayBuffer[channelNum][delayBufferReadPosition];
        
        //DBG("input = " << fIn << " output = " << fOut << " On Sample: " << delayBufferWritePosition[channelNum]);
        
        //Writes this delayed sample to the input buffer
        *pOut++ = fOut;
    }
}

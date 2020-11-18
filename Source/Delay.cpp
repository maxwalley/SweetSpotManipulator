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
        
        //Clears buffer
        for (int sample = 0; sample < 44100; sample++)
        {
            delayBuffer[channel][sample] = 0;
        }
        
        delayTimeInSamples[channel] = 0;
    }
}

int Delay::calculateDelayTime(float listenerDistance, int sampleRate, int channel)
{
    //Time = Dis/Speed
    float delayTimeInSecs = listenerDistance/SPEED_OF_SOUND;
    
    //Works it out with the sample rate and rounds it down - See David Creaseys book pg. 234
    delayTimeInSamples[channel] = floor(delayTimeInSecs * sampleRate);
    
    //Works which channel has the shorter delay time
    //If delay time is shorter on selected channel returns the larger time - this one
    if(channel == 0)
    {
        if(delayTimeInSamples[0] >= delayTimeInSamples[1])
        {
            return 0;
        }
        else
        {
            return delayTimeInSamples[1] - delayTimeInSamples[0];
        }
    }
    else if(channel == 1)
    {
        if(delayTimeInSamples[1] >= delayTimeInSamples[0])
        {
            return 0;
        }
        else
        {
            return delayTimeInSamples[0] - delayTimeInSamples[1];
        }
    }
    
}

void Delay::performDelay(AudioBuffer<float>& inputBuffer, float listenerDistance, int sampleRate, int channelNum)
{
    int delayTimeInSamples = calculateDelayTime(listenerDistance, sampleRate, channelNum);
    
    //DBG("delay time on channel" << channelNum << "=" << delayTimeInSamples);
    
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
        //fOut = delayBuffer[channelNum][delayBufferReadPosition];
        
        //For Test
        fOut = delayBuffer[channelNum][delayBufferReadPosition];
        
        //DBG("input = " << fIn << " output = " << fOut << " On Sample: " << delayBufferWritePosition[channelNum]);
        
        //Writes this delayed sample to the input buffer
        *pOut++ = fOut;
    }
}

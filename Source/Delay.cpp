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
    //delayBuffer.setSize(2, 88200);
    //delayBuffer.clear();
    
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
    //if(inputBuffer.getNumChannels() != delayBuffer.getNumChannels())
        //delayBuffer.setSize(inputBuffer.getNumChannels(), 88200);
    
    int delayTimeInSamples = calculateDelayTime(listenerDistance, sampleRate);
    
    const float* pIn = inputBuffer.getReadPointer(channelNum);
    
    float* pOut = inputBuffer.getWritePointer(channelNum);
    
    int nbSamples = inputBuffer.getNumSamples();
    
    float fIn, fOut = 0;
    
    while (nbSamples--)
    {
        fIn = *pIn++;
        
        delayBufferWritePosition[channelNum]++;
        if(delayBufferWritePosition[channelNum] == 44099)
        {
            delayBufferWritePosition[channelNum] = 0;
        }
        delayBuffer[channelNum][delayBufferWritePosition[channelNum]] = fIn;
        
        int delayBufferReadPosition = delayBufferWritePosition[channelNum] - delayTimeInSamples;
        if(delayBufferReadPosition < 0)
        {
            delayBufferReadPosition += 44100;
        }
        fOut = delayBuffer[channelNum][delayBufferReadPosition];

        fOut = fIn;
        
        *pOut++ = fOut;
    }
    
    /*const float* pIn0 = inputBuffer.getReadPointer(0);
    const float* pIn1 = inputBuffer.getReadPointer(1);
    
    float* pOut0 = inputBuffer.getWritePointer(0);
    float* pOut1 = inputBuffer.getWritePointer(1);
    
    int nbSamples = inputBuffer.getNumSamples();
    
    float fIn0, fIn1, fOut0 = 0, fOut1 = 0;
    
    while (nbSamples--)
    {
        fIn0 = *pIn0++;
        fIn1 = *pIn1++;
        
        delayBufferWritePosition++;
        if(delayBufferWritePosition == 44099)
        {
            delayBufferWritePosition = 0;
        }
        delayBuffer[0][delayBufferWritePosition] = fIn0;
        delayBuffer[1][delayBufferWritePosition] = fIn1;
        
        int delayBufferReadPosition = delayBufferWritePosition - delayTimeInSamples;
        if(delayBufferReadPosition < 0)
        {
            delayBufferReadPosition += 44100;
        }
        fOut0 = delayBuffer[0][delayBufferReadPosition];
        fOut1 = delayBuffer[1][delayBufferReadPosition];

        fOut0 = fIn0;
        fOut1 = fIn1;
        
        *pOut0++ = fOut0;
        *pOut1++ = fOut1;
    }*/
}

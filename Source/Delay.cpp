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
    
}

int Delay::calculateDelayTime(float listenerDistance, int sampleRate)
{
    float delayTimeInSecs = listenerDistance/SPEED_OF_SOUND;
    
    //Rounds the number of samples of delay as in David Creaseys book pg. 234
    return floor(delayTimeInSecs * sampleRate);
}

void Delay::getDelayValues(float listenerDistance, int sampleRate, int blockSize, const float* inputBlock)
{
    int delay = calculateDelayTime(listenerDistance, sampleRate);
    
    //Wipes delay buffer clean of last values
    for(int i = 0; i < MAXIMUM_DELAY; i++)
    {
        buffer[i] = 0;
    }
    
    //Iterates through every sample in the block
    for(int i = 0; i < blockSize; i++)
    {
        int readPosition = i - delay;
        
        if(readPosition < 0)
        {
            readPosition = readPosition + MAXIMUM_DELAY;
        }
        
        buffer[i] = inputBlock[readPosition];
    }
}

void Delay::pushDelayToBuffer(float* outputBlock, int blockSize)
{
    for(int i = 0; i < blockSize; i++)
    {
        outputBlock[i] = buffer[i];
    }
}

void Delay::audioSourceChanged()
{
    for(int index = 0; index < MAXIMUM_DELAY; index++)
    {
        buffer[index] = 0;
    }
}

/*
  ==============================================================================

    DistanceBasedAttenuationCalculator.h
    Created: 31 Oct 2019 10:43:20am
    Author:  Maximillian Walley

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
    A class for determining how much attenuation to put on an audio based on the users distance from a loudspeaker
*/
class DistanceBasedAttenuationCalculator
{
public:
    /**Constructor*/
    DistanceBasedAttenuationCalculator();
    
    /**Destructor*/
    ~DistanceBasedAttenuationCalculator();
    
    /**Returns a multiplier to apply to audio to attenuate for distance
     
     @param speaker         the channel to return the multiplier for
     @param xPos            the users position on the X axis of the colour image
     @param valueAtXPos         the depth data from the depth image at the users position
     
     @returns the multiplier to use
     */
    float workOutMultiplier(int speaker, int xPos, int valueAtXPos);
    
    /**Returns how far a listener is from a loudspeaker in meters
     
     @param speaker         the loudspeaker to work out the distance from
     @param xPos            the users position on the X axis of the colour image
     @param valueAtXPos         the depth data from the depth image at the users position
     
     @returns the users distance from the loudspeaker in meters
     */
    float getListenerDistance(int channel, int xPos, int valueAtXPos);
    
    /**Returns a multiplier to apply to audio to attenuate for distance
    
    @param channel         the channel to return the multiplier for
    @param xPos            the users position on the X axis of the colour image
    @param valueAtXPos         the depth data from the depth image at the users position
    
    @returns the multiplier to use
    */
    float getMultiplier(int channel, int xPos, int valueAtXpos);
    
    /**Returns the amount of gain that should be applied to the left channel of audio
     
     @returns the amount of gain that should be applied to the left channel of audio
     */
    float getLeftGain();
    
    /**Returns the amount of gain that should be applied to the right channel of audio
    
    @returns the amount of gain that should be applied to the right channel of audio
    */
    float getRightGain();
    
    /**Sets the speaker co-ordinates
     
     @param speakerNum          the speaker number to change the co-ordinates of
     @param xCoOrdnate          the speakers position on the x axis
     @param zCoOrdinate         the speakers position on the y axis
     */
    void setSpeakerCoOrdinates(int speakerNum, float xCoOrdinate, float zCoOrdinate);
    
    /**Sets the distance in meters to simulate through attenuation
     
    @param distance        intended distance to simulate
     */
    void setDistanceToSimulate(float distance);
    
private:
    
    float maxYDis;
    
    float currentDepthAtXPos;
    
    /**Works out a users distance from a loudspeaker on the x axis
     
     @param speaker         the loudspeaker to work out the distance from
     @param xPos            the users position on the X axis of the colour image
     @param valueAtXPos         the depth data from the depth image at the users position
     
     @returns the users distance from the loudspeaker on the x axis
     */
    float workOutLisDisXAxis(int speaker, int xPos, int valueAtXPos);
    
    /**Works out a users distance from a loudspeaker on the z axis
    
    @param speaker         the loudspeaker to work out the distance from
    @param xPos            the users position on the X axis of the colour image
    @param valueAtXPos         the depth data from the depth image at the users position
    
    @returns the users distance from the loudspeaker on the z axis
    */
    float workOutLisDisZAxis(int speaker, int valueAtXPos);
    
    struct Coordinates
    {
        float x;
        float z;
    };
    
    Array<Coordinates> speakerPositions;
    
    /**Works out a users distance from a loudspeaker
    
    @param speaker         the loudspeaker to work out the distance from
    @param xPos            the users position on the X axis of the colour image
    @param valueAtXPos         the depth data from the depth image at the users position
    
    @returns the users distance from the loudspeaker
    */
    float workOutListenerDistance(int speaker, int xPos, int valueAtXPos);
    
    float leftGain;
    float rightGain;
    
    float idealSpotDis;
    float dbAtSpotDis;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DistanceBasedAttenuationCalculator)
};

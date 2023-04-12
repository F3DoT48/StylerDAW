/*
  ==============================================================================

    ArrIDs.h
    Created: 1 Nov 2022 11:50:01am
    Author:  User

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace ArrangerIDs
{
    #define DECLARE_ID(name) const juce::Identifier name (#name);
    
    DECLARE_ID (note)
    DECLARE_ID (startBeat)
    DECLARE_ID (lengthInBeats)
    DECLARE_ID (noteIndex)
    DECLARE_ID (velocity)
    DECLARE_ID (isMute)
    DECLARE_ID (transpositionRule)
    DECLARE_ID (channelIndex)
    DECLARE_ID (arrangementSequence)
    DECLARE_ID (arrangementPattern)
    DECLARE_ID (arrangementSection)
    //DECLARE_ID (arrangementSectionIndex)
    DECLARE_ID (lengthInBars)
    DECLARE_ID (playbackMode)
    DECLARE_ID (arrangerTrackId)
    DECLARE_ID (arrangement)

}
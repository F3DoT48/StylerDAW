/*
  ==============================================================================

    ArrangementPart.h
    Created: 31 Oct 2022 2:21:14pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MidiSequence.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class ArrangementPattern : public juce::ReferenceCountedObject
    {
    public:
        ArrangementPattern() = delete;
        ArrangementPattern (const juce::ValueTree&, juce::UndoManager*);

        using Ptr = juce::ReferenceCountedObjectPtr<ArrangementPattern>;

        static juce::ValueTree createArrangementPattern (te::EditItemID trackId);
        
        MidiSequence* getMidiSequenceForChannel (int channelIndex);

        juce::ValueTree mState;
        /* !!! hack for te::ValueTreeUtilities */
        juce::ValueTree& state { mState };

    private:
        te::EditItemID mTrackId;

        juce::OwnedArray<MidiSequence> mSequences;
        static constexpr int sNumMaxSequences { 16 };

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementPattern)
    };
}



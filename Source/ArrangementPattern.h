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
        ArrangementPattern (te::Edit&
                          //, juce::ReferenceCountedObjectPtr<ArrangementSection> arrangementSection
                          //, te::Track::Ptr
                          , const juce::ValueTree&
                          , juce::UndoManager*);

        using Ptr = juce::ReferenceCountedObjectPtr<ArrangementPattern>;

        static juce::ValueTree createArrangementPattern (const te::EditItemID trackId);
        
        MidiSequence* getMidiSequenceForChannel (int channelIndex);

        juce::ValueTree mState;
        /* !!! hack for te::ValueTreeUtilities */
        juce::ValueTree& state { mState };

        te::AudioTrack::Ptr getAssociatedAudioTrack() const;

    private:
        te::EditItemID mTrackId;
        te::AudioTrack::Ptr mTrack;

        juce::OwnedArray<MidiSequence> mSequences;
        static constexpr int sNumMaxSequences { 16 };

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementPattern)
    };
}



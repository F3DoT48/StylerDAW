/*
  ==============================================================================

    ArrangementController.h
    Created: 10 Aug 2023 2:10:56pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Chord.h"

namespace te = tracktion_engine;
namespace styler_app
{
    class ArrangementController
    {
    public:
        ArrangementController (juce::ValueTree& arrangementState
                             , juce::UndoManager* undoManager);

        int getSplitNoteIndex() const;
        void setSplitNoteIndex (int newSplitNoteIndex);

        int getKeyNoteIndex() const;
        void setKeyNoteIndex(int newKeyNoteIndex);

        void setLastDetectedChord (const Chord& newChord);
        Chord getLastDetectedChord() const;

        void setCurrentSectionIndex (int newSectionIndex);
        int getCurrentSectionIndex() const;

        void setChordChangeTimeResolutionInBeats (float newTimeResolution);
        float getChordChangeTimeResolutionInBeats() const;

        void setSectionChangeTimeResolutionInBeats (float newTimeResolution);
        float getSectionChangeTimeResolutionInBeats() const;

    private:
        juce::CachedValue<te::AtomicWrapper<int>> mSplitNoteIndex; // 0-127
        juce::CachedValue<te::AtomicWrapper<int>> mKeyNoteIndex; // 0-11 -> from c to B
        std::atomic<Chord> mLastDetectedChord;
        std::atomic<int> mCurrentSectionIndex;
        std::atomic<float> mChordChangeTimeResolutionInBeats;
        std::atomic<float> mSectionChangeTimeResolutionInBeats;
    };
}
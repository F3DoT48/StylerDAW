/*
  ==============================================================================

    ChordDetector.h
    Created: 19 Jun 2023 10:38:54pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <bitset>

#include "Chord.h"

namespace styler_app
{
    class ChordNotesTracker
    {
    public:
        ChordNotesTracker();

        static inline constexpr int TrackingRange { 36 }; // 3 octaves

        void noteOn (int noteIndex);
        void noteOff (int noteIndex);

        std::bitset<TrackingRange> getNotesAsBitSequence() const;

    private:
        std::bitset<TrackingRange> mNotesBits;
        std::bitset<TrackingRange> mNotesOnHoldBits;
    };

    class ChordDetector
    {
    public:
        ChordDetector();

        Chord getCurrentChord();

        ChordNotesTracker mChordNotesTracker;

    private:
        Chord mPreparedChord;
    };
}
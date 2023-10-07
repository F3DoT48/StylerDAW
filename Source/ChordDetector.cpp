/*
  ==============================================================================

    ChordDetector.cpp
    Created: 19 Jun 2023 10:38:54pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ChordDetector.h"

using namespace styler_app;

ChordNotesTracker::ChordNotesTracker()
    : mNotesBits{}
    , mNotesOnHoldBits{}
{

}

void ChordNotesTracker::noteOn (int noteIndex)
{
    mNotesBits.set (noteIndex % TrackingRange,  true);
    mNotesOnHoldBits = mNotesBits;
}

void ChordNotesTracker::noteOff (int noteIndex)
{
    mNotesBits.set (noteIndex % TrackingRange, false);
    /*if (mNotesBits.count() == 0)
        mNotesOnHoldBits.reset();*/
}

std::bitset<ChordNotesTracker::TrackingRange> ChordNotesTracker::getNotesAsBitSequence() const
{
    return mNotesOnHoldBits;
}

ChordDetector::ChordDetector()
    : mPreparedChord { 0, Chord::Type::maj }
{

}

Chord ChordDetector::getCurrentChord()
{
    auto notesBits { mChordNotesTracker.getNotesAsBitSequence() };
    
    if (notesBits.any())
    {
        int shift { 0 };
        while (! notesBits.test (0))
        {
            notesBits >>= 1;
            ++shift;
        }

        shift %= 12;

        switch (notesBits.to_ulong())
        {
            // one key down
            case 1ul:   // root note
                mPreparedChord.setRootIndex (shift);
                mPreparedChord.setType(Chord::Type::maj);
                break;
            // two keys down
            case 17ul:  // maj 3rd
                mPreparedChord.setRootIndex (shift);
                mPreparedChord.setType(Chord::Type::maj);
                break;
            case 9ul:   // min 3rd
                mPreparedChord.setRootIndex (shift);
                mPreparedChord.setType(Chord::Type::min);
                break;
            case 3ul:   // two newighbor notes
                mPreparedChord.setRootIndex ((shift + 1) % 12);
                mPreparedChord.setType(Chord::Type::min);
                break;
            // 3 keys down
            case 145ul: // maj triad
                mPreparedChord.setRootIndex (shift);
                mPreparedChord.setType(Chord::Type::maj);
                break;
            case 265ul: // 1st inv maj triad
                mPreparedChord.setRootIndex ((shift + 8) % 12);
                mPreparedChord.setType(Chord::Type::maj);
                break;
            case 545ul: // 2nd inv maj triad
                mPreparedChord.setRootIndex ((shift + 5) % 12);
                mPreparedChord.setType(Chord::Type::maj);
                break;
            case 137ul: // min triad
                mPreparedChord.setRootIndex (shift);
                mPreparedChord.setType(Chord::Type::min);
                break;
            case 529ul: // 1st inv min triad
                mPreparedChord.setRootIndex ((shift + 9) % 12);
                mPreparedChord.setType(Chord::Type::min);
                break;
            case 289ul: // 2nd inv mon triad
                mPreparedChord.setRootIndex ((shift + 5) % 12);
                mPreparedChord.setType(Chord::Type::min);
                break;
            default:
                break;
        }
    }

    return mPreparedChord;
}
/*
  ==============================================================================

    ArrangementController.cpp
    Created: 10 Aug 2023 2:10:56pm
    Author:  User

  ==============================================================================
*/

#include "ArrangementController.h"
#include "ArrangerIDs.h"

using namespace styler_app;

ArrangementController::ArrangementController (juce::ValueTree& arrangementState
                                            , juce::UndoManager* undoManager)
    : mLastDetectedChord { Chord {0, Chord::Type::maj} }
    , mCurrentSectionIndex { 0 }
    , mChordChangeTimeResolutionInBeats { 0.5f }
    , mSectionChangeTimeResolutionInBeats { 1.0f }
{
    mSplitNoteIndex.referTo (arrangementState
                           , ArrangerIDs::splitNoteIndex
                           , undoManager);

    mKeyNoteIndex.referTo (arrangementState
                         , ArrangerIDs::keyNoteIndex
                         , undoManager);

    jassert (std::atomic<Chord>::is_always_lock_free);
}

int ArrangementController::getSplitNoteIndex() const
{
    return static_cast<uint8_t> (mSplitNoteIndex.get());
}

void ArrangementController::setSplitNoteIndex (int newSplitNoteIndex)
{
    mSplitNoteIndex.setValue (newSplitNoteIndex, nullptr);
}

int ArrangementController::getKeyNoteIndex() const
{
    return mKeyNoteIndex.get();
}

void ArrangementController::setKeyNoteIndex (int newKeyNoteIndex)
{
    mKeyNoteIndex.setValue (newKeyNoteIndex, nullptr);
}

void ArrangementController::setLastDetectedChord (const Chord& newChord)
{
    mLastDetectedChord.store (newChord);
}

Chord ArrangementController::getLastDetectedChord() const
{
    return mLastDetectedChord.load();
}

void ArrangementController::setCurrentSectionIndex (int newSectionIndex)
{
    mCurrentSectionIndex.store (newSectionIndex);
}

int ArrangementController::getCurrentSectionIndex() const
{
    return mCurrentSectionIndex.load();
}

void ArrangementController::setChordChangeTimeResolutionInBeats (float newTimeResolutionInBeats)
{
    mChordChangeTimeResolutionInBeats.store (newTimeResolutionInBeats);
}

float ArrangementController::getChordChangeTimeResolutionInBeats() const
{
    return mChordChangeTimeResolutionInBeats.load();
}

void ArrangementController::setSectionChangeTimeResolutionInBeats (float newTimeResolutionInBeats)
{
    mSectionChangeTimeResolutionInBeats.store (newTimeResolutionInBeats);
}

float ArrangementController::getSectionChangeTimeResolutionInBeats() const
{
    return mSectionChangeTimeResolutionInBeats.load();
}
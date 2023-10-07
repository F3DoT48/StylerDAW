/*
  ==============================================================================

    MidiNote.cpp
    Created: 29 Mar 2023 5:59:16pm
    Author:  User

  ==============================================================================
*/

#include "MidiNote.h"
#include "ArrangerIDs.h"
#include "ArrangerEngine.h"

using namespace styler_app;

MidiNote::MidiNote (const juce::ValueTree& valueTree)
    : mState (valueTree)
{
    jassert (mState.hasType (ArrangerIDs::note));
    updatePropertiesFromState();
}

void MidiNote::updatePropertiesFromState()
{
    mStartBeat = static_cast<double> (mState.getProperty (ArrangerIDs::startBeat));
    mLengthInBeats = std::max (0.0, static_cast<double> (mState.getProperty (ArrangerIDs::lengthInBeats)));
    mNoteIndex = static_cast<uint8_t> (juce::jlimit (0, 127, static_cast<int> (mState.getProperty (ArrangerIDs::noteIndex))));
    mVelocity = static_cast<uint8_t> (juce::jlimit (0, 127, static_cast<int> (mState.getProperty (ArrangerIDs::velocity))));
    mIsMute = mState.getProperty (ArrangerIDs::isMute) ? 1 : 0;
    mTranspositionRuleType = static_cast<NoteTranspositionRule::Type> (static_cast<int> (mState.getProperty (ArrangerIDs::transpositionRuleType)));
}

juce::ValueTree MidiNote::getValueTreeFromNote (const MidiNote& midiNote)
{
    juce::ValueTree valueTree { midiNote.mState.createCopy() };

    return valueTree;
}

juce::ValueTree MidiNote::createNoteValueTree (double startBeat
                                             , double lengthInBeats
                                             , int noteIndex
                                             , int velocity
                                             , bool isMute
                                             , NoteTranspositionRule::Type transpositionRuleType)
{
    return te::createValueTree (ArrangerIDs::note
                              , ArrangerIDs::noteIndex, noteIndex
                              , ArrangerIDs::startBeat, startBeat
                              , ArrangerIDs::lengthInBeats, std::max (0.0, lengthInBeats)
                              , ArrangerIDs::velocity, velocity
                              , ArrangerIDs::isMute, isMute ? 1 : 0
                              , ArrangerIDs::transpositionRuleType, static_cast<int> (transpositionRuleType));
}

void MidiNote::setStartBeat (double newStartBeat, juce::UndoManager* undoManager)
{
    newStartBeat = std::max (0.0, newStartBeat);
    if (mStartBeat != newStartBeat)
    {
        mState.setProperty (ArrangerIDs::startBeat, newStartBeat, undoManager);
        mStartBeat = newStartBeat;
    }
}

double MidiNote::getStartBeat() const
{
    return mStartBeat;
}

void MidiNote::setLengthInBeats (double newLengthInBeats, juce::UndoManager* undoManager)
{
    if (newLengthInBeats <= 0.0)
    {
        newLengthInBeats = 1.0 / te::Edit::ticksPerQuarterNote;
    }

    if (mLengthInBeats != newLengthInBeats)
    {
        mState.setProperty (ArrangerIDs::lengthInBeats, newLengthInBeats, undoManager);
        mLengthInBeats = newLengthInBeats;
    }
}

double MidiNote::getLengthInBeats() const
{
    return mLengthInBeats;
}

void MidiNote::setNoteIndex (int newNoteIndex, juce::UndoManager* undoManager)
{
    newNoteIndex = juce::jlimit (0, 127, newNoteIndex);

    if (getNoteIndex() != newNoteIndex)
    {
        mState.setProperty (ArrangerIDs::noteIndex, newNoteIndex, undoManager);
        mNoteIndex = static_cast<uint8_t> (newNoteIndex);
    }
}

int MidiNote::getNoteIndex() const
{
    return static_cast<int> (mNoteIndex);
}

void MidiNote::setVelocity(int newVelocity, juce::UndoManager* undoManager)
{
    newVelocity = juce::jlimit (0, 127, newVelocity);
    if (getVelocity() != newVelocity)
    {
        mState.setProperty (ArrangerIDs::velocity, newVelocity, undoManager);
        mVelocity = static_cast<uint8_t> (newVelocity);
    }
}

int MidiNote::getVelocity() const
{
    return static_cast<int> (mVelocity);
}

void MidiNote::setMute (bool shouldMute, juce::UndoManager* undoManager)
{
    if (isMute() != shouldMute)
    {
        mState.setProperty (ArrangerIDs::isMute, shouldMute, undoManager);
        mIsMute = static_cast<uint8_t> (shouldMute ? 1 : 0);
    }
}

bool MidiNote::isMute() const
{
    return mIsMute != 0;
}

void MidiNote::setTranspositionType (NoteTranspositionRule::Type newTranspositionRuleType, juce::UndoManager* undoManager)
{
    if (getTranspositionType() != newTranspositionRuleType)
    {
        mState.setProperty (ArrangerIDs::transpositionRuleType, static_cast<int> (newTranspositionRuleType), undoManager);
        mTranspositionRuleType = newTranspositionRuleType;
    }
}

NoteTranspositionRule::Type MidiNote::getTranspositionType() const
{
    return mTranspositionRuleType;
}
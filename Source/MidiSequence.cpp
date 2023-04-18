/*
  ==============================================================================

    MidiList.cpp
    Created: 29 Mar 2023 5:59:39pm
    Author:  User

  ==============================================================================
*/

#include "MidiSequence.h"
#include "ArrangerIDs.h"

using namespace styler_app;

MidiSequence::MidiSequence (const juce::ValueTree& valueTree
                          , juce::UndoManager* undoManager)
    : mState { valueTree }
{
    jassert (mState.hasType (ArrangerIDs::arrangementSequence));

    /*
    if (valueTree.hasType (ArrangerIDs::note))
    {
        convertPropertyToType<double> (mState, ArrangerIDs::startBeat);
        convertPropertyToType<double> (mState, ArrangerIDs::lengthInBeats);
        convertPropertyToType<int> (mState, ArrangerIDs::noteIndex);
        convertPropertyToType<int> (mState, ArrangerIDs::velocity);
        convertPropertyToType<int> (mState, ArrangerIDs::isMute);
        convertPropertyToType<int> (mState, ArrangerIDs::transpositionRule);
    }*/

    mMidiChannel.referTo  (mState, ArrangerIDs::channelIndex, undoManager);

    mNoteList = std::make_unique<EventList<MidiNote>> (mState);

    //startTimer (sTimerPeriodInMilliseconds);
}

MidiSequence::~MidiSequence()
{
    //stopTimer();
}

juce::ValueTree MidiSequence::createMidiSequence (te::MidiChannel midiChannel)
{
    return te::createValueTree (ArrangerIDs::arrangementSequence
                              , ArrangerIDs::channelIndex, midiChannel);
}

const juce::Array<MidiNote*>& MidiSequence::getNotes() const
{
    jassert (mNoteList != nullptr);
    return mNoteList->getSortedList();
}

te::MidiChannel MidiSequence::getMidiChannel() const noexcept
{
    return mMidiChannel;
}

void MidiSequence::setMidiChannel (te::MidiChannel newMidiChannel)
{
    mMidiChannel = newMidiChannel;
}

void MidiSequence::generateAndCacheForAudioThread()
{
    auto copyOfThis { std::make_shared<MidiSequence> (mState, nullptr) };
    mGarbagePool.addSharedInstance (copyOfThis);
    mAtomicSharedForAudioThread.exchange (copyOfThis);
}

/*void MidiSequence::timerCallback()
{
    generateAndCacheForAudioThread();
    mGarbagePool.cleanUp();
}*/

template<>
struct MidiSequence::EventDelegate<MidiNote>
{
    static bool isSuitableType (const juce::ValueTree& valueTree)
    {
        return valueTree.hasType (ArrangerIDs::note);
    }

    static bool updateObject (MidiNote& note, const juce::Identifier& identifier)
    {
        note.updatePropertiesFromState();
        return identifier == ArrangerIDs::startBeat;
    }

    static void removeFromSelection (MidiNote* note)
    {
        removeMidiEventFromSelection (note);
    }
};

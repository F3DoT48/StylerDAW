/*
  ==============================================================================

    MidiList.cpp
    Created: 29 Mar 2023 5:59:39pm
    Author:  User

  ==============================================================================
*/

#include "MidiSequence.h"

using namespace styler_app;

namespace styler_app
{
    namespace
    {
        template <typename Type>
        static void removeMidiEventFromSelection (Type* event)
        {
            //for (te::SelectionManager::Iterator sm; sm.next();)
            //    if (auto sme = sm->getFirstItemOfType<SelectedMidiEvents>())
            //        sme->removeSelectedEvent (event);
        }

        template<typename VarType>
        inline void convertPropertyToType (juce::ValueTree& valueTree
                                         , const juce::Identifier& identifier)
        {
            if (const auto* prop = valueTree.getPropertyPointer (identifier))
            {
                if (prop->isString())
                {
                    (*const_cast<juce::var*> (prop)) = static_cast<VarType> (*prop);
                }
            }
        }
    }
}

template<>
class MidiSequence::EventDelegate<MidiNote>
{
public:
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

te::MidiChannel MidiSequence::getMidiChannel() const
{
    return mMidiChannel;
}

void MidiSequence::setMidiChannel (te::MidiChannel newMidiChannel)
{
    mMidiChannel = newMidiChannel;
}

void MidiSequence::addNote (double startBeat
                          , double lengthInBeats
                          , int noteIndex
                          , int velocity
                          , bool isMute
                          , NoteTranspositionRule::Type transpositionRuleType
                          , juce::UndoManager* undoManager)
{
    auto valueTree {MidiNote::createNoteValueTree (startBeat
                                                 , lengthInBeats
                                                 , noteIndex
                                                 , velocity
                                                 , isMute
                                                 , transpositionRuleType)};
    mState.addChild (valueTree, -1, undoManager);
}

void MidiSequence::removeNote (MidiNote& note, juce::UndoManager* um)
{
    mState.removeChild (note.state, um);
}



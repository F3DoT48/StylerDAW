

#include "ArrangementPattern.h"
#include "ArrangerIDs.h"

using namespace styler_app;

ArrangementPattern::ArrangementPattern (const juce::ValueTree& valueTree
                                      , juce::UndoManager* undoManager)
    : mState { valueTree }
{
    jassert (valueTree.hasType (ArrangerIDs::arrangementPattern));

    mTrackId = te::EditItemID::fromVar (valueTree.getProperty (ArrangerIDs::arrangerTrackId));

    for (auto seqState : mState)
        mSequences.add (new MidiSequence (seqState, undoManager));
}

juce::ValueTree ArrangementPattern::createArrangementPattern (te::EditItemID trackId)
{
    auto valueTree { te::createValueTree (ArrangerIDs::arrangementPattern
                                        , ArrangerIDs::arrangerTrackId, trackId)};

    for (int seqId { 1 }; seqId <= sNumMaxSequences; ++seqId)
    {
        valueTree.appendChild (MidiSequence::createMidiSequence (te::MidiChannel (seqId))
                             , nullptr);
    }

    return valueTree;
}

MidiSequence* ArrangementPattern::getMidiSequenceForChannel (int channelIndex)
{
    return mSequences[channelIndex - 1];
}

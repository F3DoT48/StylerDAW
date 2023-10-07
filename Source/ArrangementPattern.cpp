

#include "ArrangementPattern.h"
#include "ArrangerIDs.h"

using namespace styler_app;

ArrangementPattern::ArrangementPattern (te::Edit& edit
                                      , const juce::ValueTree& valueTree
                                      , juce::UndoManager* undoManager)
    : mState { valueTree }
    , mTrack { nullptr }
{
    jassert (valueTree.hasType (ArrangerIDs::arrangementPattern));
    jassert (valueTree.hasProperty (ArrangerIDs::arrangementTrackId));

    mTrackId = te::EditItemID::fromVar (valueTree.getProperty (ArrangerIDs::arrangementTrackId));

    auto tracks {te::getAudioTracks (edit)};

    DBG ("numTracks = " << tracks.size());

    for (int trackId {1}; trackId < tracks.size(); ++trackId)
    {
        DBG (mTrackId.toString() << " " << tracks[trackId]->itemID.toString());
        if (tracks[trackId]->itemID == mTrackId)
        {
            mTrack = tracks[trackId];
            break;
        }
    }

    jassert (mTrack != nullptr);

    for (auto seqState : mState)
        mSequences.add (new MidiSequence (seqState, undoManager));
}

juce::ValueTree ArrangementPattern::createArrangementPattern (const te::EditItemID trackId)
{
    DBG (trackId.toString());
    auto valueTree { te::createValueTree (ArrangerIDs::arrangementPattern
                                        , ArrangerIDs::arrangementTrackId, trackId)};

    jassert (valueTree.hasType (ArrangerIDs::arrangementPattern));
    jassert (valueTree.hasProperty (ArrangerIDs::arrangementTrackId));

    for (int seqId { 1 }; seqId <= sNumMaxSequences; ++seqId)
    {
        valueTree.appendChild (MidiSequence::createMidiSequence (te::MidiChannel (seqId))
                             , nullptr);
    }

    return valueTree;
}

MidiSequence* ArrangementPattern::getMidiSequenceForChannel (int channelIndex)
{
    jassert (!mSequences.isEmpty());
    return mSequences[channelIndex - 1];
}


te::AudioTrack::Ptr ArrangementPattern::getAssociatedAudioTrack() const
{
    jassert (mTrack != nullptr);
    return mTrack;
}
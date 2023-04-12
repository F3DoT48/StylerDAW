/*
  ==============================================================================

    ArrangementSection.cpp
    Created: 31 Mar 2023 12:57:32pm
    Author:  User

  ==============================================================================
*/

#include "ArrangementSection.h"

#include "ArrangerIDs.h"

using namespace styler_app;

ArrangementSection::ArrangementSection (te::Edit& edit
                                      , const juce::ValueTree& valueTree
                                      , juce::UndoManager* undoManager)
    : mEdit { edit }
    , mState { valueTree }
    , mPatterns { std::make_unique<ArrangementPatternList> (mEdit
                                                          , mState
                                                          , nullptr) }
{
    jassert (valueTree.hasType (ArrangerIDs::arrangementSection));

    mLengthInBars = static_cast<int> (valueTree.getProperty (ArrangerIDs::lengthInBars));
    switch (static_cast<int> (valueTree.getProperty (ArrangerIDs::playbackMode)))
    {
        case 0:
            mPlaybackMode = PlaybackMode::loop;
            break;
        case 1:
            mPlaybackMode = PlaybackMode::fill;
            break;
        case 2:
            mPlaybackMode = PlaybackMode::intro;
            break;
        case 3:
            mPlaybackMode = PlaybackMode::ending;
            break;
        default:
            jassert ("Unknown playback mode");
            break;
    }

    mEdit.state.addListener (this);
}

ArrangementSection::~ArrangementSection()
{
    mEdit.state.removeListener (this);
}

juce::ValueTree ArrangementSection::createSection (const te::Edit& edit)
{
    auto valueTree { te::createValueTree (ArrangerIDs::arrangementSection
                                        //, ArrangerIDs::arrangementSectionIndex, sectionId
                                        , ArrangerIDs::lengthInBars, sMinLengthInBars
                                        , ArrangerIDs::playbackMode, static_cast<int> (PlaybackMode::loop)) };

    const auto tracks { te::getAudioTracks (edit) };

    // 0th audio track is hidden for chord detection
    for (int trackId { 1 }; trackId < tracks.size(); ++trackId)
    {
        valueTree.appendChild (ArrangementPattern::createArrangementPattern (te::EditItemID::fromVar (tracks[trackId]->state.getProperty (te::IDs::id)))
                             , nullptr);
    }

    return valueTree;
}

ArrangementPatternList& ArrangementSection::getAllPatterns() noexcept
{
    return *mPatterns;
}

int ArrangementSection::getLengthInBars() const noexcept
{
    return mLengthInBars;
}

void ArrangementSection::setLengthInBars (int newLengthInBars, juce::UndoManager* undoManager)
{
    if (mLengthInBars != newLengthInBars)
    {
        mState.setProperty (ArrangerIDs::lengthInBars, newLengthInBars, undoManager);
        mLengthInBars = newLengthInBars;
    }
}

ArrangementSection::PlaybackMode ArrangementSection::getPlaybackMode() const noexcept
{
    return mPlaybackMode;
}

void ArrangementSection::setPlaybackMode (ArrangementSection::PlaybackMode newPlaybackMode, juce::UndoManager* undoManager)
{
    if (mPlaybackMode != newPlaybackMode)
    {
        mState.setProperty (ArrangerIDs::playbackMode, static_cast<int> (newPlaybackMode), undoManager);
        mPlaybackMode = newPlaybackMode;
    }
}

void ArrangementSection::valueTreeChildAdded (juce::ValueTree& parent
                                            , juce::ValueTree& child)
{
    if (te::TrackList::isTrack (child))
    {
        mState.appendChild (ArrangementPattern::createArrangementPattern (te::EditItemID::fromVar (child.getProperty (te::IDs::id)))
                          , nullptr);
    }
}

void ArrangementSection::valueTreeChildRemoved (juce::ValueTree& parent
                                              , juce::ValueTree& child
                                              , int)
{
    if (te::TrackList::isTrack (child))
    {
        mState.removeChild (mState.getChildWithProperty (ArrangerIDs::arrangerTrackId
                                                       , child.getProperty (te::IDs::id))
                           , nullptr);
    }
}

void ArrangementSection::valueTreeChildOrderChanged (juce::ValueTree& parent, int oldIndex, int newIndex)
{
    mState.moveChild (oldIndex, newIndex, nullptr);
}

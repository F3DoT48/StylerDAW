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
    : te::EditItem {te::EditItemID::readOrCreateNewID (edit, valueTree), edit }
    , mState { valueTree }
    , mEdit { edit }
    , mPatterns { std::make_unique<ArrangementPatternList> (mEdit
                                                          , mState
                                                          , nullptr) }
{
    jassert (valueTree.hasType (ArrangerIDs::arrangementSection));

    mSectionName.referTo (mState
                        , ArrangerIDs::arrangementSectionName
                        , undoManager);

    mLengthInBars.referTo (mState
                         , ArrangerIDs::lengthInBars
                         , undoManager);

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

    /*mPatterns = std::make_unique<ArrangementPatternList> (mEdit
                                                        , this
                                                        , mState
                                                        , nullptr);*/

    mEdit.state.addListener (this);
}

ArrangementSection::~ArrangementSection()
{
    mEdit.state.removeListener (this);
}

juce::ValueTree ArrangementSection::createSection (const te::Edit& edit)
{
    auto valueTree { te::createValueTree (ArrangerIDs::arrangementSection
                                        , ArrangerIDs::arrangementSectionName, juce::String ("Arrangement section")
                                        , ArrangerIDs::lengthInBars, sMinLengthInBars
                                        , ArrangerIDs::playbackMode, static_cast<int> (PlaybackMode::loop)) };

    const auto tracks { te::getAudioTracks (edit) };

    // 0th audio track is hidden for chord detection
    for (int trackId { 1 }; trackId < tracks.size(); ++trackId)
    {
        /*valueTree.appendChild (ArrangementPattern::createArrangementPattern (te::EditItemID::fromVar (tracks[trackId]->state.getProperty (te::IDs::id)))
                             , nullptr);*/
        valueTree.appendChild (ArrangementPattern::createArrangementPattern (tracks[trackId]->itemID)
                             , nullptr);
    }

    return valueTree;
}

ArrangementPatternList& ArrangementSection::getAllPatterns()
{
    jassert (mPatterns != nullptr);
    return *mPatterns;
}

juce::String ArrangementSection::getName() const
{
    return mSectionName;
}

void ArrangementSection::setName (const juce::String& name)
{
    auto newName { name.substring (0, 64) };

    if (mSectionName != newName)
        mSectionName = newName;
}

int ArrangementSection::getLengthInBars() const
{
    return mLengthInBars;
}

void ArrangementSection::setLengthInBars (int newLengthInBars, juce::UndoManager* undoManager)
{
    if (mLengthInBars != newLengthInBars)
        mLengthInBars = newLengthInBars;
}

ArrangementSection::PlaybackMode ArrangementSection::getPlaybackMode() const
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
        mState.removeChild (mState.getChildWithProperty (ArrangerIDs::arrangementTrackId
                                                       , child.getProperty (te::IDs::id))
                          , nullptr);
    }
}

void ArrangementSection::valueTreeChildOrderChanged (juce::ValueTree& parent, int oldIndex, int newIndex)
{
    mState.moveChild (oldIndex, newIndex, nullptr);
}

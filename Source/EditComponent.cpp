/*
  ==============================================================================

    EditComponent.cpp
    Created: 2 May 2022 7:50:20pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EditComponent.h"
#include "StylerMainComponent.h"

using namespace styler_app;

//==============================================================================
EditComponent::EditComponent (te::Edit& edit
                            , te::SelectionManager& selectionManager
                            , Arrangement& arrangement
                            , juce::Viewport& arrangementControlsAreaViewport)
    : mEdit { edit }
    , mEditViewState { edit, selectionManager }
    , mArrangement { arrangement }
    , mArrangementControlsAreaViewport { arrangementControlsAreaViewport }
    , mArrangementSectionsArea { std::make_unique<ArrangementSectionsArea> (mArrangement) }
    , mSectionsAreaViewport { std::make_unique<juce::Viewport> ("mSectionsAreaViewport") }
    , mUpdateTracks { false }
{
    mEdit.state.addListener (this);
    mEditViewState.mSelectionManager.addChangeListener (this);

    mSectionsAreaViewport->setScrollBarsShown (false, false, false, false);
    mSectionsAreaViewport->setViewedComponent (mArrangementSectionsArea.get(), false);

    mArrangementControlsAreaViewport.getHorizontalScrollBar().addListener (this);

    addAndMakeVisible (mSectionsAreaViewport.get());

    markAndUpdate (mUpdateTracks);
}

EditComponent::~EditComponent()
{
    mEditViewState.mSelectionManager.removeChangeListener (this);
    mEdit.state.removeListener (this);
    mArrangementControlsAreaViewport.getHorizontalScrollBar().removeListener (this);
}

void EditComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("EditComponent", getLocalBounds(),
                juce::Justification::centred, true);
}

void EditComponent::resized()
{
    auto rec{ getLocalBounds() };

    auto recInputAreas{ rec.removeFromLeft (TrackComponentAttributes::inputsAreaWidthInPixels) };
    auto recMixerControlsAreas{ rec.removeFromRight (TrackComponentAttributes::mixerControlsParametersAreaWidthInPixels) };
    auto recPluginsAreas{ rec.removeFromRight (TrackComponentAttributes::pluginAreaWidthInPixels) };

    jassert (mInputsAreasPermanentTracks.size() == mPluginsAreasPermanentTracks.size()
          && mInputsAreasPermanentTracks.size() == mMixerControlsAreasPermanentTracks.size());

    for (int trackId{ 0 }; trackId < mInputsAreasPermanentTracks.size(); ++trackId)
    {
        mInputsAreasPermanentTracks[trackId]->setBounds (recInputAreas.removeFromTop (TrackComponentAttributes::minimumHeightInPixels));

        mMixerControlsAreasPermanentTracks[trackId]->setBounds (recMixerControlsAreas.removeFromTop (TrackComponentAttributes::minimumHeightInPixels));

        mPluginsAreasPermanentTracks[trackId]->setBounds (recPluginsAreas.removeFromTop (TrackComponentAttributes::minimumHeightInPixels));
    }

    assert (mInputsAreasAudioTracks.size() == mPluginsAreasAudioTracks.size()
         && mInputsAreasAudioTracks.size() == mMixerControlsAreasAudioTracks.size());

    for (int trackId{ 0 }; trackId < mInputsAreasAudioTracks.size(); ++trackId)
    {
        mInputsAreasAudioTracks[trackId]->setBounds (recInputAreas.removeFromTop (TrackComponentAttributes::minimumHeightInPixels));

        mMixerControlsAreasAudioTracks[trackId]->setBounds (recMixerControlsAreas.removeFromTop (TrackComponentAttributes::minimumHeightInPixels));

        mPluginsAreasAudioTracks[trackId]->setBounds (recPluginsAreas.removeFromTop (TrackComponentAttributes::minimumHeightInPixels));
    }

    for (auto inputsArea : mInputsAreasPermanentTracks)
    {
        inputsArea->resized();
    }

    for (auto pluginsArea : mPluginsAreasPermanentTracks)
    {
        pluginsArea->resized();
    }

    for (auto mixerControlsArea : mMixerControlsAreasPermanentTracks)
    {
        mixerControlsArea->resized();
    }

    for (auto inputsArea : mInputsAreasAudioTracks)
    {
        inputsArea->resized();
    }

    for (auto pluginsArea : mPluginsAreasAudioTracks)
    {
        pluginsArea->resized();
    }

    for (auto mixerControlsArea : mMixerControlsAreasAudioTracks)
    {
        mixerControlsArea->resized();
    }

    auto rectForArrangementSections { getLocalBounds() };
    rectForArrangementSections.setLeft (recInputAreas.getRight());
    rectForArrangementSections.setRight (recPluginsAreas.getX());
    rectForArrangementSections.setBottom (recPluginsAreas.getBottom());

    mArrangementSectionsArea->setBounds (rectForArrangementSections.getX()
                                       , rectForArrangementSections.getY()
                                       , mArrangement.getAllSections().size()
                                       * ArrangementSectionComponent::sWidthInPixels
                                       , rectForArrangementSections.getHeight());

    mSectionsAreaViewport->setBounds (rectForArrangementSections);
}

EditViewState& EditComponent::getEditViewState()
{
    return mEditViewState;
}

juce::Viewport* EditComponent::getSectionsAreaViewport() const noexcept
{
    return mSectionsAreaViewport.get();
}

void EditComponent::buildTracks()
{
    mInputsAreasPermanentTracks.clear();
    mPluginsAreasPermanentTracks.clear();
    mMixerControlsAreasPermanentTracks.clear();

    mInputsAreasAudioTracks.clear();
    mPluginsAreasAudioTracks.clear();
    mMixerControlsAreasAudioTracks.clear();

    InputsAreaAudioTrackComponent* tmpInputsAreaAudioTrack{ nullptr };
    PluginsAreaAudioTrackComponent* tmpPluginsAreaAudioTrack{ nullptr };
    MixerControlsAreaAudioTrackComponent* tmpMixerControlsAreaAudioTrack{ nullptr };

    int currentAudioTrackId {0};
    
    for (auto track : getAllTracks (mEdit))
    {
        if (track->isMasterTrack())
        {
            InputsAreaMasterTrackComponent* tmpInputsAreaMasterTrack{ new  InputsAreaMasterTrackComponent (mEditViewState, track) };
            mInputsAreasPermanentTracks.add (tmpInputsAreaMasterTrack);
            addAndMakeVisible (tmpInputsAreaMasterTrack);

            PluginsAreaMasterTrackComponent* tmpPluginsAreaMasterTrack{ new  PluginsAreaMasterTrackComponent (mEditViewState, track) };
            mPluginsAreasPermanentTracks.add (tmpPluginsAreaMasterTrack);
            addAndMakeVisible (tmpPluginsAreaMasterTrack);

            MixerControlsAreaMasterTrackComponent* tmpMixerControlsAreaMasterTrack{ new  MixerControlsAreaMasterTrackComponent (mEditViewState, track) };
            mMixerControlsAreasPermanentTracks.add (tmpMixerControlsAreaMasterTrack);
            addAndMakeVisible (tmpMixerControlsAreaMasterTrack);
        }
        else if (track->isTempoTrack())
        {
            InputsAreaTempoTrackComponent* tmpInputsAreaTempoTrack{ new  InputsAreaTempoTrackComponent (mEditViewState, track) };
            mInputsAreasPermanentTracks.add (tmpInputsAreaTempoTrack);
            addAndMakeVisible (tmpInputsAreaTempoTrack);

            PluginsAreaTempoTrackComponent* tmpPluginsAreaTempoTrack{ new  PluginsAreaTempoTrackComponent (mEditViewState, track) };
            mPluginsAreasPermanentTracks.add (tmpPluginsAreaTempoTrack);
            addAndMakeVisible (tmpPluginsAreaTempoTrack);

            MixerControlsAreaTempoTrackComponent* tmpMixerControlsAreaTempoTrack{ new  MixerControlsAreaTempoTrackComponent (mEditViewState, track) };
            mMixerControlsAreasPermanentTracks.add (tmpMixerControlsAreaTempoTrack);
            addAndMakeVisible (tmpMixerControlsAreaTempoTrack);
        }
        else if (track->isChordTrack())
        {
            InputsAreaChordTrackComponent* tmpInputsAreaChordTrack{ new  InputsAreaChordTrackComponent (mEditViewState, track) };
            mInputsAreasPermanentTracks.add (tmpInputsAreaChordTrack);
            addAndMakeVisible (tmpInputsAreaChordTrack);

            PluginsAreaChordTrackComponent* tmpPluginsAreaChordTrack{ new  PluginsAreaChordTrackComponent (mEditViewState, track) };
            mPluginsAreasPermanentTracks.add (tmpPluginsAreaChordTrack);
            addAndMakeVisible (tmpPluginsAreaChordTrack);

            MixerControlsAreaChordTrackComponent* tmpMixerControlsAreaChordTrack{ new  MixerControlsAreaChordTrackComponent (mEditViewState, track) };
            mMixerControlsAreasPermanentTracks.add (tmpMixerControlsAreaChordTrack);
            addAndMakeVisible (tmpMixerControlsAreaChordTrack);
        }
        else if (track->isAudioTrack())
        {
            if (currentAudioTrackId == 0)
            {
                // create audio track components for chord detection functionality
            }
            else
            {
                tmpInputsAreaAudioTrack = new InputsAreaAudioTrackComponent (mEditViewState, track);
                if (tmpInputsAreaAudioTrack != nullptr)
                {
                    mInputsAreasAudioTracks.add (tmpInputsAreaAudioTrack);
                    addAndMakeVisible (tmpInputsAreaAudioTrack);
                }

                tmpPluginsAreaAudioTrack = new PluginsAreaAudioTrackComponent (mEditViewState, track);
                if (tmpPluginsAreaAudioTrack != nullptr)
                {
                    mPluginsAreasAudioTracks.add (tmpPluginsAreaAudioTrack);
                    addAndMakeVisible (tmpPluginsAreaAudioTrack);
                }
            
                tmpMixerControlsAreaAudioTrack = new MixerControlsAreaAudioTrackComponent (mEditViewState, track);
                if (tmpMixerControlsAreaAudioTrack != nullptr)
                {
                    mMixerControlsAreasAudioTracks.add (tmpMixerControlsAreaAudioTrack);
                    addAndMakeVisible (tmpMixerControlsAreaAudioTrack);
                }
            }
            ++currentAudioTrackId;
        }

        tmpInputsAreaAudioTrack = nullptr;
        tmpPluginsAreaAudioTrack = nullptr;
        tmpMixerControlsAreaAudioTrack = nullptr;
    }

    resized();
}


void EditComponent::valueTreePropertyChanged (juce::ValueTree& tree, const juce::Identifier& property)
{
    if (te::TrackList::isTrack (tree))
    {
        markAndUpdate (mUpdateTracks);
    }
}

void EditComponent::valueTreeChildAdded (juce::ValueTree&, juce::ValueTree& child)
{
    if (te::TrackList::isTrack (child))
    {
        markAndUpdate (mUpdateTracks);
        auto rect { getBounds() };
        rect.setBottom (rect.getBottom() 
                      + TrackComponentAttributes::minimumHeightInPixels);
        setBounds (rect);
        findParentComponentOfClass<StylerMainComponent>()->getEditComponentViewport()->setViewPositionProportionately (0.0, 1.0);
    }
}


void EditComponent::valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree& child, int)
{
    if (te::TrackList::isTrack (child))
    {
        markAndUpdate (mUpdateTracks);
        auto rect { getBounds() };
        rect.setBottom (rect.getBottom() 
                      - TrackComponentAttributes::minimumHeightInPixels);
        setBounds (rect);
    }
}

void EditComponent::handleAsyncUpdate()
{
    if (compareAndReset (mUpdateTracks))
    {
        buildTracks();
    }
}

void EditComponent::changeListenerCallback (juce::ChangeBroadcaster*)
{
    repaint();     
}

void EditComponent::scrollBarMoved (juce::ScrollBar*, double newRangeStart)
{
    mSectionsAreaViewport->getHorizontalScrollBar().setCurrentRangeStart (newRangeStart);
}

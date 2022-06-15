/*
  ==============================================================================

    EditComponent.cpp
    Created: 2 May 2022 7:50:20pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EditComponent.h"
#include "TrackComponentArranger.h"
#include "TrackComponentChord.h"
#include "TrackComponentMarker.h"
#include "TrackComponentMaster.h"
#include "TrackComponentTempo.h"
#include "InputsAreaComponent.h"

using namespace styler_app;

//==============================================================================
EditComponent::EditComponent (te::Edit& edit
                            , te::SelectionManager& selectionManager)
    : mEdit{ edit }
    , mEditViewState{ edit, selectionManager}
    , mNewTrackButton{ std::make_unique<juce::TextButton> ("Add new track") }
{
    mEdit.state.addListener (this);
    mEditViewState.mSelectionManager.addChangeListener (this);

    addAndMakeVisible (mNewTrackButton.get());
    setupNewTrackButton();

    markAndUpdate (mUpdateTracks);
}

EditComponent::~EditComponent()
{
    mEditViewState.mSelectionManager.removeChangeListener (this);
    mEdit.state.removeListener (this);
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
    int verticalOffset {0};

    for (auto track : mAlwaysPresentTracks)
    {
        track->setBounds (0, verticalOffset, getWidth(), TrackComponentAttributes::minimumHeightInPixels);
        verticalOffset += TrackComponentAttributes::minimumHeightInPixels + TrackComponentAttributes::trackGapInPixels;
    }

    jassert (mTrackInputsAreas.size () != mTrackMixerControlsAreas.size ()
          && mTrackInputsAreas.size () != mTrackPluginsAreas.size ());

    for (int trackId{ 0 }; trackId < mTrackInputsAreas.size(); ++trackId)
    {
        mTrackInputsAreas[trackId]->setBounds (0
                                             , verticalOffset
                                             , TrackComponentAttributes::inputsAreaWidthInPixels
                                             , TrackComponentAttributes::minimumHeightInPixels);

        mTrackMixerControlsAreas[trackId]->setBounds (getWidth()
                                                    - TrackComponentAttributes::mixerControlsParametersAreaWidthInPixels
                                                    , verticalOffset
                                                    , TrackComponentAttributes::mixerControlsParametersAreaWidthInPixels
                                                    , TrackComponentAttributes::minimumHeightInPixels);

        mTrackPluginsAreas[trackId]->setBounds (getWidth()
                                              - TrackComponentAttributes::mixerControlsParametersAreaWidthInPixels
                                              - TrackComponentAttributes::pluginAreaWidthInPixels
                                              , verticalOffset
                                              , TrackComponentAttributes::pluginAreaWidthInPixels
                                              , TrackComponentAttributes::minimumHeightInPixels);

        verticalOffset += TrackComponentAttributes::minimumHeightInPixels + TrackComponentAttributes::trackGapInPixels;
    }

    for (auto track : mAlwaysPresentTracks)
    {
        track->resized();
    }

    for (auto inputsArea : mTrackInputsAreas)
    {
        inputsArea->resized();
    }

    for (auto pluginsArea : mTrackPluginsAreas)
    {
        pluginsArea->resized();
    }

    for (auto mixerControlsArea : mTrackMixerControlsAreas)
    {
        mixerControlsArea->resized();
    }

    mNewTrackButton->setBounds (getWidth() - TrackComponentAttributes::newTrackButtonOffsetFromRight
                              , verticalOffset
                              , mNewTrackButton->getLookAndFeel().getTextButtonWidthToFitText (*mNewTrackButton
                                                                                             , TrackComponentAttributes::newTrackButtonHeight)
                              , TrackComponentAttributes::newTrackButtonHeight);
}

EditViewState& EditComponent::getEditViewState()
{
    return mEditViewState;
}

void EditComponent::buildTracks()
{
    mAlwaysPresentTracks.clear();
    TrackComponent* tmpTrackComponent{ nullptr };

    mTrackInputsAreas.clear();
    InputsAreaComponent* tmpInputsAreaComponent{ nullptr };

    mTrackPluginsAreas.clear();
    PluginsAreaComponent* tmpPluginsAreaComponent{ nullptr };

    mTrackMixerControlsAreas.clear();
    MixerControlsAreaComponent* tmpMixerControlsAreaComponent{ nullptr };

    
    for (auto track : getAllTracks (mEdit))
    {
        if (track->isMasterTrack())
        {
            tmpTrackComponent = new TrackComponentMaster (mEditViewState, track);
        }
        /*else if (track->isArrangerTrack())
        {
            tmpTrackComponent = new TrackComponentArranger (mEditViewState, track);
        }*/
        else if (track->isTempoTrack())
        {
            tmpTrackComponent = new TrackComponentTempo (mEditViewState, track);
        }
        /*else if (track->isMarkerTrack())
        {
            tmpTrackComponent = new TrackComponentMarker (mEditViewState, track);
        }*/
        else if (track->isChordTrack())
        {
            tmpTrackComponent = new TrackComponentChord (mEditViewState, track);
        }
        else if (track->isAudioTrack())
        {
            tmpInputsAreaComponent = new InputsAreaComponent (mEditViewState, track);
            tmpPluginsAreaComponent = new PluginsAreaComponent (mEditViewState, track);
            tmpMixerControlsAreaComponent = new MixerControlsAreaComponent (mEditViewState, track);
        }

        if (tmpTrackComponent != nullptr)
        {
            mAlwaysPresentTracks.add (tmpTrackComponent);
            addAndMakeVisible (tmpTrackComponent);
        }

        tmpTrackComponent = nullptr;
        
        if (tmpInputsAreaComponent != nullptr)
        {
            mTrackInputsAreas.add (tmpInputsAreaComponent);
            addAndMakeVisible (tmpInputsAreaComponent);
        }
        
        tmpInputsAreaComponent = nullptr;

        if (tmpPluginsAreaComponent != nullptr)
        {
            mTrackPluginsAreas.add (tmpPluginsAreaComponent);
            addAndMakeVisible (tmpPluginsAreaComponent);
        }

        tmpPluginsAreaComponent = nullptr;

        if (tmpMixerControlsAreaComponent != nullptr)
        {
            mTrackMixerControlsAreas.add (tmpMixerControlsAreaComponent);
            addAndMakeVisible (tmpMixerControlsAreaComponent);
        }

        tmpMixerControlsAreaComponent = nullptr;

    }

    resized();
}

void EditComponent::valueTreeChildAdded (juce::ValueTree&, juce::ValueTree& child)
{
    if (te::TrackList::isTrack (child))
    {
        markAndUpdate (mUpdateTracks);
    }
}


void EditComponent::valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree& child, int)
{
    if (te::TrackList::isTrack (child))
    {
        markAndUpdate (mUpdateTracks);
    }
}

void EditComponent::handleAsyncUpdate ()
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

void EditComponent::setupNewTrackButton ()
{
    mNewTrackButton->onClick = [this]
    {
        mEdit.ensureNumberOfAudioTracks (getAudioTracks (mEdit).size() + 1);
        auto rect{ getBounds() };
        rect.setBottom (rect.getBottom() 
                      + TrackComponentAttributes::trackGapInPixels
                      + TrackComponentAttributes::minimumHeightInPixels);
        setBounds (rect);        
    };
}
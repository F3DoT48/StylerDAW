/*
  ==============================================================================

    EditComponent.cpp
    Created: 2 May 2022 7:50:20pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "EditComponent.h"

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
    auto rec{ getLocalBounds() };
    
    auto recInputAreas{ rec.removeFromLeft (TrackComponentAttributes::inputsAreaWidthInPixels) };
    auto recMixerControlsAreas{ rec.removeFromRight (TrackComponentAttributes::mixerControlsParametersAreaWidthInPixels) };
    auto recPluginsAreas{ rec.removeFromRight (TrackComponentAttributes::pluginAreaWidthInPixels) };

    jassert (mInputsAreasPermanentTracks.size() == mPluginsAreasPermanentTracks.size()
          && mInputsAreasPermanentTracks.size() == mMixerControlsAreasPermanentTracks.size());

    for (int trackId{ 0 }; trackId < mInputsAreasPermanentTracks.size(); ++trackId)
    {
        mInputsAreasPermanentTracks[trackId]->setBounds (recInputAreas.removeFromTop (TrackComponentAttributes::minimumHeightInPixels
                                                                                    + TrackComponentAttributes::trackGapInPixels));

        mMixerControlsAreasPermanentTracks[trackId]->setBounds (recMixerControlsAreas.removeFromTop (TrackComponentAttributes::minimumHeightInPixels
                                                                                                   + TrackComponentAttributes::trackGapInPixels));

        mPluginsAreasPermanentTracks[trackId]->setBounds (recPluginsAreas.removeFromTop (TrackComponentAttributes::minimumHeightInPixels
                                                                                       + TrackComponentAttributes::trackGapInPixels));
    }

    assert (mInputsAreasAudioTracks.size() == mPluginsAreasAudioTracks.size()
         && mInputsAreasAudioTracks.size() == mMixerControlsAreasAudioTracks.size());

    for (int trackId{ 0 }; trackId < mInputsAreasAudioTracks.size(); ++trackId)
    {
        mInputsAreasAudioTracks[trackId]->setBounds (recInputAreas.removeFromTop (TrackComponentAttributes::minimumHeightInPixels
                                                                                + TrackComponentAttributes::trackGapInPixels));

        mMixerControlsAreasAudioTracks[trackId]->setBounds (recMixerControlsAreas.removeFromTop (TrackComponentAttributes::minimumHeightInPixels
                                                                                               + TrackComponentAttributes::trackGapInPixels));

        mPluginsAreasAudioTracks[trackId]->setBounds (recPluginsAreas.removeFromTop (TrackComponentAttributes::minimumHeightInPixels
                                                                                   + TrackComponentAttributes::trackGapInPixels));
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

    const int buttonOffsetVertical{ (TrackComponentAttributes::minimumHeightInPixels
                                   + TrackComponentAttributes::trackGapInPixels)
                                  * (mPluginsAreasAudioTracks.size()
                                   + mPluginsAreasPermanentTracks.size())};

    mNewTrackButton->setBounds (getWidth() - TrackComponentAttributes::newTrackButtonOffsetFromRight
                              , buttonOffsetVertical
                              , mNewTrackButton->getLookAndFeel().getTextButtonWidthToFitText (*mNewTrackButton
                                                                                             , TrackComponentAttributes::newTrackButtonHeight)
                              , TrackComponentAttributes::newTrackButtonHeight);
    mNewTrackButton->resized();
}

EditViewState& EditComponent::getEditViewState()
{
    return mEditViewState;
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
        else if (track->isAudioTrack ())
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

        tmpInputsAreaAudioTrack = nullptr;
        tmpPluginsAreaAudioTrack = nullptr;
        tmpMixerControlsAreaAudioTrack = nullptr;
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

void EditComponent::setupNewTrackButton()
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
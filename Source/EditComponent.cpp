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
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("EditComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void EditComponent::resized()
{
    const int trackGap{ 2 };
    int verticalOffset {100};
    for (auto track : mTracks)
    {
        track->setBounds (0, verticalOffset, getWidth(), track->getAttributes().heightInPixels);
        verticalOffset += track->getAttributes().heightInPixels + trackGap;
    }

    for (auto track : mTracks)
    {
        track->resized();
    }

    const int buttonOffsetFromRight{ 150 };
    const int buttonHeight{ 30 };

    mNewTrackButton->setBounds (getWidth() - buttonOffsetFromRight
                              , verticalOffset
                              , mNewTrackButton->getLookAndFeel().getTextButtonWidthToFitText (*mNewTrackButton
                                                                                             , buttonHeight)
                              , buttonHeight);

}

EditViewState& EditComponent::getEditViewState()
{
    return mEditViewState;
}

void EditComponent::buildTracks ()
{
    mTracks.clear();
    TrackComponent* tmpTrackComponent{ nullptr };

    for (auto track : getAllTracks (mEdit))
    {
        if (track->isMasterTrack ())
        {
            tmpTrackComponent = new TrackComponent (mEditViewState, track); // MasterTrackComponent?
        }
        else
        {
            tmpTrackComponent = new TrackComponent (mEditViewState, track);
        }

        if (tmpTrackComponent != nullptr)
        {
            mTracks.add (tmpTrackComponent);
            addAndMakeVisible (tmpTrackComponent);
        }
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
    };
}
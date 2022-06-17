/*
  ==============================================================================

    MixerControlsAreaTempoTrackComponent.cpp
    Created: 16 Jun 2022 3:50:06pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MixerControlsAreaTempoTrackComponent.h"

using namespace styler_app;

//==============================================================================
MixerControlsAreaTempoTrackComponent::MixerControlsAreaTempoTrackComponent(EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent (editViewState, track)
{

}

MixerControlsAreaTempoTrackComponent::~MixerControlsAreaTempoTrackComponent()
{

}

void MixerControlsAreaTempoTrackComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText (mTrack->getName() + " MixerControlsArea"
                , getLocalBounds()
                , juce::Justification::centred
                , true);
}

void MixerControlsAreaTempoTrackComponent::resized()
{

}

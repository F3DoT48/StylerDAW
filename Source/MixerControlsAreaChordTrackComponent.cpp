/*
  ==============================================================================

    MixerControlsAreaChordTrackComponent.cpp
    Created: 16 Jun 2022 3:49:31pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MixerControlsAreaChordTrackComponent.h"

using namespace styler_app;

//==============================================================================
MixerControlsAreaChordTrackComponent::MixerControlsAreaChordTrackComponent(EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent (editViewState, track)
{

}

MixerControlsAreaChordTrackComponent::~MixerControlsAreaChordTrackComponent()
{

}

void MixerControlsAreaChordTrackComponent::paint (juce::Graphics& g)
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

void MixerControlsAreaChordTrackComponent::resized()
{

}

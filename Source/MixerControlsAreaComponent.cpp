/*
  ==============================================================================

    MixerControlsAreaComponent.cpp
    Created: 4 Jun 2022 10:37:55pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MixerControlsAreaComponent.h"

using namespace styler_app;

//==============================================================================
MixerControlsAreaComponent::MixerControlsAreaComponent(EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent (editViewState, track)
{

}

MixerControlsAreaComponent::~MixerControlsAreaComponent()
{

}

void MixerControlsAreaComponent::paint (juce::Graphics& g)
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

void MixerControlsAreaComponent::resized()
{

}

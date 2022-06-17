/*
  ==============================================================================

    InputsAreaTempoTrackComponent.cpp
    Created: 16 Jun 2022 3:52:43pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InputsAreaTempoTrackComponent.h"

using namespace styler_app;

//==============================================================================

InputsAreaTempoTrackComponent::InputsAreaTempoTrackComponent (EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent (editViewState, track)
{

}

InputsAreaTempoTrackComponent::~InputsAreaTempoTrackComponent()
{

}

void InputsAreaTempoTrackComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText (mTrack->getName() + " InputsArea"
                , getLocalBounds()
                , juce::Justification::centred
                , true);
}

void InputsAreaTempoTrackComponent::resized()
{

}

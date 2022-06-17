/*
  ==============================================================================

    InputsAreaMasterTrackComponent.cpp
    Created: 16 Jun 2022 3:51:46pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InputsAreaMasterTrackComponent.h"

using namespace styler_app;

//==============================================================================

InputsAreaMasterTrackComponent::InputsAreaMasterTrackComponent (EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent (editViewState, track)
{

}

InputsAreaMasterTrackComponent::~InputsAreaMasterTrackComponent()
{

}

void InputsAreaMasterTrackComponent::paint (juce::Graphics& g)
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

void InputsAreaMasterTrackComponent::resized()
{

}

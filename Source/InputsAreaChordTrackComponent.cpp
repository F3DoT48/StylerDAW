/*
  ==============================================================================

    InputsAreaChordTrackComponent.cpp
    Created: 16 Jun 2022 3:52:26pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InputsAreaChordTrackComponent.h"

using namespace styler_app;

//==============================================================================

InputsAreaChordTrackComponent::InputsAreaChordTrackComponent (EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent (editViewState, track)
{
   
}

InputsAreaChordTrackComponent::~InputsAreaChordTrackComponent()
{

}

void InputsAreaChordTrackComponent::paint (juce::Graphics& g)
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

void InputsAreaChordTrackComponent::resized()
{

}


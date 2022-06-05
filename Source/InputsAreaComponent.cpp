/*
  ==============================================================================

    InputsAreaComponent.cpp
    Created: 4 Jun 2022 10:36:47pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InputsAreaComponent.h"

using namespace styler_app;

//==============================================================================

styler_app::InputsAreaComponent::InputsAreaComponent (EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent (editViewState, track)
{

}

InputsAreaComponent::~InputsAreaComponent()
{
}

void InputsAreaComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText (mTrack->getName()
              , getLocalBounds()
              , juce::Justification::centred
              , true);
}

void InputsAreaComponent::resized()
{



}

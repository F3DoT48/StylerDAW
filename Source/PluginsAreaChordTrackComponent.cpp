/*
  ==============================================================================

    PluginsAreaChordTrackComponent.cpp
    Created: 16 Jun 2022 3:54:00pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginsAreaChordTrackComponent.h"

using namespace styler_app;

//==============================================================================
PluginsAreaChordTrackComponent::PluginsAreaChordTrackComponent(EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent (editViewState, track)
{

}

PluginsAreaChordTrackComponent::~PluginsAreaChordTrackComponent()
{
}

void PluginsAreaChordTrackComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText (mTrack->getName() + " PluginsArea"
                , getLocalBounds()
                , juce::Justification::centred
                , true);
}

void PluginsAreaChordTrackComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

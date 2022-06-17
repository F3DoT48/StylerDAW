/*
  ==============================================================================

    PluginsAreaTempoTrackComponent.cpp
    Created: 16 Jun 2022 3:53:24pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginsAreaTempoTrackComponent.h"

using namespace styler_app;

//==============================================================================
PluginsAreaTempoTrackComponent::PluginsAreaTempoTrackComponent(EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent (editViewState, track)
{

}

PluginsAreaTempoTrackComponent::~PluginsAreaTempoTrackComponent()
{
}

void PluginsAreaTempoTrackComponent::paint (juce::Graphics& g)
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

void PluginsAreaTempoTrackComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

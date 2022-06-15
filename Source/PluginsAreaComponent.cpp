/*
  ==============================================================================

    PluginsAreaComponent.cpp
    Created: 4 Jun 2022 10:37:39pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginsAreaComponent.h"

using namespace styler_app;

//==============================================================================
PluginsAreaComponent::PluginsAreaComponent(EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent (editViewState, track)
{
    
}

PluginsAreaComponent::~PluginsAreaComponent()
{
}

void PluginsAreaComponent::paint (juce::Graphics& g)
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

void PluginsAreaComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

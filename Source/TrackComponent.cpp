/*
  ==============================================================================

    TrackComponent.cpp
    Created: 2 May 2022 8:26:17pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TrackComponent.h"

using namespace styler_app;

//==============================================================================
TrackComponent::TrackComponent (EditViewState& editViewState, te::Track::Ptr track)
    : mEditViewState{ editViewState }
    , mTrack{ track }
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

TrackComponent::~TrackComponent()
{
}

void TrackComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText (mTrack->getName()
              , getLocalBounds ()
              , juce::Justification::centred
              , true);
}

void TrackComponent::resized()
{

}

te::Track::Ptr TrackComponent::getTrack() noexcept
{
    return mTrack;
}
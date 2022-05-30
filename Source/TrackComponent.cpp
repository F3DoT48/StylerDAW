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
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText (mTrack->getName(), getLocalBounds (),
                juce::Justification::centred, true);   // draw some placeholder text
}

void TrackComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

TrackComponentAttributes TrackComponent::getAttributes () noexcept
{
    return {};
}

te::Track::Ptr TrackComponent::getTrack () noexcept
{
    return mTrack;
}
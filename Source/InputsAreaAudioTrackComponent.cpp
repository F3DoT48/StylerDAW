/*
  ==============================================================================

    InputsAreaComponent.cpp
    Created: 4 Jun 2022 10:36:47pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "InputsAreaAudioTrackComponent.h"

using namespace styler_app;

//==============================================================================

InputsAreaAudioTrackComponent::InputsAreaAudioTrackComponent (EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent (editViewState, track)
    , mDeleteTrackButton ("Delete track")
{
    mDeleteTrackButton.onClick = [this]()
    {
        mEditViewState.mEdit.deleteTrack (getTrack().get());
        auto rect{ getParentComponent()->getBounds() };
        rect.setBottom (rect.getBottom() 
                      - TrackComponentAttributes::trackGapInPixels
                      - TrackComponentAttributes::minimumHeightInPixels);
        getParentComponent()->setBounds (rect);      
    };
    
    addAndMakeVisible (mDeleteTrackButton);
}

InputsAreaAudioTrackComponent::~InputsAreaAudioTrackComponent()
{
}

void InputsAreaAudioTrackComponent::paint (juce::Graphics& g)
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

void InputsAreaAudioTrackComponent::resized()
{
    auto rectangle{ getLocalBounds()};

    mDeleteTrackButton.setBounds (rectangle.removeFromTop(20));
}

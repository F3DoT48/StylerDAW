/*
  ==============================================================================

    PlayStopButtonComponent.cpp
    Created: 20 Sep 2023 1:35:31pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlayStopButtonComponent.h"

using namespace styler_app;

PlayStopButtonComponent::PlayStopButtonComponent (te::Edit& edit)
    : mEdit { edit }
{
    updateButtonText();

    mButton.onClick = [this]()
    {
        auto& transport = mEdit.getTransport();

        if (transport.isPlaying())
        {
            transport.stop (false, false);
            transport.setCurrentPosition (0.0);
        }
        else
            transport.play (false);

        updateButtonText();
    };

    addAndMakeVisible (mButton);
}

PlayStopButtonComponent::~PlayStopButtonComponent()
{
}

void PlayStopButtonComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void PlayStopButtonComponent::resized()
{
    mButton.setBounds (getLocalBounds());
}

void PlayStopButtonComponent::updateButtonText()
{
    auto& transport = mEdit.getTransport();

    if (transport.isPlaying())
        mButton.setButtonText ("Stop");
    else
        mButton.setButtonText ("Play");
}
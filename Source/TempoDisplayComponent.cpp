/*
  ==============================================================================

    TempoDisplayComponent.cpp
    Created: 19 Aug 2023 7:20:08pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TempoDisplayComponent.h"

using namespace styler_app;

TempoDisplayComponent::TempoDisplayComponent (te::TempoSequence& tempoSequence)
    : mSelectorComponent { tempoSequence }
    , mUnitsLabel { "tempoUnits", "bpm" }
{
    addAndMakeVisible (mSelectorComponent);

    mUnitsLabel.setEditable (false);
    mUnitsLabel.setJustificationType (juce::Justification::left);
    mUnitsLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (mUnitsLabel);
}

TempoDisplayComponent::~TempoDisplayComponent()
{
}

void TempoDisplayComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void TempoDisplayComponent::resized()
{
    auto rec { getLocalBounds() };

    mUnitsLabel.setBounds (rec.removeFromRight (mUnitsLabel.getFont().getStringWidth (mUnitsLabel.getText()) + 10));
    mSelectorComponent.setBounds (rec);
}

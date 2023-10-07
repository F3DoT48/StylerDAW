/*
  ==============================================================================

    TimeSignatureDisplayComponent.cpp
    Created: 19 Aug 2023 5:30:29pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "TimeSignatureDisplayComponent.h"

using namespace styler_app;

TimeSignatureDisplayComponent::TimeSignatureDisplayComponent (te::TempoSequence& tempoSequence)
    : mTempoSequence { tempoSequence }
    , mComboBox {}
{
    mComboBox.addItemList (mSignaturesStrings, 1);
    mComboBox.setSelectedId (2);
    mComboBox.onChange = [this]()
    {
        mTempoSequence.getTimeSigs()[0]->setStringTimeSig (mComboBox.getText());
    };

    addAndMakeVisible (mComboBox);
}

TimeSignatureDisplayComponent::~TimeSignatureDisplayComponent()
{
}

void TimeSignatureDisplayComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void TimeSignatureDisplayComponent::resized()
{
    mComboBox.setBounds (getLocalBounds());
}

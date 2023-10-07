/*
  ==============================================================================

    SectionLengthDisplay.cpp
    Created: 4 Sep 2023 7:05:09pm
    Author:  User

  ==============================================================================
*/

#include "SectionLengthDisplay.h"

using namespace styler_app;

SectionLengthDisplay::SectionLengthDisplay (ArrangementSection::Ptr arrangementSection)
    : mArrangementSection { arrangementSection }
    , mLengthLabel { "sectionLength", "Length:" }
    , mSelectorComponent { mArrangementSection }
    , mUnitsLabel { "sectionLengthUnits", "bars" }
{
    mLengthLabel.setEditable (false);
    mLengthLabel.setJustificationType (juce::Justification::left);
    mLengthLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (mLengthLabel);

    addAndMakeVisible (mSelectorComponent);

    mUnitsLabel.setEditable (false);
    mUnitsLabel.setJustificationType (juce::Justification::left);
    mUnitsLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (mUnitsLabel);
}

SectionLengthDisplay::~SectionLengthDisplay()
{

}

void SectionLengthDisplay::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void SectionLengthDisplay::resized()
{
    auto rec { getLocalBounds() };

    mLengthLabel.setBounds (rec.removeFromLeft (mLengthLabel.getFont().getStringWidth (mLengthLabel.getText()) + 10));
    mUnitsLabel.setBounds (rec.removeFromRight (mUnitsLabel.getFont().getStringWidth (mUnitsLabel.getText()) + 10));
    mSelectorComponent.setBounds (rec);
}

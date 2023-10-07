/*
  ==============================================================================

    SnapResolutionSelectorComponent.cpp
    Created: 11 Sep 2023 11:51:50am
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SnapResolutionSelectorComponent.h"

using namespace styler_app;

SnapResolutionSelectorComponent::SnapResolutionSelectorComponent (SnapOption& currentSnapOption)
    : mCurrentSnapOption { currentSnapOption }
    , mSnapLabel { "snapLabel", "Snap:" }
    , mComboBox{}
{
    mSnapLabel.setEditable (false);
    mSnapLabel.setJustificationType (juce::Justification::left);
    mSnapLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (mSnapLabel);

    mComboBox.setJustificationType (juce::Justification::right);
    mComboBox.addItemList (sSnapOptionsNames, 1);
    mComboBox.setSelectedId (3);

    mComboBox.onChange = [this]()
    {
        mCurrentSnapOption = static_cast<SnapOption> (mComboBox.getSelectedId());
    };
    
    addAndMakeVisible (mComboBox);
}

SnapResolutionSelectorComponent::~SnapResolutionSelectorComponent()
{

}

void SnapResolutionSelectorComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void SnapResolutionSelectorComponent::resized()
{
    auto rec { getLocalBounds() };
    const int labelSize { mSnapLabel.getFont().getStringWidth (mSnapLabel.getText())};

    mSnapLabel.setBounds (rec.removeFromLeft (labelSize));

    mComboBox.setBounds (rec.withSizeKeepingCentre (rec.getWidth(), 25));
}

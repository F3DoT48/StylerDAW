/*
  ==============================================================================

    MidiChannelSelectorComponent.cpp
    Created: 11 Sep 2023 12:41:32pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MidiChannelSelectorComponent.h"
#include "ArrangerIDs.h"


using namespace styler_app;

//==============================================================================
MidiChannelSelectorComponent::MidiChannelSelectorComponent (juce::ValueTree& toolBarState
                                                          , ArrangementPattern::Ptr arrangementPattern)
    : mToolBarState { toolBarState }
    , mArrangementPattern { arrangementPattern }
    , mChannelLabel { "channelLabel", "Channel:" }
    , mComboBox {}
{
    mChannelLabel.setEditable (false);
    mChannelLabel.setJustificationType (juce::Justification::left);
    mChannelLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (mChannelLabel);

    mComboBox.setJustificationType (juce::Justification::right);
    for (int channelIndex { 1 }; channelIndex <= 16; ++channelIndex)
    {        
        mComboBox.addItem (juce::String (channelIndex) + ": -", channelIndex);
    }
    updateComboBox();
    mComboBox.setSelectedId (1);
    mComboBox.onChange = [this]()
    {
        const int newChannelIndex { mComboBox.getSelectedId() };
        
        if (newChannelIndex > 0 && newChannelIndex <= 16)
        {
            mToolBarState.setProperty (ArrangerIDs::channelIndex, newChannelIndex, nullptr);
        }
    };

    mArrangementPattern->mState.addListener (this);

    addAndMakeVisible (mComboBox);
}

MidiChannelSelectorComponent::~MidiChannelSelectorComponent()
{
    mArrangementPattern->mState.removeListener (this);
}

void MidiChannelSelectorComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void MidiChannelSelectorComponent::resized()
{
    auto rec { getLocalBounds() };
    const int labelSize { mChannelLabel.getFont().getStringWidth (mChannelLabel.getText()) };
    mChannelLabel.setBounds (rec.removeFromLeft (labelSize));

    updateComboBox();
    mComboBox.setBounds (rec.withSizeKeepingCentre (rec.getWidth(), 25));
    mComboBox.repaint();
}

void MidiChannelSelectorComponent::updateComboBox()
{
    const int prevId { mComboBox.getSelectedId() };
    for (int channelIndex { 1 }; channelIndex <= 16; ++channelIndex)
    {
        juce::String itemText { channelIndex };
        if (mArrangementPattern->getMidiSequenceForChannel (channelIndex)->getNotes().isEmpty())
        {
            itemText += ": -";
        }
        else
        {
            itemText += ": +";
        }
        mComboBox.changeItemText (channelIndex, itemText);
        mComboBox.setSelectedId (prevId);
    }
}

void MidiChannelSelectorComponent::valueTreeChanged()
{
    resized();
}
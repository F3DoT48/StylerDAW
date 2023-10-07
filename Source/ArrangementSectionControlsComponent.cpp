/*
  ==============================================================================

    ArrangmentSectionControlsComponent.cpp
    Created: 26 Nov 2022 9:39:17pm
    Author:  User

  ==============================================================================
*/

#include "ArrangementSectionControlsComponent.h"

using namespace styler_app;

ArrangementSectionControlsComponent::ArrangementSectionControlsComponent (Arrangement& arrangement
                                                                        , ArrangementSection::Ptr section)
    : mArrangement { arrangement }
    , mArrangementSection { section }
    , mNameLabel { "Section name", mArrangementSection->getName() }
    , mDeleteSectionButton { "Delete section" }
    , mTypeSelectorBox{}
    , mLengthDisplay { mArrangementSection }
{
    mNameLabel.setJustificationType (juce::Justification::centred);
    mNameLabel.setEditable (true);
    mNameLabel.onTextChange = [this]()
    {
        mArrangementSection->setName (mNameLabel.getText());
    };

    addAndMakeVisible (mNameLabel);

    mDeleteSectionButton.onClick = [this]()
    {
        mArrangement.removeSection (mArrangementSection.get());
    };

    addAndMakeVisible (mDeleteSectionButton);

    mTypeSelectorBox.setJustificationType (juce::Justification::centred);
    mTypeSelectorBox.addItemList (ArrangementSection::PlaybackModeNames, 1);
    mTypeSelectorBox.setSelectedId (static_cast<int> (mArrangementSection->getPlaybackMode()) + 1);
    mTypeSelectorBox.onChange = [this]()
    {
        mArrangementSection->setPlaybackMode (static_cast<ArrangementSection::PlaybackMode> (mTypeSelectorBox.getSelectedId() - 1), nullptr);
    };

    addAndMakeVisible (mTypeSelectorBox);

    addAndMakeVisible (mLengthDisplay);
}

ArrangementSectionControlsComponent::~ArrangementSectionControlsComponent()
{

}

void ArrangementSectionControlsComponent::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("ArrangementSectionControlsComponent", getLocalBounds(),
                juce::Justification::centred, true);
}

void ArrangementSectionControlsComponent::resized()
{
    auto rec { getLocalBounds()};
    mNameLabel.setBounds (rec.removeFromTop (20));
    mDeleteSectionButton.setBounds (rec.removeFromTop (20));
    mTypeSelectorBox.setBounds (rec.removeFromTop (20));
    mLengthDisplay.setBounds (rec.removeFromTop (20));
}
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
    , mDeleteSectionButton { "Delete section" }
{
    mDeleteSectionButton.onClick = [this]()
    {
        mArrangement.removeSection (mArrangementSection.get());
    };

    addAndMakeVisible (mDeleteSectionButton);
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
    mDeleteSectionButton.setBounds (rec.removeFromTop (20));
}
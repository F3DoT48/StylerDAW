/*
  ==============================================================================

    ArrangementControlsArea.cpp
    Created: 25 Nov 2022 11:41:47am
    Author:  User

  ==============================================================================
*/

#include "ArrangementControlsArea.h"
#include "ArrangerIDs.h"
#include "StylerMainComponent.h"

using namespace styler_app;

ArrangementControlsArea::ArrangementControlsArea (Arrangement& arrangement)
    : mArrangement { arrangement }
    , mUpdateSections { true }
    , mSectionControlsAreas{}
{
    mArrangement.mState.addListener (this);

    markAndUpdate (mUpdateSections);
}

ArrangementControlsArea::~ArrangementControlsArea()
{
    mArrangement.mState.removeListener (this);
}

void ArrangementControlsArea::paint (juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("ArrangementControlsArea", getLocalBounds(),
                juce::Justification::centred, true);
}

void ArrangementControlsArea::resized()
{
    auto rec { getLocalBounds() };

    for (int secId { 0 }; secId < mSectionControlsAreas.size(); ++secId)
    {
        mSectionControlsAreas[secId]->setBounds (rec.removeFromLeft (ArrangementSectionControlsComponent::sWidthInPixels));
        mSectionControlsAreas[secId]->resized();
    }
}

void ArrangementControlsArea::valueTreeChildAdded (juce::ValueTree&, juce::ValueTree& child)
{
    if (child.hasType (ArrangerIDs::arrangementSection))
    {
        markAndUpdate (mUpdateSections);

        auto rect { getBounds() };
        rect.setRight (rect.getRight()
                     + ArrangementSectionControlsComponent::sWidthInPixels);
        setBounds (rect);
        findParentComponentOfClass<StylerMainComponent>()->getArrangementControlsAreaViewport()->setViewPositionProportionately (1.0, 0.0);
    }
}

void ArrangementControlsArea::valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree& child, int)
{
    if (child.hasType (ArrangerIDs::arrangementSection))
    {
        markAndUpdate (mUpdateSections);
        auto rect { getBounds() };
        rect.setRight (rect.getRight()
                     - ArrangementSectionControlsComponent::sWidthInPixels);
        setBounds (rect);
    }
}

void ArrangementControlsArea::handleAsyncUpdate()
{
    if (compareAndReset (mUpdateSections))
    {
        buildSections();
    }
}

void ArrangementControlsArea::changeListenerCallback (juce::ChangeBroadcaster*)
{
    repaint();
}

void ArrangementControlsArea::buildSections()
{
    mSectionControlsAreas.clear();

    if (auto& sections { mArrangement.getAllSections() };
        !sections.isEmpty())
    {
        for (ArrangementSection::Ptr section : sections)
        {
            auto* tmpSectionControlsComponent { new ArrangementSectionControlsComponent (mArrangement, section) };
            mSectionControlsAreas.add (tmpSectionControlsComponent);
            addAndMakeVisible (tmpSectionControlsComponent);
        }
    }

    resized();
}
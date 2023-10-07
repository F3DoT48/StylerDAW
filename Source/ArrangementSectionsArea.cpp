/*
  ==============================================================================

    ArrangementSectionsArea.cpp
    Created: 25 Nov 2022 12:05:35pm
    Author:  User

  ==============================================================================
*/

#include "ArrangementSectionsArea.h"
#include "ArrangerIDs.h"
#include "EditComponent.h"

using namespace styler_app;

ArrangementSectionsArea::ArrangementSectionsArea (Arrangement& arrangement)
    : mArrangement { arrangement }
    , mUpdateSections { false }
{
    mArrangement.mState.addListener (this);

    markAndUpdate (mUpdateSections);
}

ArrangementSectionsArea::~ArrangementSectionsArea()
{
    mArrangement.mState.removeListener (this);
}

void ArrangementSectionsArea::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("ArrangementSectionsArea", getLocalBounds(),
                juce::Justification::centred, true);
}

void ArrangementSectionsArea::resized()
{
    auto rec { getLocalBounds() };

    for (int secId { 0 }; secId < mSections.size(); ++secId)
    {
        mSections[secId]->setBounds (rec.removeFromLeft (ArrangementSectionComponent::sWidthInPixels));
        mSections[secId]->resized();
    }
}

void ArrangementSectionsArea::valueTreeChildAdded (juce::ValueTree&, juce::ValueTree& child)
{
    if (child.hasType (ArrangerIDs::arrangementSection))
    {
        markAndUpdate (mUpdateSections);

        auto rect { getBounds() };
        rect.setRight (rect.getRight()
                     + ArrangementSectionComponent::sWidthInPixels);
        setBounds (rect);
    }
}


void ArrangementSectionsArea::valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree& child, int)
{
    if (child.hasType (ArrangerIDs::arrangementSection))
    {
        markAndUpdate (mUpdateSections);

        auto rect { getBounds() };
        rect.setRight (rect.getRight()
                     - ArrangementSectionComponent::sWidthInPixels);
        setBounds (rect);
    }
}

void ArrangementSectionsArea::handleAsyncUpdate()
{
    if (compareAndReset (mUpdateSections))
    {
        buildSections();
    }
}

void ArrangementSectionsArea::changeListenerCallback (juce::ChangeBroadcaster*)
{
    repaint();
}

void ArrangementSectionsArea::buildSections()
{
    mSections.clear();

    if (auto& sections { mArrangement.getAllSections() };
        !sections.isEmpty())
    {
        for (ArrangementSection::Ptr section : sections)
        {
            auto* tmpSectionComponent { new ArrangementSectionComponent (mArrangement, section) };
            mSections.add (tmpSectionComponent);
            addAndMakeVisible (tmpSectionComponent);
        }
    }

    resized();
}
/*
  ==============================================================================

    ArrangementSectionComponent.cpp
    Created: 11 Apr 2023 8:04:40pm
    Author:  User

  ==============================================================================
*/

#include "ArrangementSectionComponent.h"
#include "TrackComponent.h"
#include "ArrangerIDs.h"

using namespace styler_app;

ArrangementSectionComponent::ArrangementSectionComponent (Arrangement& arrangement
                                                        , ArrangementSection::Ptr section)
    : mArrangement { arrangement }
    , mArrangementSection { section }
{
    mArrangementSection->mState.addListener (this);

    markAndUpdate (mUpdatePatterns);
}

ArrangementSectionComponent::~ArrangementSectionComponent()
{
    mArrangementSection->mState.removeListener (this);
}

void ArrangementSectionComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("ArrangementSectionComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void ArrangementSectionComponent::resized()
{
    auto rec { getLocalBounds()};

    rec.removeFromTop (3 // permanent tracks
                     * (TrackComponentAttributes::minimumHeightInPixels
                      + TrackComponentAttributes::trackGapInPixels));

    for (auto pattern : mPatterns)
    {
        pattern->setBounds (rec.removeFromTop (TrackComponentAttributes::minimumHeightInPixels
                                             + TrackComponentAttributes::trackGapInPixels));
        pattern->resized();
    }
}

void ArrangementSectionComponent::valueTreeChildAdded (juce::ValueTree&, juce::ValueTree& child)
{
    if (child.hasType (ArrangerIDs::arrangementPattern))
    {
        markAndUpdate (mUpdatePatterns);
    }
}

void ArrangementSectionComponent::valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree& child, int)
{
    if (child.hasType (ArrangerIDs::arrangementPattern))
    {
        markAndUpdate (mUpdatePatterns);
    }
}

void ArrangementSectionComponent::handleAsyncUpdate()
{
    if (compareAndReset (mUpdatePatterns))
    {
        buildPatterns();
    }
}

void ArrangementSectionComponent::changeListenerCallback (juce::ChangeBroadcaster*)
{
    repaint();
}

void ArrangementSectionComponent::buildPatterns()
{
    mPatterns.clear();

    for (auto pattern : mArrangementSection->getAllPatterns())
    {
        auto* tmpPatternComponent { new ArrangementPatternComponent () };
        mPatterns.add (tmpPatternComponent);
        addAndMakeVisible (tmpPatternComponent);
    }

    resized();
}
/*
  ==============================================================================

    WireComponent.cpp
    Created: 16 Oct 2022 11:33:33am
    Author:  User

  ==============================================================================
*/

#include "WireComponent.h"

using namespace styler_app;

WireComponent::WireComponent (juce::Point<int> beginInRack
                            , juce::Point<int> endInRack)
    : mBeginInRack { beginInRack }
    , mEndInRack { endInRack }
{
    
}


WireComponent::~WireComponent()
{
}

void WireComponent::paint(juce::Graphics& g)
{
    const juce::Point<int> beginLocal { mBeginInRack.getX() < mEndInRack.getX() ? 0 : getWidth()
                                      , mBeginInRack.getY() < mEndInRack.getY() ? 0 : getHeight() };

    const juce::Point<int> endLocal { mBeginInRack.getX() < mEndInRack.getX() ? getWidth() : 0
                                    , mBeginInRack.getY() < mEndInRack.getY() ? getHeight() : 0 };

    mPath.clear();
    mPath.startNewSubPath (beginLocal.toFloat());
    mPath.lineTo (endLocal.toFloat());

    g.setColour (juce::Colours::orange);
    g.strokePath (mPath, juce::PathStrokeType (2.0f));
}

const juce::Point<int>& WireComponent::getBeginInRack() const
{
    return mBeginInRack;
}

void WireComponent::setBeginInRack (const juce::Point<int>& newBegin)
{
    mBeginInRack = newBegin;
}

const juce::Point<int>& WireComponent::getEndInRack() const
{
    return mEndInRack;
}

void WireComponent::setEndInRack (const juce::Point<int>& newEnd)
{
    mEndInRack = newEnd;
}

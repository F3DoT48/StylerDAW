/*
  ==============================================================================

    PRRulerComponent.cpp
    Created: 9 Sep 2023 4:23:38pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PRRulerComponent.h"

using namespace styler_app;

//==============================================================================
PRRulerComponent::PRRulerComponent (ArrangementSection::Ptr section
                                  , PRToolBarComponent& toolbar
                                  , juce::Viewport& gridViewport)
    : mRulerViewport { std::make_unique<juce::Viewport> ("MeasureRulerViewport") }
    , mMeasureRulerComponent { std::make_unique<MeasureRulerComponent> (section
                                                                      , toolbar) }
    , mPRGridViewport { gridViewport }
{
    mRulerViewport->setViewedComponent (mMeasureRulerComponent.get());
    mRulerViewport->setScrollBarsShown (false, false, false, false);
    addAndMakeVisible (mRulerViewport.get());

    mPRGridViewport.getHorizontalScrollBar().addListener (this);
}

PRRulerComponent::~PRRulerComponent()
{
    mPRGridViewport.getHorizontalScrollBar().removeListener (this);
}

void PRRulerComponent::paint (juce::Graphics&)
{

}

void PRRulerComponent::resized()
{
    mMeasureRulerComponent->setBounds (0
                                     , 0
                                     , mPRGridViewport.getViewedComponent()->getWidth()
                                     , getHeight());

    mRulerViewport->setBounds (getLocalBounds());

    mMeasureRulerComponent->resized();
    mRulerViewport->resized();
}

void PRRulerComponent::scrollBarMoved (juce::ScrollBar*, double newRangeStart)
{
    mRulerViewport->getHorizontalScrollBar().setCurrentRangeStart (newRangeStart);
}
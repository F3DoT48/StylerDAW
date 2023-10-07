/*
  ==============================================================================

    MeasureRulerComponent.cpp
    Created: 9 Sep 2023 5:06:45pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MeasureRulerComponent.h"

using namespace styler_app;

//==============================================================================
MeasureRulerComponent::MeasureRulerComponent (ArrangementSection::Ptr section
                                            , PRToolBarComponent& toolbar)
    : mArrangementSection { section }
    , mToolBar { toolbar }
{
}

MeasureRulerComponent::~MeasureRulerComponent()
{
}

void MeasureRulerComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

    const float barWidth { static_cast<float> (getWidth())
                         / mArrangementSection->getLengthInBars() };
    float currLineX { 0.0f };
    for (int barIndex { 2 }; barIndex <= mArrangementSection->getLengthInBars(); ++barIndex)
    {
        currLineX += barWidth;
        const juce::String barLabel { barIndex };
        /*g.drawVerticalLine (static_cast<int> (currLineX)
                          , 0.0f
                          , static_cast<float> (getHeight()));*/
        g.drawText (barLabel
                  , juce::Rectangle<float> { g.getCurrentFont().getStringWidthFloat (barLabel)
                                           , static_cast<float> (getHeight()) }
                    .withCentre (juce::Point<float> { currLineX
                                        , getHeight() / 2.0f })
                  , juce::Justification::centred
                  , false);
    }
}

void MeasureRulerComponent::resized()
{

}


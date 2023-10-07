/*
  ==============================================================================

    PRNoteEditorComponent.cpp
    Created: 3 May 2023 3:05:05pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PRNoteEditorComponent.h"

using namespace styler_app;

//==============================================================================
PRNoteEditorComponent::PRNoteEditorComponent (ArrangementPattern::Ptr arrangementPattern
                                            , ArrangementSection::Ptr arrangementSection
                                            , PRToolBarComponent& toolBar)
    : mArrangementPattern { arrangementPattern }
    , mArrangementSection { arrangementSection }
    , mToolBar { toolBar }
    , mPRGridViewport { std::make_unique<juce::Viewport> ("PRGridViewport") }
    , mPRKeyboardComponent { std::make_unique<PRKeyboardComponent> (mArrangementPattern, *mPRGridViewport, mToolBar) }
    , mPRGridComponent { std::make_unique<PRGridComponent> (mArrangementPattern
                                                          , mArrangementSection
                                                          , mToolBar
                                                          , mPRKeyboardComponent->getMidiKeyboard()) }
    , mPRRulerComponent { std::make_unique<PRRulerComponent> (mArrangementSection
                                                            , mToolBar
                                                            , *mPRGridViewport) }
{
    addAndMakeVisible (mPRKeyboardComponent.get());

    mPRGridViewport->setViewPositionProportionately (0.0, 0.5);
    mPRGridViewport->setViewedComponent (mPRGridComponent.get());
    addAndMakeVisible (mPRGridViewport.get());
    addAndMakeVisible (mPRRulerComponent.get());
}

PRNoteEditorComponent::~PRNoteEditorComponent()
{

}

void PRNoteEditorComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PRNoteEditorComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PRNoteEditorComponent::resized()
{
    DBG ("PRNoteEditorComponent resized()");
    auto rec { getLocalBounds() };

    const auto recForRuler { rec.removeFromTop (MeasureRulerComponent::sDefaultHeightInPixels)
                                .withWidth (getWidth() 
                                          - sPRKeyboardWidthInPixels
                                          - getLookAndFeel().getDefaultScrollbarWidth())
                                .translated (sPRKeyboardWidthInPixels, 0) };

    mPRKeyboardComponent->setBounds (rec.removeFromLeft (sPRKeyboardWidthInPixels));
    mPRKeyboardComponent->resized();

    const double numPixelsPerBar { static_cast<double> (rec.getWidth() - getLookAndFeel().getDefaultScrollbarWidth())
                                 / mToolBar.getNumBarsInView() };
    const double gridWidth { numPixelsPerBar * mArrangementSection->getLengthInBars() };

    const auto currPosition { mPRGridViewport->getViewPosition() };
    juce::Point<double> currProportion { static_cast<double> (currPosition.getX()) / (mPRGridComponent->getWidth() - mPRGridViewport->getWidth())//mPRGridViewport->getViewWidth())
                                       , static_cast<double> (currPosition.getY()) / (mPRGridComponent->getHeight() - mPRGridViewport->getHeight()) };//mPRGridViewport->getViewHeight()) };

    DBG ("currProportion: " << currProportion.getX() << " " << currProportion.getY());

    // sets the view to the middle vertically upon first opening of the pianoroll window
    if (std::isnan (currProportion.getY()))
    {
        currProportion = juce::Point<double> {0.0, 0.5};
    }
    // =================================================================================

    mPRGridComponent->setBounds (rec.getX()
                               , rec.getY()
                               , static_cast<int> (gridWidth)
                               , static_cast<int> (mPRKeyboardComponent->getKeyboardHeight()));

    mPRGridViewport->setBounds (rec);
    mPRRulerComponent->setBounds (recForRuler);
    mPRRulerComponent->resized();
    mPRGridViewport->setViewPositionProportionately (currProportion.getX(), currProportion.getY());

    
}

/*
  ==============================================================================

    ArrangementPatternComponent.cpp
    Created: 11 Apr 2023 8:05:02pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "ArrangementPatternComponent.h"
#include "PRMainComponent.h"

using namespace styler_app;

ArrangementPatternComponent::ArrangementPatternComponent (ArrangementPattern::Ptr pattern, ArrangementSection::Ptr section)
    : mArrangementPattern { pattern }
    , mArrangementSection { section }
    , mOpenPianoRollButton { "Open Piano Roll" }
    , mPianoRollWindow { new PianoRollWindow ("Piano Roll") }
{
    setupPianoRollButton();

    addAndMakeVisible (mOpenPianoRollButton);
}

ArrangementPatternComponent::~ArrangementPatternComponent()
{
}

void ArrangementPatternComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);

    juce::String componentText { "ArrangementPatternComponent" };
    /*componentText.append (" ", 1);
    componentText.append (mArrangementPattern->getAssociatedArrangementSection()->getName(), 64);
    componentText.append (" ", 1);
    componentText.append (mArrangementPattern->getAssociatedAudioTrack()->getName(), 64);*/

    const int numLines {3};

    g.drawFittedText (componentText
                    , getLocalBounds()
                    , juce::Justification::centred
                    , numLines);
}

void ArrangementPatternComponent::resized()
{
    auto rec { getLocalBounds() };
    mOpenPianoRollButton.setBounds (rec.removeFromTop (sOpenPianoRollButtonHeigthInPixels));
}

void ArrangementPatternComponent::setupPianoRollButton()
{
    mOpenPianoRollButton.onClick = [this]()
    {
        if (mPianoRollWindow->getContentComponent() == nullptr)
        {
            auto* pianoRollMainComponent {new PRMainComponent (mArrangementPattern, mArrangementSection)};
            pianoRollMainComponent->setSize (PRMainComponent::sDefaultWidthInPixels
                                           , PRMainComponent::sDefaultHeightInPixels);

            mPianoRollWindow->setContentOwned (pianoRollMainComponent, true);
            mPianoRollWindow->centreWithSize (800, 600);
            /*mPianoRollWindow->setName (juce::String (mArrangementSection->getName()
                                                   + " "
                                                   + mArrangementPattern->getAssociatedAudioTrack()->getName()
                                                   + " Piano Roll"));*/
        }

        mPianoRollWindow->setVisible (true);
        mPianoRollWindow->toFront (true);
    };
}
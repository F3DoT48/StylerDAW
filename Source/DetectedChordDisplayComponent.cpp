/*
  ==============================================================================

    DetectedChordDisplayComponent.cpp
    Created: 20 Aug 2023 5:39:52pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DetectedChordDisplayComponent.h"

using namespace styler_app;

DetectedChordDisplayComponent::DetectedChordDisplayComponent (Arrangement& arrangement)
    : mArrangement { arrangement }
    , mChordLabel {}
{
    mChordLabel.setEditable (false);
    mChordLabel.setJustificationType (juce::Justification::centred);
    mChordLabel.setColour (juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible (&mChordLabel);

    startTimer (250);
}

DetectedChordDisplayComponent::~DetectedChordDisplayComponent()
{
    stopTimer();
}

void DetectedChordDisplayComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}

void DetectedChordDisplayComponent::resized()
{
    mChordLabel.setBounds (getLocalBounds());
}

juce::String getChordRootSymbol (const Chord& chord)
{
    static juce::StringArray sSymbols ("C"
                                     , "C#/Db"
                                     , "D"
                                     , "D#/Eb"
                                     , "E"
                                     , "F"
                                     , "F#/Gb"
                                     , "G"
                                     , "G#/Ab"
                                     , "A"
                                     , "A#/Bb"
                                     , "B");

    return sSymbols[chord.getRootIndex()];
}

juce::String getChordTypeSymbol (const Chord& chord)
{
    juce::String symbol{};

    if (chord.getType() == Chord::Type::min)
    {
        symbol = "m";
    }

    return symbol;
}

void DetectedChordDisplayComponent::timerCallback()
{
    const auto chord { mArrangement.getPtrToController()->getLastDetectedChord() };

    const juce::String rootSymbol { getChordRootSymbol (chord) };

    const juce::String typeSymbol { getChordTypeSymbol (chord) };

    mChordLabel.setText (rootSymbol + typeSymbol
                       , juce::dontSendNotification);
}

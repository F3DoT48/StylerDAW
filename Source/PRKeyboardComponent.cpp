/*
  ==============================================================================

    PRKeyboardComponent.cpp
    Created: 3 May 2023 8:39:32pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PRKeyboardComponent.h"

using namespace styler_app;

//==============================================================================
PRKeyboardComponent::PRKeyboardComponent (ArrangementPattern::Ptr arrangementPattern
                                        , juce::Viewport& gridViewport
                                        , PRToolBarComponent& toolBar)
    : mArrangementPattern { arrangementPattern }
    , mPRGridViewport { gridViewport }
    , mToolBar { toolBar }
    , mMidiKeyboardViewport { std::make_unique<juce::Viewport> ("MidiKeyboardViewport") }
    , mMidiKeyboardState { }
    , mMidiKeyboardComponent { std::make_unique<juce::MidiKeyboardComponent> (mMidiKeyboardState
                                                                            , juce::KeyboardComponentBase::Orientation::verticalKeyboardFacingRight) }
{
    mMidiKeyboardComponent->setScrollButtonsVisible (false);
    mMidiKeyboardComponent->setBlackNoteWidthProportion (0.6f);

    mMidiKeyboardViewport->setViewedComponent (mMidiKeyboardComponent.get());
    mMidiKeyboardViewport->setScrollBarsShown (false, false, false, false);
    mPRGridViewport.getVerticalScrollBar().addListener (this);
    addAndMakeVisible (mMidiKeyboardViewport.get());

    mMidiKeyboardState.addListener (this);
}

PRKeyboardComponent::~PRKeyboardComponent()
{
    mPRGridViewport.getVerticalScrollBar().removeListener (this);
    mMidiKeyboardState.removeListener (this);
}

void PRKeyboardComponent::paint (juce::Graphics& g)
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
    g.drawText ("PRKeyboardComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PRKeyboardComponent::resized()
{
    const int scrollBarWidth { juce::LookAndFeel::getDefaultLookAndFeel().getDefaultScrollbarWidth() };

    if (const float newWhiteKeyWidth { (getHeight() - scrollBarWidth)
                                     / mToolBar.getNumKeysInView() };
        newWhiteKeyWidth > 0.0f)
        mMidiKeyboardComponent->setKeyWidth (newWhiteKeyWidth);

    mMidiKeyboardComponent->setBounds (0
                                     , 0
                                     , getWidth()
                                     , static_cast<int> (getKeyboardHeight()));

    mMidiKeyboardViewport->setBounds (0
                                    , 0
                                    , getWidth()
                                    , getHeight() - scrollBarWidth);

    mMidiKeyboardComponent->resized();
    mMidiKeyboardViewport->resized();
}

float PRKeyboardComponent::getKeyboardHeight() const
{
    return mMidiKeyboardComponent->getTotalKeyboardWidth();
}

juce::MidiKeyboardComponent& PRKeyboardComponent::getMidiKeyboard() const
{
    return *mMidiKeyboardComponent;
}

void PRKeyboardComponent::scrollBarMoved (juce::ScrollBar*, double newRangeStart)
{
    mMidiKeyboardViewport->getVerticalScrollBar().setCurrentRangeStart (newRangeStart);
}

void PRKeyboardComponent::handleNoteOn (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    DBG ("note on sent");
    mArrangementPattern->getAssociatedAudioTrack()->injectLiveMidiMessage (juce::MidiMessage::noteOn (midiChannel
                                                                                                    , midiNoteNumber
                                                                                                    , velocity)
                                                                         , 0);
}

void PRKeyboardComponent::handleNoteOff (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity)
{
    DBG ("note off sent");
    mArrangementPattern->getAssociatedAudioTrack()->injectLiveMidiMessage (juce::MidiMessage::noteOff (midiChannel
                                                                                                     , midiNoteNumber
                                                                                                     , velocity)
                                                                         , 0);
}
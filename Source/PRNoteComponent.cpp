/*
  ==============================================================================

    PRNoteComponent.cpp
    Created: 26 Jun 2023 7:15:44pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PRNoteComponent.h"
#include "PRGridComponent.h"

using namespace styler_app;

//==============================================================================
PRNoteComponent::PRNoteComponent (ArrangementSection::Ptr arrangemetnSection
                                , ArrangementPattern::Ptr arrangementPattern
                                , MidiNote* midiNote
                                , PRToolBarComponent& toolBar)
    : mArrangementSection { arrangemetnSection }
    , mArrangementPattern { arrangementPattern }
    , mMidiNote { midiNote }
    , mToolBar { toolBar }
    , mStepwiseConstrainer { 1, mToolBar }
    , mResizerComponent {mArrangementSection
                       , mArrangementPattern
                       , *mMidiNote
                       , mToolBar
                       , this
                       , &mStepwiseConstrainer
                       , juce::ResizableEdgeComponent::Edge::rightEdge }
{
    addAndMakeVisible (mResizerComponent);
}

PRNoteComponent::~PRNoteComponent()
{
}

void PRNoteComponent::paint (juce::Graphics& g)
{
    g.setColour (juce::Colours::grey);
    g.drawRoundedRectangle (getLocalBounds().toFloat(), 2.0f, 1.0f);   // draw an outline around the component
    g.setColour (juce::Colours::darkblue);
    g.fillRoundedRectangle (getLocalBounds().toFloat(), 2.0f);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PRNoteComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PRNoteComponent::resized()
{
    const int currBeatsInBar { mArrangementPattern
                             ->getAssociatedAudioTrack()
                             ->edit.tempoSequence.getTimeSigs()[0]->numerator };

    const int numPixelsPerBeat { static_cast<int> (findParentComponentOfClass<PRGridComponent>()->getWidth()
                                                 / mArrangementSection->getLengthInBars()
                                                 / currBeatsInBar) };

    mStepwiseConstrainer.updateNumPixelsPerBeat (numPixelsPerBeat);
    mResizerComponent.setBounds (getLocalBounds().removeFromRight (5));
}

void PRNoteComponent::mouseDown (const juce::MouseEvent& event)
{
    if (event.mods.isRightButtonDown())
    {
        const int currMidiChannel { mToolBar.getCurrentMidiChannel().getChannelNumber() };
        mArrangementPattern->getMidiSequenceForChannel (currMidiChannel)->removeNote (*mMidiNote, nullptr);
    }
}
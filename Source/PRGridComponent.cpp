/*
  ==============================================================================

    PRGridComponent.cpp
    Created: 3 May 2023 7:37:32pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PRGridComponent.h"
#include "PRKeyboardComponent.h"
#include "ArrangerEngine.h"
#include "PRMainComponent.h"

using namespace styler_app;

//==============================================================================
PRGridComponent::PRGridComponent (ArrangementPattern::Ptr arrangementPattern
                                , ArrangementSection::Ptr arrangementSection
                                , PRToolBarComponent& toolBar
                                , juce::MidiKeyboardComponent& keyboard)
    : mArrangementPattern { arrangementPattern }
    , mArrangementSection { arrangementSection }
    , mToolBar { toolBar }
    , mMidiKeyboard { keyboard }
    , mNotes {}
    , mUpdateNotes { true }
{
    //mArrangementPattern->mState.addListener (this);
    mToolBar.mState.addListener (this);
    mArrangementPattern->getAssociatedAudioTrack()->edit.state.addListener (this);

    markAndUpdate (mUpdateNotes);
}

PRGridComponent::~PRGridComponent()
{

    mArrangementPattern->getAssociatedAudioTrack()->edit.state.removeListener (this);
    mToolBar.mState.removeListener (this);
    //mArrangementPattern->mState.removeListener (this);
}

void PRGridComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PRGridComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text

    g.setColour (juce::Colour::fromRGBA (255, 255, 255, 63));

    for (int noteId { mMidiKeyboard.getRangeStart() }; noteId <= mMidiKeyboard.getRangeEnd(); ++noteId)
    {
        if (noteId % 12 == 0  // C
         || noteId % 12 == 5) // F
        {
            g.drawHorizontalLine (getHeight()
                                - static_cast<int> (mMidiKeyboard.getKeyPosition (noteId
                                                                                , mMidiKeyboard.getKeyWidth()).getStart())
                                , 0.0f
                                , static_cast<float> (getWidth()));
        }
        else if (noteId % 12 == 1   // C#
              || noteId % 12 == 3   // D#
              || noteId % 12 == 6   // F#
              || noteId % 12 == 8   // G#
              || noteId % 12 == 10) // A#
        {
            g.drawHorizontalLine (getHeight()
                                - static_cast<int> (mMidiKeyboard.getKeyPosition (noteId
                                                                                , mMidiKeyboard.getKeyWidth()).getStart())
                                , 0.0f
                                , static_cast<float> (getWidth()));
            g.drawHorizontalLine (getHeight()
                                - static_cast<int> (mMidiKeyboard.getKeyPosition (noteId
                                                                                , mMidiKeyboard.getKeyWidth()).getEnd())
                                , 0.0f
                                , static_cast<float> (getWidth()));
        }
    }

    const int tmpMeasureNumerator { mArrangementPattern
                                    ->getAssociatedAudioTrack()
                                    ->edit.tempoSequence.getTimeSigs()[0]->numerator };
    const float beatWidth { static_cast<float> (getWidth()) / mArrangementSection->getLengthInBars() / tmpMeasureNumerator };
    float currLineX { 0.0f };
    for (int beatId { 0 }; beatId < mArrangementSection->getLengthInBars() * tmpMeasureNumerator - 1; ++beatId)
    {
        currLineX += beatWidth;
        g.drawVerticalLine (static_cast<int> (currLineX), 0.0f, static_cast<float> (getHeight()));
    }
}

void PRGridComponent::resized()
{
    for (int noteId {0}; noteId < mNotes.size(); ++noteId)
    {
        const int currBeatsInBar { mArrangementPattern
                                  ->getAssociatedAudioTrack()
                                  ->edit.tempoSequence.getTimeSigs()[0]->numerator };
       
        const int xStart { static_cast<int> (mNotes[noteId]->getMidiNote()->getStartBeat()
                            / (mArrangementSection->getLengthInBars() * currBeatsInBar)
                            * getWidth()) };
        
        const int xLength { static_cast<int> (mNotes[noteId]->getMidiNote()->getLengthInBeats()
                            / (mArrangementSection->getLengthInBars() * currBeatsInBar)
                            * getWidth()) };

        const int currPitchIndex { mNotes[noteId]->getMidiNote()->getNoteIndex() };

        int yStart { 0 };
        int yLength { 0 };

        if (currPitchIndex % 12 == 0  // C
         || currPitchIndex % 12 == 5) // F
        {
            yStart = getHeight() - static_cast<int> (mMidiKeyboard.getKeyPosition (currPitchIndex + 1
                                                                                 , mMidiKeyboard.getKeyWidth()).getStart());

            yLength = getHeight()
                    - static_cast<int> (mMidiKeyboard.getKeyPosition (currPitchIndex
                                                                    , mMidiKeyboard.getKeyWidth()).getStart())
                    - yStart;
        }
        else if (currPitchIndex % 12 == 11 // B
              || currPitchIndex % 12 == 4
              || currPitchIndex == 127) // E
        {
            yStart = getHeight() - static_cast<int> (mMidiKeyboard.getKeyPosition (currPitchIndex
                                                                                 , mMidiKeyboard.getKeyWidth()).getEnd());

            yLength = getHeight()
                    - static_cast<int> (mMidiKeyboard.getKeyPosition (currPitchIndex - 1
                                                                    , mMidiKeyboard.getKeyWidth()).getEnd())
                    - yStart;
        }
        else if (currPitchIndex % 12 == 1   // C#
              || currPitchIndex % 12 == 3   // D#
              || currPitchIndex % 12 == 6   // F#
              || currPitchIndex % 12 == 8   // G#
              || currPitchIndex % 12 == 10) // A#
        {
            yStart = getHeight() - static_cast<int> (mMidiKeyboard.getKeyPosition (currPitchIndex
                                                                                   , mMidiKeyboard.getKeyWidth()).getEnd());
            
            yLength = static_cast<int> (mMidiKeyboard.getKeyPosition (currPitchIndex
                                                                    , mMidiKeyboard.getKeyWidth()).getLength());
        }
        else if (currPitchIndex % 12 == 2  // D
              || currPitchIndex % 12 == 7  // G
              || currPitchIndex % 12 == 9) // A
        {
            yStart = getHeight() - static_cast<int> (mMidiKeyboard.getKeyPosition (currPitchIndex + 1
                                                                                   , mMidiKeyboard.getKeyWidth()).getStart());
            yLength = getHeight() 
                    - static_cast<int> (mMidiKeyboard.getKeyPosition (currPitchIndex - 1 
                                                                    , mMidiKeyboard.getKeyWidth()).getEnd())
                    - yStart;
        }

        DBG ("currPitchIndex = " << currPitchIndex);
        DBG ("startBeat = " << mNotes[noteId]->getMidiNote()->getStartBeat());
        DBG ("lengthInBeats = " << mNotes[noteId]->getMidiNote()->getLengthInBeats());
        DBG ("Ystart " << yStart << " Ylength " << yLength);
        DBG ("Xstart " << xStart << " Xlength " << xLength);

        juce::Rectangle<int> noteRect
        {
            xStart
          , yStart
          , xLength
          , yLength
        };
        
        mNotes[noteId]->setBounds (noteRect);
        mNotes[noteId]->resized();
    }
}

void PRGridComponent::mouseDown (const juce::MouseEvent& event)
{
    if (event.mods.isLeftButtonDown())
    {
        const te::MidiChannel currMidiChannel { mToolBar.getCurrentMidiChannel() };
        const int currBeatsInBar { mArrangementPattern
                                 ->getAssociatedAudioTrack()
                                 ->edit.tempoSequence.getTimeSigs()[0]->numerator };
        const int currTranspositionRule {0};

        const double snapResolution { mToolBar.getSnapResolutionInBeats() };
        const double mouseDownAtBeat { static_cast<double> (event.getMouseDownPosition().getX())
                                     / getWidth()
                                     * mArrangementSection->getLengthInBars()
                                     * currBeatsInBar };

        const double startInBeats { static_cast<int> (mouseDownAtBeat
                                                    / snapResolution)
                                   * snapResolution };

        mArrangementPattern
            ->getMidiSequenceForChannel (currMidiChannel.getChannelNumber())
                ->addNote (startInBeats
                         , 1.0
                         , mMidiKeyboard.getNoteAtPosition ({0.0
                                                          , static_cast<float> (event.getMouseDownPosition().getY())})
                         , 100
                         , false
                         , static_cast<NoteTranspositionRule::Type> (currTranspositionRule)
                         , nullptr);
    }
}

void PRGridComponent::valueTreeChildAdded (juce::ValueTree&, juce::ValueTree& child)
{
    if (child.hasType (ArrangerIDs::note))
    {
        markAndUpdate (mUpdateNotes);
    }
}

void PRGridComponent::valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree& child, int)
{
    if (child.hasType (ArrangerIDs::note))
    {
        markAndUpdate (mUpdateNotes);
    }
}

void PRGridComponent::valueTreePropertyChanged (juce::ValueTree& tree, const juce::Identifier&)
{
    DBG (tree.getType().toString());
    if (tree.hasType (ArrangerIDs::note)
     || tree.hasProperty (ArrangerIDs::channelIndex)
     || tree.hasType (te::IDs::TIMESIG))
    {
        markAndUpdate (mUpdateNotes);
    }
}

void PRGridComponent::handleAsyncUpdate()
{
    if (compareAndReset (mUpdateNotes))
    {
        buildNotes();
    }
}

void PRGridComponent::changeListenerCallback (juce::ChangeBroadcaster*)
{
    repaint();
}

void PRGridComponent::buildNotes()
{
    mNotes.clear();

    const int currMidiChannelIndex { mToolBar.getCurrentMidiChannel().getChannelNumber() };

    for (auto* note : mArrangementPattern->getMidiSequenceForChannel(currMidiChannelIndex)->getNotes())
    {
        auto* tmpNoteComponent { new PRNoteComponent { mArrangementSection, mArrangementPattern, note, mToolBar } };
        mNotes.add (tmpNoteComponent);
        addAndMakeVisible (tmpNoteComponent);
    }

    repaint();
    resized();
}
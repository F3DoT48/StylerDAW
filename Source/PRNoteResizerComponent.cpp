/*
  ==============================================================================

    PRNoteResizerComponent.cpp
    Created: 30 Jun 2023 11:08:17am
    Author:  User

  ==============================================================================
*/

#include "PRNoteResizerComponent.h"
#include "PRGridComponent.h"

using namespace styler_app;

PRNoteResizerComponent::PRNoteResizerComponent (ArrangementSection::Ptr arrangementSection
                                              , ArrangementPattern::Ptr arrangementPattern
                                              , MidiNote& noteToResize
                                              , const PRToolBarComponent& toolBar
                                              , juce::Component* componentToResize
                                              , juce::ComponentBoundsConstrainer* constrainer
                                              , juce::ResizableEdgeComponent::Edge edgeToResize)
    : juce::ResizableEdgeComponent { componentToResize, constrainer, edgeToResize }
    , mArrangementSection { arrangementSection }
    , mArrangementPattern { arrangementPattern }
    , mMidiNote { noteToResize }
    , mToolBar { toolBar }
{

}

void PRNoteResizerComponent::mouseUp (const juce::MouseEvent& event)
{
    juce::ResizableEdgeComponent::mouseUp (event);

    const int extraPixels { event.getOffsetFromDragStart().getX() };
    const int currBeatsInBar { mArrangementPattern
                             ->getAssociatedAudioTrack()
                             ->edit.tempoSequence.getTimeSigs()[0]->numerator };
    const double numPixelsPerBeat { static_cast<double> (findParentComponentOfClass<PRGridComponent>()->getWidth())
                                   / mArrangementSection->getLengthInBars()
                                   / currBeatsInBar };

    double addLengthInBeats (extraPixels
                           / numPixelsPerBeat);

    const int correctionIfShorteningTheNote { extraPixels < 0 ? 1 : 0 };

    addLengthInBeats = (static_cast<int> (addLengthInBeats 
                                        / mToolBar.getSnapResolutionInBeats())
                      - correctionIfShorteningTheNote)
                      * mToolBar.getSnapResolutionInBeats();

    const double newLengthInBeats { juce::jmax (mMidiNote.getLengthInBeats()
                                              + addLengthInBeats
                                              , mToolBar.getSnapResolutionInBeats()) };

    DBG ("newLengthInBeats: " << newLengthInBeats);

    mMidiNote.setLengthInBeats (newLengthInBeats, nullptr);
};

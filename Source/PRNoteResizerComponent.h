/*
  ==============================================================================

    PRNoteResizerComponent.h
    Created: 30 Jun 2023 11:08:17am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ArrangementSection.h"
#include "PRToolBarComponent.h"

namespace styler_app
{
    class PRNoteResizerComponent
        : public juce::ResizableEdgeComponent
    {
    public:
        PRNoteResizerComponent (ArrangementSection::Ptr arrangementSection
                              , ArrangementPattern::Ptr arrangementPattern
                              , MidiNote& noteToResize
                              , const PRToolBarComponent& toolBar
                              , juce::Component* componentToResize
                              , juce::ComponentBoundsConstrainer* constrainer
                              , juce::ResizableEdgeComponent::Edge edgeToResize);

    private:
        ArrangementSection::Ptr mArrangementSection;
        ArrangementPattern::Ptr mArrangementPattern;
        MidiNote& mMidiNote;
        const PRToolBarComponent& mToolBar;

        void mouseUp (const juce::MouseEvent& event) override;


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PRNoteResizerComponent)
    };
}

/*
  ==============================================================================

    PRNoteComponent.h
    Created: 26 Jun 2023 7:15:44pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ArrangementSection.h"
#include "StepwiseRightEdgeBoundConstsrainer.h"
#include "PRNoteResizerComponent.h"
#include "PRToolBarComponent.h"
//==============================================================================
/*
*/
namespace styler_app
{
    class PRNoteComponent  : public juce::Component
    {
    public:
        PRNoteComponent (ArrangementSection::Ptr
                       , ArrangementPattern::Ptr
                       , MidiNote*
                       , PRToolBarComponent&);
        ~PRNoteComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        MidiNote* getMidiNote() { return mMidiNote; };

    private:
        ArrangementSection::Ptr mArrangementSection;
        ArrangementPattern::Ptr mArrangementPattern;
        MidiNote* mMidiNote;
        PRToolBarComponent& mToolBar;
        StepwiseRightEdgeBoundConstsrainer mStepwiseConstrainer;
        PRNoteResizerComponent mResizerComponent;
        
        void mouseDown (const juce::MouseEvent& event) override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PRNoteComponent)
    };
}

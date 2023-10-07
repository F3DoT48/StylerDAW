/*
  ==============================================================================

    PRGridComponent.h
    Created: 3 May 2023 7:37:32pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ArrangementPattern.h"
#include "ArrangementSection.h"
#include "PRToolBarComponent.h"
#include "PRNoteComponent.h"
#include "Utilities.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class PRGridComponent  : public juce::Component
                           , private te::ValueTreeAllEventListener
                           , private FlaggedAsyncUpdater
                           , private juce::ChangeListener
    {
    public:
        PRGridComponent (ArrangementPattern::Ptr
                       , ArrangementSection::Ptr
                       , PRToolBarComponent&
                       , juce::MidiKeyboardComponent&);
        ~PRGridComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        ArrangementPattern::Ptr mArrangementPattern;
        ArrangementSection::Ptr mArrangementSection;
        PRToolBarComponent& mToolBar;

        juce::MidiKeyboardComponent& mMidiKeyboard;

        juce::OwnedArray<PRNoteComponent> mNotes;

        bool mUpdateNotes;

        void valueTreeChanged() override {}
        void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override;
        void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override;
        void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override;
        void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override {}

        void handleAsyncUpdate() override;

        void changeListenerCallback (juce::ChangeBroadcaster*) override;

        void buildNotes();

        void mouseDown (const juce::MouseEvent& event) override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PRGridComponent)
    };
}
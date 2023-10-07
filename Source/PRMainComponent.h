/*
  ==============================================================================

    PRMainComponent.h
    Created: 3 May 2023 3:16:39pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PRToolBarComponent.h"
#include "PRNoteEditorComponent.h"
#include "PREventEditorComponent.h"
#include "ArrangementPattern.h"
#include "ArrangementSection.h"

//==============================================================================
/*
*/
namespace styler_app
{
    class PRMainComponent  : public juce::Component
    {
    public:
        PRMainComponent (ArrangementPattern::Ptr
                       , ArrangementSection::Ptr);
        ~PRMainComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        static constexpr int sDefaultWidthInPixels { 800 };
        static constexpr int sDefaultHeightInPixels { 600 };
        static constexpr int sToolBarHeightInPixels { 50 };
        static constexpr int sEventEditorDefaultHeightInPixels { 150 };

    private:
        ArrangementPattern::Ptr mArrangementPattern;
        ArrangementSection::Ptr mArrangementSection;

        std::unique_ptr<PRToolBarComponent> mToolBar;
        std::unique_ptr<PRNoteEditorComponent> mNoteEditor;
        std::unique_ptr<PREventEditorComponent> mEventEditor;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PRMainComponent)
    };
}
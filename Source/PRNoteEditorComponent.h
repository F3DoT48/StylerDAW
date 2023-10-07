/*
  ==============================================================================

    PRNoteEditorComponent.h
    Created: 3 May 2023 3:05:05pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PRKeyboardComponent.h"
#include "PRGridComponent.h"
#include "ArrangementPattern.h"
#include "ArrangementSection.h"
#include "PRToolBarComponent.h"
#include "PRRulerComponent.h"

//==============================================================================
/*
*/
namespace styler_app
{
    class PRNoteEditorComponent  : public juce::Component
    {
    public:
        PRNoteEditorComponent (ArrangementPattern::Ptr
                             , ArrangementSection::Ptr
                             , PRToolBarComponent&);
        ~PRNoteEditorComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;
                
        static constexpr int sPRKeyboardWidthInPixels { 75 };

    private:
        ArrangementPattern::Ptr mArrangementPattern;
        ArrangementSection::Ptr mArrangementSection;
        PRToolBarComponent& mToolBar;


        std::unique_ptr<juce::Viewport> mPRGridViewport;
        std::unique_ptr<PRKeyboardComponent> mPRKeyboardComponent;
        std::unique_ptr<PRGridComponent> mPRGridComponent;
        std::unique_ptr<PRRulerComponent> mPRRulerComponent;        

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PRNoteEditorComponent)
    };
}

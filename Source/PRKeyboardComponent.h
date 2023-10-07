/*
  ==============================================================================

    PRKeyboardComponent.h
    Created: 3 May 2023 8:39:32pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ArrangementPattern.h"
#include "PRToolBarComponent.h"

//==============================================================================
/*
*/
namespace styler_app
{
    class PRKeyboardComponent  : public juce::Component
                               , private juce::ScrollBar::Listener
                               , private juce::MidiKeyboardState::Listener
    {
    public:
        PRKeyboardComponent (ArrangementPattern::Ptr
                           , juce::Viewport&
                           , PRToolBarComponent&);
        ~PRKeyboardComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        static constexpr int sKeyboardScrollButtonsWidth { 10 }; // needed to be set manually for drawing lines in the grid

        float getKeyboardHeight() const;
        juce::MidiKeyboardComponent& getMidiKeyboard() const;

    private:
        ArrangementPattern::Ptr mArrangementPattern;

        juce::Viewport& mPRGridViewport;
        PRToolBarComponent& mToolBar;
        std::unique_ptr<juce::Viewport> mMidiKeyboardViewport;

        juce::MidiKeyboardState mMidiKeyboardState;
        std::unique_ptr<juce::MidiKeyboardComponent> mMidiKeyboardComponent;

        void scrollBarMoved (juce::ScrollBar*, double) override;

        void handleNoteOn (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;
        void handleNoteOff (juce::MidiKeyboardState* source, int midiChannel, int midiNoteNumber, float velocity) override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PRKeyboardComponent)
    };
}
/*
  ==============================================================================

    PRToolBarComponent.h
    Created: 3 May 2023 3:04:44pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "BarsInViewSelectorComponent.h"
#include "KeysInViewSelectorComponent.h"
#include "ArrangementPattern.h"
#include "ArrangementSection.h"
#include "MidiNote.h"
#include "MidiChannelSelectorComponent.h"
#include "SnapResolutionSelectorComponent.h"
#include "NoteTranspositionRuleSelectorComponent.h"

namespace styler_app
{
    class PRToolBarComponent  : public juce::Component
    {
    public:
        PRToolBarComponent (ArrangementPattern::Ptr
                          , ArrangementSection::Ptr);
        ~PRToolBarComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        static constexpr int sToolWidthInPixels { 50 };

        te::MidiChannel getCurrentMidiChannel() const;
        MidiNote& getNotePrototype();
        double getSnapResolutionInBeats() const;
        float getNumBarsInView() const;
        float getNumKeysInView() const;

        juce::ValueTree mState;

    private:
        ArrangementPattern::Ptr mArrangementPattern;
        ArrangementSection::Ptr mArrangementSection;

        te::MidiChannel mCurrentMidiChannel;
        SnapOption mCurrentSnapOption;
        MidiNote mNotePrototype;
        std::unique_ptr<MidiChannelSelectorComponent> mMidiChannelSelectorComponent;
        std::unique_ptr<SnapResolutionSelectorComponent> mSnapResolutionSelectorComponent;
        std::unique_ptr<NoteTranspositionRuleSelectorComponent> mNoteTranspositionRuleSelectorComponent;

        std::unique_ptr<BarsInViewSelectorComponent> mBarsInViewSelector;
        std::unique_ptr<KeysInViewSelectorComponent> mKeysInViewSelector;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PRToolBarComponent)
    };
}

/*
  ==============================================================================

    ArrangementPatternComponent.h
    Created: 11 Apr 2023 8:05:02pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "ArrangementPattern.h"
#include "ArrangementSection.h"
#include "PianoRollWindow.h"

namespace styler_app
{
    class ArrangementPatternComponent  : public juce::Component
    {
    public:
        ArrangementPatternComponent() = delete;
        ArrangementPatternComponent (ArrangementPattern::Ptr, ArrangementSection::Ptr);
        ~ArrangementPatternComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        static constexpr int sOpenPianoRollButtonHeigthInPixels { 20 };

    private:
        ArrangementPattern::Ptr mArrangementPattern;
        ArrangementSection::Ptr mArrangementSection;

        juce::TextButton mOpenPianoRollButton;
        std::unique_ptr<PianoRollWindow> mPianoRollWindow;

        void setupPianoRollButton();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementPatternComponent)
    };
}

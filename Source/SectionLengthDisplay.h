/*
  ==============================================================================

    SectionLengthDisplay.h
    Created: 4 Sep 2023 7:05:09pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SectionLengthSelectorComponent.h"

namespace styler_app
{
    class SectionLengthDisplay : public juce::Component
    {
    public:
        SectionLengthDisplay (ArrangementSection::Ptr arrangementSection);
        ~SectionLengthDisplay() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        ArrangementSection::Ptr mArrangementSection;
        juce::Label mLengthLabel;
        SectionLengthSelectorComponent mSelectorComponent;
        juce::Label mUnitsLabel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SectionLengthDisplay)
    };
}
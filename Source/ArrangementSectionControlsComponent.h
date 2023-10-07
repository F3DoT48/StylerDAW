/*
  ==============================================================================

    ArrangmentSectionControlsComponent.h
    Created: 26 Nov 2022 9:39:17pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Arrangement.h"
#include "ArrangementSection.h"
#include "SectionLengthDisplay.h"

namespace styler_app
{
    class ArrangementSectionControlsComponent : public juce::Component
    {
    public:
        ArrangementSectionControlsComponent() = delete;
        ArrangementSectionControlsComponent (Arrangement&, ArrangementSection::Ptr);
        ~ArrangementSectionControlsComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        static constexpr int sWidthInPixels { 200 };

    private:
        Arrangement& mArrangement;
        ArrangementSection::Ptr mArrangementSection;

        juce::Label mNameLabel;
        juce::TextButton mDeleteSectionButton;
        juce::ComboBox mTypeSelectorBox;
        SectionLengthDisplay mLengthDisplay;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementSectionControlsComponent)
    };
}
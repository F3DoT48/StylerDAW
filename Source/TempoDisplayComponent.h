/*
  ==============================================================================

    TempoDisplayComponent.h
    Created: 19 Aug 2023 7:20:08pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TempoSelectorComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class TempoDisplayComponent  : public juce::Component
    {
    public:
        TempoDisplayComponent (te::TempoSequence& tempoSequence);
        ~TempoDisplayComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        TempoSelectorComponent mSelectorComponent;
        juce::Label mUnitsLabel;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TempoDisplayComponent)
    };
}
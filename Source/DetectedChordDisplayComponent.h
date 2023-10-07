/*
  ==============================================================================

    DetectedChordDisplayComponent.h
    Created: 20 Aug 2023 5:39:52pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Arrangement.h"

namespace styler_app
{
    class DetectedChordDisplayComponent  : public juce::Component
                                         , juce::Timer
    {
    public:
        DetectedChordDisplayComponent (Arrangement& arrangement);
        ~DetectedChordDisplayComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        void timerCallback() override;

    private:
        Arrangement& mArrangement;
        juce::Label mChordLabel;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DetectedChordDisplayComponent)
    };
}
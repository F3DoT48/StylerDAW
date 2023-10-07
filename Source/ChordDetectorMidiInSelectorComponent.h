/*
  ==============================================================================

    ChordDetectorMidiInSelectorComponent.h
    Created: 18 Aug 2023 6:48:47pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace te = tracktion_engine;
namespace styler_app
{
    class ChordDetectorMidiInSelectorComponent  : public juce::Component
    {
    public:
        ChordDetectorMidiInSelectorComponent (te::Edit& edit);
        ~ChordDetectorMidiInSelectorComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        te::Edit& mEdit;

        juce::TextButton mMidiInputSelectorButton;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChordDetectorMidiInSelectorComponent)
    };
}
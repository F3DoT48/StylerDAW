/*
  ==============================================================================

    PlayStopButtonComponent.h
    Created: 20 Sep 2023 1:35:31pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace te = tracktion_engine;

namespace styler_app
{
    class PlayStopButtonComponent  : public juce::Component
    {
    public:
        PlayStopButtonComponent (te::Edit&);

        ~PlayStopButtonComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        te::Edit& mEdit;
        juce::TextButton mButton;

        void updateButtonText();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayStopButtonComponent)
    };
}
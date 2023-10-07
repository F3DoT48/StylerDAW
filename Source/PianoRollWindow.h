/*
  ==============================================================================

    PianoRollWindow.h
    Created: 3 May 2023 1:40:09pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace te = tracktion_engine;

namespace styler_app
{
    class PianoRollWindow : public juce::DocumentWindow
    {
    public:
        PianoRollWindow (const juce::String& name);
        ~PianoRollWindow();

        void closeButtonPressed() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PianoRollWindow)
    };
}
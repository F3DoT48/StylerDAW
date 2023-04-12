/*
  ==============================================================================

    ArrangerEngine.h
    Created: 20 Nov 2022 5:45:08pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "NoteTranspositionRule.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class ArrangerEngine
    {
    public:
        ArrangerEngine(te::Edit&, juce::ValueTree& parent);

        static constexpr NoteIgnoreTranspositionRule* const sNoteIgnoreTranspositionRule {};
        static constexpr NoteRetriggerWithNewPitchRule* const sNoteRetriggerWithNewPitchRule {};

        
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ArrangerEngine)
    };
}

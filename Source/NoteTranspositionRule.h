/*
  ==============================================================================

    NoteTranspositionRule.h
    Created: 20 Nov 2022 5:49:03pm
    Author:  User

  ==============================================================================
*/

#pragma once

namespace styler_app
{
    class NoteTranspositionRule
    {
    public:
        enum class Type : int
        {
            ignore = 0
            , retriggerWithNewPitch = 1

        };

        virtual void transposeAccordingToChord() = 0;
    };

    class NoteIgnoreTranspositionRule : public NoteTranspositionRule
    {
    public:
        void transposeAccordingToChord() override;
    };

    class NoteRetriggerWithNewPitchRule : public NoteTranspositionRule
    {
    public:
        void transposeAccordingToChord() override;
    };
}

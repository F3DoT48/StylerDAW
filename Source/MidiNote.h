/*
  ==============================================================================

    MidiNote.h
    Created: 29 Mar 2023 5:59:16pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "NoteTranspositionRule.h"

namespace te = tracktion_engine;

namespace styler_app
{

    class MidiNote
    {
    public:
        MidiNote (const juce::ValueTree&);
        ~MidiNote() = default;

        static juce::ValueTree createNote (const MidiNote&);

        void setStartBeat (double newStartBeat, juce::UndoManager*);
        double getStartBeat() const noexcept;

        void setLengthInBeats (double newLengthInBeats, juce::UndoManager*);
        double getLengthInBeats() const noexcept;

        void setNoteIndex (int newNoteIndex, juce::UndoManager*);
        int getNoteIndex() const noexcept;

        void setVelocity (int newVelocity, juce::UndoManager*);
        int getVelocity() const noexcept;

        void setMute (bool shouldMute, juce::UndoManager*);
        bool isMute() const noexcept;

        juce::ValueTree mState;

        // Hack for compatibility with some te::... utilities.
        juce::ValueTree& state { mState };

    private:
        friend class MidiSequence;
        double mStartBeat, mLengthInBeats;
        uint8_t mNoteIndex, mVelocity, mIsMute;
        NoteTranspositionRule* mTranspositionRule;

        void updatePropertiesFromState();

        MidiNote() = delete;
        MidiNote (const MidiNote&) = delete;
        MidiNote& operator= (const MidiNote&) = delete;
        MidiNote (const MidiNote&&) = delete;
        MidiNote& operator= (MidiNote&&) = delete;

        JUCE_LEAK_DETECTOR (MidiNote)
    };
}

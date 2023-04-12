/*
  ==============================================================================

    ArrangementSection.h
    Created: 31 Mar 2023 12:57:32pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ArrangementPatternList.h"

namespace styler_app
{
    class ArrangementSection : public juce::ReferenceCountedObject
                             , private juce::ValueTree::Listener
    {
    public:

        ArrangementSection() = delete;
        ArrangementSection (te::Edit&, const juce::ValueTree&, juce::UndoManager*);
        ~ArrangementSection();

        using Ptr = juce::ReferenceCountedObjectPtr<ArrangementSection>;

        static juce::ValueTree createSection (const te::Edit&);

        ArrangementPatternList& getAllPatterns() noexcept;

        static constexpr int sMaxLengthInBars { 16 };
        static constexpr int sMinLengthInBars { 1 };

        int getLengthInBars() const noexcept;
        void setLengthInBars (int newLengthInBars, juce::UndoManager*);

        enum class PlaybackMode : int
        {
            loop = 0
          , fill = 1
          , intro = 2
          , ending = 3
        };

        PlaybackMode getPlaybackMode() const noexcept;
        void setPlaybackMode (PlaybackMode newPlaybackMode, juce::UndoManager*);

        juce::ValueTree mState;
        /* !!! hack for te::ValueTreeUtilities */
        juce::ValueTree& state { mState };

    private:
        te::Edit& mEdit;
        //int mSectionId;

        int mLengthInBars;

        PlaybackMode mPlaybackMode;

        std::unique_ptr<ArrangementPatternList> mPatterns;

        /* Listen to edit state and rebuild patterns from scratch on track additions / deletions
        * Manipulate the patterns state according to track changes (one can use EditItemID for matching patterns to tracks)
        * Rebuild the mPatterns.
        */
        void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override;
        void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override;
        void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override;       

        juce::ValueTree::Listener listener;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementSection)
    };
}
/*
  ==============================================================================

    SnapResolutionSelectorComponent.h
    Created: 11 Sep 2023 11:51:50am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MidiNote.h"

namespace te = tracktion_engine;

namespace styler_app
{

    enum class SnapOption : int
    {
        bar = 1
      , halfBar
      , beat
      , halfBeat
      , thirdBeat
      , quarterBeat
      , sixthBeat
      , eighthBeat
      , twelfthBeat
      , sixteenthBeat
      , smallest
    };

    static inline const juce::StringArray sSnapOptionsNames
    {
        "bar"
      , "1/2 bar"
      , "beat"
      , "1/2 beat"
      , "1/3 beat"
      , "1/4 beat"
      , "1/6 beat"
      , "1/8 beat"
      , "1/12 beat"
      , "1/16 beat"
      , "smallest"
    };

    class SnapResolutionSelectorComponent
        : public juce::Component
    {
    public:
        SnapResolutionSelectorComponent (SnapOption& currentSnapOption);
        ~SnapResolutionSelectorComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        SnapOption& mCurrentSnapOption;
        juce::Label mSnapLabel;
        juce::ComboBox mComboBox;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SnapResolutionSelectorComponent)
    };
}
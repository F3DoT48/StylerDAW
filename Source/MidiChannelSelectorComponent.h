  /*
  ==============================================================================

    MidiChannelSelectorComponent.h
    Created: 11 Sep 2023 12:41:32pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ArrangementPattern.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class MidiChannelSelectorComponent
        : public juce::Component
        , private te::ValueTreeAllEventListener
    {
    public:
        MidiChannelSelectorComponent (juce::ValueTree& toolBarState, ArrangementPattern::Ptr);
        ~MidiChannelSelectorComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        juce::ValueTree& mToolBarState;
        ArrangementPattern::Ptr mArrangementPattern;
        juce::Label mChannelLabel;
        juce::ComboBox mComboBox;

        void updateComboBox();

        void valueTreeChanged() override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiChannelSelectorComponent)
    };
}
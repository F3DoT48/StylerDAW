/*
  ==============================================================================

    ArrangementSectionComponent.h
    Created: 11 Apr 2023 8:04:40pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Arrangement.h"
#include "ArrangementSection.h"
#include "Utilities.h"
#include "ArrangementPatternComponent.h"

namespace styler_app
{
    class ArrangementSectionComponent : public juce::Component
                                      , private te::ValueTreeAllEventListener
                                      , private FlaggedAsyncUpdater
                                      , private juce::ChangeListener
    {
    public:
        ArrangementSectionComponent() = delete;
        ArrangementSectionComponent (Arrangement&, ArrangementSection::Ptr);
        ~ArrangementSectionComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        static constexpr int sWidthInPixels { 200 };

    private:
        Arrangement& mArrangement;
        ArrangementSection::Ptr mArrangementSection;

        juce::OwnedArray<ArrangementPatternComponent> mPatterns;

        bool mUpdatePatterns;

        void valueTreeChanged() override {}
        void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override;
        void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override;
        void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override;
        void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override;

        void handleAsyncUpdate() override;

        void changeListenerCallback (juce::ChangeBroadcaster*) override;

        void buildPatterns();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementSectionComponent)
    };
}

/*
  ==============================================================================

    ArrangementControlsArea.h
    Created: 25 Nov 2022 11:41:47am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Arrangement.h"
#include "Utilities.h"
#include "ArrangementSectionControlsComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class ArrangementControlsArea : public juce::Component
                                  , private te::ValueTreeAllEventListener
                                  , private FlaggedAsyncUpdater
                                  , private juce::ChangeListener
    {
    public:
        ArrangementControlsArea (Arrangement&);
        ~ArrangementControlsArea() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        static constexpr int sHeightInPixels { 80 };

    private:
        Arrangement& mArrangement;

        juce::OwnedArray<ArrangementSectionControlsComponent> mSectionControlsAreas;

        bool mUpdateSections;

        void valueTreeChanged() override {}
        void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override {}
        void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override;
        void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override;
        void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override {}

        void handleAsyncUpdate() override;

        void changeListenerCallback (juce::ChangeBroadcaster*) override;

        void buildSections();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementControlsArea)
    };
}

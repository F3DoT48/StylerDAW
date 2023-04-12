/*
  ==============================================================================

    ArrangementSectionsArea.h
    Created: 25 Nov 2022 12:05:35pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Arrangement.h"
#include "Utilities.h"
#include "ArrangementSectionComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class ArrangementSectionsArea : public juce::Component
                                  , private te::ValueTreeAllEventListener
                                  , private FlaggedAsyncUpdater
                                  , private juce::ChangeListener
    {
    public:
        ArrangementSectionsArea() = delete;
        ArrangementSectionsArea (Arrangement&);
        ~ArrangementSectionsArea() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        Arrangement& mArrangement;

        juce::OwnedArray<ArrangementSectionComponent> mSections;

        bool mUpdateSections;

        void valueTreeChanged() override {}
        void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override {}
        void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override;
        void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override;
        void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override {}

        void handleAsyncUpdate() override;

        void changeListenerCallback (juce::ChangeBroadcaster*) override;

        void buildSections();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementSectionsArea)
    };
}
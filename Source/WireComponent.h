/*
  ==============================================================================

    WireComponent.h
    Created: 16 Oct 2022 11:33:33am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace te = tracktion_engine;

namespace styler_app
{
    class WireComponent : public juce::Component
    {
    public:
        WireComponent (juce::Point<int>
                     , juce::Point<int>);

        ~WireComponent() override;

        void paint (juce::Graphics&) override;

        const juce::Point<int>& getBeginInRack() const;
        void setBeginInRack (const juce::Point<int>&);
        const juce::Point<int>& getEndInRack() const;
        void setEndInRack (const juce::Point<int>&);

    private:
        juce::Point<int> mBeginInRack;
        juce::Point<int> mEndInRack;
        juce::Path mPath;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WireComponent)
    };
}
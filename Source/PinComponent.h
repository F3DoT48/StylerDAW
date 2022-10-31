/*
  ==============================================================================

    PinComponent.h
    Created: 16 Oct 2022 11:32:55am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ConnectionTarget.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class PinComponent : public juce::Component
                       , public ConnectionTarget
    {
    public:
        PinComponent (te::EditItemID editItemId
                    , const int pinIndex
                    , const bool isInputPin
                    , const juce::String& name);

        ~PinComponent() override;

        void paint (juce::Graphics&) override;
        void mouseDown (const juce::MouseEvent&) override;
        void mouseUp (const juce::MouseEvent&) override;
        void mouseDrag (const juce::MouseEvent&) override;

        juce::Point<int> getWireSnapPositionInRack() const noexcept;

        static inline constexpr int sWidthInPixels {50};
        static inline constexpr int sHeightInPixels {20};

    private:
        const juce::String mName;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PinComponent)
    };
}

/*
  ==============================================================================

    PluginInRackComponent.h
    Created: 18 Jul 2022 6:37:31pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PinComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class PluginInRackComponent  : public juce::Component
    {
    public:
        PluginInRackComponent (te::Plugin::Ptr);
        ~PluginInRackComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;
        void mouseDrag (const juce::MouseEvent&) override;

        te::Plugin::Ptr getPlugin() const;
        const juce::OwnedArray<PinComponent>& getInputPins () const;
        const juce::OwnedArray<PinComponent>& getOutputPins () const;
                
        static inline constexpr int sWidthInPixels {150};        
        static inline constexpr int sButtonWidthInPixels {20};
        static inline constexpr int sMinHeightInPixels {2 * sButtonWidthInPixels};
        static inline constexpr int sGapBtwPins {3};

    private:
        te::Plugin::Ptr mPlugin;

        juce::TextButton mShowPluginUIButton;
        juce::TextButton mDeleteFromRackButton;
        juce::TextButton mOutputPinsButton;

        juce::OwnedArray<PinComponent> mInputPins;
        juce::OwnedArray<PinComponent> mOutputPins;      

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginInRackComponent)
    };
}

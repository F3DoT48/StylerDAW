/*
  ==============================================================================

    PluginsRackComponent.h
    Created: 18 Jul 2022 5:40:15pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Utilities.h"
#include "PluginInRackComponent.h"
#include "PinComponent.h"
#include "WireComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class PluginsRackComponent  : public juce::Component
                                , private styler_app::FlaggedAsyncUpdater
                                , private te::ValueTreeAllEventListener
    {
    public:
        PluginsRackComponent (te::RackInstance*);
        ~PluginsRackComponent () override;

        void paint (juce::Graphics&) override;
        void resized() override;

        static inline constexpr int sViewportWidth { 800 };
        static inline constexpr int sViewportHeight { 600 };

        te::RackInstance* getRackInstance() const;
        WireComponent& getDrawableWire() const;

    private:
        void valueTreeChanged() override;
        void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override;
        void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override;
        void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override;

        void handleAsyncUpdate() override;
    
        void buildPlugins();

        void rebuildWiresUsingPlugins();

        void buildWires();

        te::RackInstance* mRackInstance;

        juce::TextButton mAddPluginButton;
        std::unique_ptr<PinComponent> mMidiPinToPlugins;
        std::unique_ptr<PinComponent> mAudioLeftPinToPlugins;
        std::unique_ptr<PinComponent> mAudioRightPinToPlugins;
        std::unique_ptr<PinComponent> mAudioLeftPinFromPlugins;
        std::unique_ptr<PinComponent> mAudioRightPinFromPlugins;

        juce::OwnedArray<PluginInRackComponent> mPlugins;
        juce::OwnedArray<WireComponent> mWires;
        std::unique_ptr<WireComponent> mDrawableWire;

        bool mUpdatePlugins;
        bool mUpdateWires;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginsRackComponent)
    };
}

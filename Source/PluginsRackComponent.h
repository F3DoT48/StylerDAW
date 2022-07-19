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

    private:
        void valueTreeChanged() override;
        void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override;
        void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override;
        void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override;

        void handleAsyncUpdate() override;
    
        void buildPlugins();

        te::RackInstance* mRackInstance;

        juce::TextButton mAddPluginButton;

        juce::OwnedArray<PluginInRackComponent> mPlugins;

        bool mUpdatePlugins;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginsRackComponent)
    };
}

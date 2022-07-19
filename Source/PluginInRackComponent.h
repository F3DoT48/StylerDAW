/*
  ==============================================================================

    PluginInRackComponent.h
    Created: 18 Jul 2022 6:37:31pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

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

        te::Plugin::Ptr getPlugin();

    private:
        te::Plugin::Ptr mPlugin;

        juce::TextButton mShowPluginUIButton;
        juce::TextButton mDeleteFromRackButton;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginInRackComponent)
    };
}

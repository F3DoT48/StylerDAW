#pragma once

#include <JuceHeader.h>

namespace te = tracktion_engine;

namespace styler_app
{
    class SettingsComponent : public juce::TabbedComponent
    {
    public:
        SettingsComponent (te::Engine&);
        ~SettingsComponent();

    private:
        te::Engine& mTracktionEngine;

        std::unique_ptr<juce::AudioDeviceSelectorComponent> mAudioDeviceSelectorComponent;
        std::unique_ptr<juce::PluginListComponent> mPluginListComponent;
    };
}

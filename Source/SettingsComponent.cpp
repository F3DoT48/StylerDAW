#include <JuceHeader.h>
#include "SettingsComponent.h"

using namespace styler_app;

SettingsComponent::SettingsComponent (te::Engine& tracktionEngine)
    : juce::TabbedComponent{ juce::TabbedButtonBar::Orientation::TabsAtLeft }
    , mTracktionEngine{ tracktionEngine }
    , mAudioDeviceSelectorComponent{ std::make_unique<juce::AudioDeviceSelectorComponent> (mTracktionEngine.getDeviceManager().deviceManager
                                                                                         , 0
                                                                                         , 64
                                                                                         , 1
                                                                                         , 64
                                                                                         , true
                                                                                         , true
                                                                                         , true
                                                                                         , false) }
    , mPluginListComponent{ std::make_unique<juce::PluginListComponent> (mTracktionEngine.getPluginManager().pluginFormatManager
                                                                       , mTracktionEngine.getPluginManager().knownPluginList
                                                                       , mTracktionEngine.getTemporaryFileManager().getTempFile ("PluginScanDeadMansPedal") 
                                                                       , te::getApplicationSettings()) }
{
    setTitle ("Settings");
    const auto tabColour{ juce::LookAndFeel::getDefaultLookAndFeel().findColour (juce::TabbedComponent::backgroundColourId).darker (0.1f)};

    addTab ("Audio settings", tabColour, mAudioDeviceSelectorComponent.get(), false);
    addTab ("Plugins", tabColour, mPluginListComponent.get(), false);
    
    setSize (800, 600);
}

SettingsComponent::~SettingsComponent()
{

}
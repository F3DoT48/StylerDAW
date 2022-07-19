/*
  ==============================================================================

    PluginsAreaComponent.cpp
    Created: 4 Jun 2022 10:37:39pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginsAreaAudioTrackComponent.h"
#include "PluginsRackComponent.h"

using namespace styler_app;

//==============================================================================
PluginsAreaAudioTrackComponent::PluginsAreaAudioTrackComponent (EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent {editViewState, track}
    , mPluginsRackButton {"Open Pugins Rack"}
{
    mPluginsRackButton.onClick = [this]()
    {
        juce::DialogWindow::LaunchOptions pluginsRackWindowLaunchOptions;
        pluginsRackWindowLaunchOptions.dialogTitle = "Plugins Rack";
        pluginsRackWindowLaunchOptions.dialogBackgroundColour = getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId);
        pluginsRackWindowLaunchOptions.escapeKeyTriggersCloseButton = false;
        pluginsRackWindowLaunchOptions.resizable = true;
        pluginsRackWindowLaunchOptions.useBottomRightCornerResizer = true;

        auto pluginsRackComponent {new PluginsRackComponent (dynamic_cast<te::RackInstance*> (mTrack->pluginList[0]))};

        pluginsRackComponent->setSize (800, 600);
        pluginsRackWindowLaunchOptions.content.setOwned (pluginsRackComponent);
        pluginsRackWindowLaunchOptions.launchAsync();
    };

    addAndMakeVisible (mPluginsRackButton);
}

PluginsAreaAudioTrackComponent::~PluginsAreaAudioTrackComponent()
{
}

void PluginsAreaAudioTrackComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText (mTrack->getName() + " PluginsArea"
                , getLocalBounds()
                , juce::Justification::centred
                , true);
}

void PluginsAreaAudioTrackComponent::resized()
{
    auto rectangle{ getLocalBounds()};

    mPluginsRackButton.setBounds (rectangle.removeFromTop(20));
}

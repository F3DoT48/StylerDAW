/*
  ==============================================================================

    PluginInRackComponent.cpp
    Created: 18 Jul 2022 6:37:31pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginInRackComponent.h"

using namespace styler_app;

//==============================================================================

PluginInRackComponent::PluginInRackComponent (te::Plugin::Ptr plugin)
    : mPlugin { plugin }
    , mShowPluginUIButton { "Show Plugin " }
    , mDeleteFromRackButton { "Delete Plugin" }
{
    mShowPluginUIButton.onClick = [this]()
    {
        mPlugin->showWindowExplicitly();
    };

    mDeleteFromRackButton.onClick = [this]()
    {
        mPlugin->deleteFromParent();
    };

    addAndMakeVisible (mShowPluginUIButton);
    addAndMakeVisible (mDeleteFromRackButton);
}

PluginInRackComponent::~PluginInRackComponent()
{
}

void PluginInRackComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PluginInRackComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PluginInRackComponent::resized()
{
    auto rect { getLocalBounds() };

    mShowPluginUIButton.setBounds (rect.removeFromTop (30));
    mDeleteFromRackButton.setBounds (rect.removeFromTop (30));
}

te::Plugin::Ptr PluginInRackComponent::getPlugin()
{
    return mPlugin;
}

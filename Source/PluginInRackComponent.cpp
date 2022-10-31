/*
  ==============================================================================

    PluginInRackComponent.cpp
    Created: 18 Jul 2022 6:37:31pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginInRackComponent.h"
#include "PluginsRackComponent.h"

using namespace styler_app;

//==============================================================================

PluginInRackComponent::PluginInRackComponent (te::Plugin::Ptr plugin)
    : mPlugin { plugin }
    , mShowPluginUIButton { plugin->getName() }
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

    juce::StringArray ins, outs;
    mPlugin->getChannelNames (&ins, &outs);

    auto midiInputPin { new PinComponent (mPlugin->itemID, 0, true, "MIDI") };
    mInputPins.add (midiInputPin);
    addAndMakeVisible (midiInputPin);

    for (int pinId { 1 }; pinId <= ins.size(); ++pinId)
    {
        auto pinComponent { new PinComponent (mPlugin->itemID
                                            , pinId
                                            , true
                                            , ins[pinId - 1]) };
        mInputPins.add (pinComponent);

        addAndMakeVisible (pinComponent);
    }

    auto midiOutputPin { new PinComponent (mPlugin->itemID, 0, false, "MIDI") };
     mOutputPins.add (midiOutputPin);
     addAndMakeVisible (midiOutputPin);

    for (int pinId { 1 }; pinId <= outs.size(); ++pinId)
    {
        auto pinComponent { new PinComponent (mPlugin->itemID
                                            , pinId
                                            , false
                                            , outs[pinId - 1]) };
        mOutputPins.add (pinComponent);

        addAndMakeVisible (pinComponent);
    }

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

    //g.setColour (juce::Colours::white);
    //g.setFont (14.0f);
    //g.drawText ("PluginInRackComponent", getLocalBounds(),
    //            juce::Justification::centred, true);   // draw some placeholder text
}

void PluginInRackComponent::resized()
{
    auto rect { getLocalBounds() };

    mShowPluginUIButton.setBounds (rect.removeFromTop (sButtonWidthInPixels));
    mDeleteFromRackButton.setBounds (rect.removeFromTop (sButtonWidthInPixels));

    jassert (rect.getY() == sMinHeightInPixels);

    auto rectForInputPins { rect.removeFromLeft (PinComponent::sWidthInPixels) };

    for (auto inputPin : mInputPins)
    {
        inputPin->setBounds (rectForInputPins.removeFromTop (PinComponent::sHeightInPixels + sGapBtwPins));
    }

    auto rectForOutputPins {rect.removeFromRight (PinComponent::sWidthInPixels)};

    for (auto outputPin : mOutputPins)
    {
        outputPin->setBounds (rectForOutputPins.removeFromTop (PinComponent::sHeightInPixels + sGapBtwPins));
    }
}

void PluginInRackComponent::mouseDrag (const juce::MouseEvent& mouseEvent)
{
    if (! mouseEvent.mods.isPopupMenu())
    {
        toFront (true);
        const juce::Point<int> parentDimensions {getParentWidth(), getParentHeight()};
        const juce::Point<int> viewportDimensions { PluginsRackComponent::sViewportWidth, PluginsRackComponent::sViewportHeight };
        
        int maxHeight { PluginsRackComponent::sViewportHeight };
        int maxWidth { PluginsRackComponent::sViewportWidth };

        for (const auto plugin : mPlugin->getOwnerRackType()->getPlugins())
        {
            juce::StringArray ins, outs;
            plugin->getChannelNames (&ins, &outs);
            const auto pluginPosition { mPlugin->getOwnerRackType()->getPluginPosition (plugin) };
            const int maxNumPins { juce::jmax (ins.size()
                                             , outs.size()) };

            const int pluginBottom { PluginInRackComponent::sMinHeightInPixels
                                   + maxNumPins * PinComponent::sHeightInPixels
                                   + (maxNumPins - 1) * PluginInRackComponent::sGapBtwPins };

            const int newHeight { static_cast<int> (pluginPosition.getY() * viewportDimensions.getY())
                                + pluginBottom };

            maxHeight = juce::jmax (maxHeight
                                  , newHeight);

            const int newWidth { static_cast<int> (pluginPosition.getX() * viewportDimensions.getX())
                               + PluginInRackComponent::sWidthInPixels };

            maxWidth = juce::jmax (maxWidth
                                 , newWidth);            
        }

        getParentComponent()->setSize (maxWidth, maxHeight);

        const juce::Point<float> offsetFloat { mouseEvent.getOffsetFromDragStart().toFloat() / viewportDimensions.toFloat()};

        const juce::Point<float> positionFloat {(getParentComponent()->getLocalPoint (this, mouseEvent.mouseDownPosition)
                                               - getLocalBounds().getCentre().toFloat()) / viewportDimensions.toFloat() };

        mPlugin->getOwnerRackType()->setPluginPosition (mPlugin->getOwnerRackType()->getPlugins().indexOf (mPlugin.get())
                                                      , positionFloat + offsetFloat);

        getParentComponent()->resized();
        beginDragAutoRepeat (100);
    }
}

te::Plugin::Ptr PluginInRackComponent::getPlugin() const
{
    return mPlugin;
}

const juce::OwnedArray<PinComponent>& PluginInRackComponent::getInputPins() const
{
    return mInputPins;
}

const juce::OwnedArray<PinComponent>& PluginInRackComponent::getOutputPins() const
{
    return mOutputPins;
}

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
#include "PinComponent.h"

using namespace styler_app;

//==============================================================================
PluginsAreaAudioTrackComponent::PluginsAreaAudioTrackComponent (EditViewState& editViewState, te::Track::Ptr track)
    : TrackComponent {editViewState, track}
    , mPluginsRackButton {"Open Plugins Rack"}
    , mPluginsRackWindow {new PluginsRackWindow (juce::String (track->getName()) += " Plugins Rack")}
{
    mPluginsRackButton.onClick = [this]()
    {
        if (mPluginsRackWindow->getContentComponent() == nullptr)
        {
            auto rack { dynamic_cast<te::RackInstance*> (mTrack->pluginList[1]) };

            auto pluginsRackComponent { new PluginsRackComponent (rack) };
        
            int pluginRackComponentWidth { PluginsRackComponent::sViewportWidth };
            int pluginRackComponentHeight { PluginsRackComponent::sViewportHeight };

            for (const auto plugin : rack->type->getPlugins())
            {
                juce::StringArray ins, outs;
                plugin->getChannelNames (&ins, &outs);
                const auto pluginPosition { rack->type->getPluginPosition (plugin) };
                const int maxNumPins { juce::jmax (ins.size()
                                                 , outs.size()) };

                const int pluginBottom { PluginInRackComponent::sMinHeightInPixels
                                       + maxNumPins * PinComponent::sHeightInPixels
                                       + (maxNumPins - 1) * PluginInRackComponent::sGapBtwPins };

                const int newHeight { static_cast<int> (pluginPosition.getY() * pluginRackComponentHeight)
                                    + pluginBottom };

                pluginRackComponentHeight = juce::jmax (pluginRackComponentHeight
                                                      , newHeight);

                const int newWidth { static_cast<int> (pluginPosition.getX() * pluginRackComponentWidth)
                                   + PluginInRackComponent::sWidthInPixels };

                pluginRackComponentWidth = juce::jmax (pluginRackComponentWidth
                                                     , newWidth);
            }

            pluginsRackComponent->setSize (pluginRackComponentWidth, pluginRackComponentHeight);

            auto pluginsRackComponentViewport {new juce::Viewport ("PluginsRackComponentViewport") };
            pluginsRackComponentViewport->setViewedComponent (pluginsRackComponent);
            pluginsRackComponentViewport->setBounds (0, 0, PluginsRackComponent::sViewportWidth, PluginsRackComponent::sViewportHeight);
            mPluginsRackWindow->setContentOwned (pluginsRackComponentViewport, true);
        }


        mPluginsRackWindow->centreWithSize (mPluginsRackWindow->getWidth(), mPluginsRackWindow->getHeight());
        mPluginsRackWindow->setVisible (true);
        mPluginsRackWindow->toFront (true);
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

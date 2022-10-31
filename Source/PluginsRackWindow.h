/*
  ==============================================================================

    PluginsRackWindow.h
    Created: 27 Oct 2022 4:06:25pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PinComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class PluginsRackWindow : public juce::DocumentWindow
    {
        public:
            PluginsRackWindow(const juce::String& name);
            ~PluginsRackWindow() override;

            void closeButtonPressed() override;
    };
}
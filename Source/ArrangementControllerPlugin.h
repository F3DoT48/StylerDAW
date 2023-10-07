/*
  ==============================================================================

    ArrangementControllerPlugin.h
    Created: 10 Aug 2023 2:05:13pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ArrangementController.h"
#include "ChordDetector.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class ArrangementControllerPlugin : public te::Plugin
    {
    public:
        ArrangementControllerPlugin (te::PluginCreationInfo);
        ~ArrangementControllerPlugin() override;

        juce::String getName() const override;
        static const char* xmlTypeName;
        juce::String getPluginType() override;

        void initialise (const te::PluginInitialisationInfo&) override;
        void deinitialise() override;

        void applyToBuffer (const te::PluginRenderContext&) override;

        double getLatencySeconds() override;
        
        int getNumOutputChannelsGivenInputs (int) override;
        void getChannelNames (juce::StringArray*, juce::StringArray*) override;

        bool takesAudioInput() override;
        bool canBeAddedToClip() override;
        bool needsConstantBufferSize() override;

        juce::String getSelectableDescription() override;

        void setArrangementController (ArrangementController* controllerToUse);

    private:
        std::atomic<ArrangementController*> mArrangementController;
        ChordDetector mChordDetector;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementControllerPlugin)
    };
}

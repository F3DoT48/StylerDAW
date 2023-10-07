/*
  ==============================================================================

    PatternPlayerPlugin.h
    Created: 10 Aug 2023 2:04:51pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ArrangementController.h"
#include "ArrangementForAudioThreadGenerator.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class PatternPlayerPlugin 
        : public te::Plugin
    {
    public:
        PatternPlayerPlugin (te::PluginCreationInfo creationInfo);
        ~PatternPlayerPlugin();

        juce::String getName() const override;
        static const char* xmlTypeName;
        juce::String getPluginType() override;

        void initialise (const te::PluginInitialisationInfo&) override;
        void deinitialise() override;

        void prepareForNextBlock (tracktion::TimePosition editTime) override;
        void applyToBuffer (const te::PluginRenderContext&) override;

        double getLatencySeconds() override;

        int getNumOutputChannelsGivenInputs (int) override;
        void getChannelNames (juce::StringArray*, juce::StringArray*) override;

        bool canBeAddedToClip() override;
        bool needsConstantBufferSize() override;

        juce::String getSelectableDescription() override;

        void setArrangementController (ArrangementController* controllerToUse);
        void setArrangementForAudioThreadGenerator (ArrangementForAudioThreadGenerator* generatorToUse);
        void setOwnerTrackIndex (int newIndex);

    private:
        std::atomic<ArrangementController*> mArrangementController;
        std::atomic<ArrangementForAudioThreadGenerator*> mGenerator;
        std::atomic<int> mOwnerTrackIndex;
        double mBeatsPerSecond;
        double mBeatsSinceSectionStart;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PatternPlayerPlugin)
    };
}
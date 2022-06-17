/*
  ==============================================================================

    PluginsAreaMasterTrackComponent.h
    Created: 16 Jun 2022 3:53:10pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class PluginsAreaMasterTrackComponent  : public styler_app::TrackComponent
    {
    public:
        PluginsAreaMasterTrackComponent (EditViewState&, te::Track::Ptr);
        ~PluginsAreaMasterTrackComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginsAreaMasterTrackComponent)
    };
}

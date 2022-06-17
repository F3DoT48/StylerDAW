/*
  ==============================================================================

    PluginsAreaTempoTrackComponent.h
    Created: 16 Jun 2022 3:53:24pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class PluginsAreaTempoTrackComponent  : public styler_app::TrackComponent
    {
    public:
        PluginsAreaTempoTrackComponent (EditViewState&, te::Track::Ptr);
        ~PluginsAreaTempoTrackComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginsAreaTempoTrackComponent)
    };
}

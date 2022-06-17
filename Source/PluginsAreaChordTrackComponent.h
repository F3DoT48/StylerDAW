/*
  ==============================================================================

    PluginsAreaChordTrackComponent.h
    Created: 16 Jun 2022 3:54:00pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class PluginsAreaChordTrackComponent  : public styler_app::TrackComponent
    {
    public:
        PluginsAreaChordTrackComponent (EditViewState&, te::Track::Ptr);
        ~PluginsAreaChordTrackComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginsAreaChordTrackComponent)
    };
}

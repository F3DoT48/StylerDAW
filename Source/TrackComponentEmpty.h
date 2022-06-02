/*
  ==============================================================================

    TrackComponentEmpty.h
    Created: 30 May 2022 7:28:51pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class TrackComponentEmpty : public TrackComponent
    {
        public:
        TrackComponentEmpty (EditViewState&, te::Track::Ptr);
        ~TrackComponentEmpty() override;

        void paint(juce::Graphics&) override;
        void resized() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackComponentEmpty)

    };
}
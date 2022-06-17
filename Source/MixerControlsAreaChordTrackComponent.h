/*
  ==============================================================================

    MixerControlsAreaChordTrackComponent.h
    Created: 16 Jun 2022 3:49:31pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class MixerControlsAreaChordTrackComponent  : public styler_app::TrackComponent
    {
    public:
        MixerControlsAreaChordTrackComponent (EditViewState&, te::Track::Ptr);
        ~MixerControlsAreaChordTrackComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerControlsAreaChordTrackComponent)
    };
}

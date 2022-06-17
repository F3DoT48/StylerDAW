/*
  ==============================================================================

    MixerControlsAreaMasterTrackComponent.h
    Created: 16 Jun 2022 3:48:33pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class MixerControlsAreaMasterTrackComponent  : public styler_app::TrackComponent
    {
    public:
        MixerControlsAreaMasterTrackComponent (EditViewState&, te::Track::Ptr);
        ~MixerControlsAreaMasterTrackComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerControlsAreaMasterTrackComponent)
    };
}


/*
  ==============================================================================

    MixerControlsAreaComponent.h
    Created: 4 Jun 2022 10:37:55pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class MixerControlsAreaComponent  : public styler_app::TrackComponent
    {
    public:
        MixerControlsAreaComponent (EditViewState&, te::Track::Ptr);
        ~MixerControlsAreaComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerControlsAreaComponent)
    };
}

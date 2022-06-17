/*
  ==============================================================================

    InputsAreaTempoTrackComponent.h
    Created: 16 Jun 2022 3:52:43pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class InputsAreaTempoTrackComponent  : public styler_app::TrackComponent
    {
    public:
        InputsAreaTempoTrackComponent (EditViewState&, te::Track::Ptr);
        ~InputsAreaTempoTrackComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputsAreaTempoTrackComponent)
    };
}

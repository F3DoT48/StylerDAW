/*
  ==============================================================================

    InputsAreaChordTrackComponent.h
    Created: 16 Jun 2022 3:52:26pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class InputsAreaChordTrackComponent  : public styler_app::TrackComponent
    {
    public:
        InputsAreaChordTrackComponent (EditViewState&, te::Track::Ptr);
        ~InputsAreaChordTrackComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputsAreaChordTrackComponent)
    };
}
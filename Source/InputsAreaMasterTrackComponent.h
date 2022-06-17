/*
  ==============================================================================

    InputsAreaMasterTrackComponent.h
    Created: 16 Jun 2022 3:51:46pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class InputsAreaMasterTrackComponent  : public styler_app::TrackComponent
    {
    public:
        InputsAreaMasterTrackComponent (EditViewState&, te::Track::Ptr);
        ~InputsAreaMasterTrackComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputsAreaMasterTrackComponent)
    };
}

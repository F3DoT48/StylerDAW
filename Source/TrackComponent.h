/*
  ==============================================================================

    TrackComponent.h
    Created: 2 May 2022 8:26:17pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StylerTracktionEngineBehaviour.h"
#include "EditViewState.h"

namespace te = tracktion_engine;

namespace styler_app
{
    struct TrackComponentAttributes
    {
        int heightInPixels{ 40 };
        int mixerParametersAreaWidth{ 100 };
        int arrangementPartWidthInPixels{ 100 };
        int pluginAreaWidthInPixels{ 100 };
    };

    class TrackComponent : public juce::Component
    {
    public:
        TrackComponent (EditViewState&, te::Track::Ptr);
        ~TrackComponent() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        static TrackComponentAttributes getAttributes() noexcept;

    private:
        EditViewState& mEditViewState;
        te::Track::Ptr mTrack;


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackComponent)
    };
}

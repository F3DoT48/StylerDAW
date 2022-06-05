/*
  ==============================================================================

    TrackComponent.h
    Created: 2 May 2022 8:26:17pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "EditViewState.h"

namespace te = tracktion_engine;

namespace styler_app
{
    struct TrackComponentAttributes
    {
        static constexpr int minimumHeightInPixels{ 80 };
        static constexpr int trackGapInPixels{ 2 };
        static constexpr int mixerParametersAreaWidth{ 100 };
        static constexpr int arrangementPartWidthInPixels{ 100 };
        static constexpr int pluginAreaWidthInPixels{ 100 };

        static constexpr  int newTrackButtonOffsetFromRight{ 150 };
        static constexpr  int newTrackButtonHeight{ 30 };
    };

    class TrackComponent : public juce::Component
    {
    public:
        TrackComponent (EditViewState&, te::Track::Ptr);
        ~TrackComponent() override;

        void paint(juce::Graphics&) override;
        void resized() override;

        te::Track::Ptr getTrack() noexcept;

    protected:
        EditViewState& mEditViewState;
        te::Track::Ptr mTrack;

    private:

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackComponent)
    };
}

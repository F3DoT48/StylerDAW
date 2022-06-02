/*
  ==============================================================================

    TrackComponentMaster.h
    Created: 30 May 2022 7:25:54pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class TrackComponentMaster : public TrackComponent
    {
    public:
        TrackComponentMaster (EditViewState&, te::Track::Ptr);

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackComponentMaster)

    };
}
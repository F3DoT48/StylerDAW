/*
  ==============================================================================

    TrackComponentArranger.h
    Created: 30 May 2022 7:26:26pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponentEmpty.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class TrackComponentArranger : public TrackComponentEmpty
    {
    public:
        TrackComponentArranger (EditViewState&, te::Track::Ptr);
       
    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackComponentArranger)

    };
}
/*
  ==============================================================================

    TempoSelectorComponent.h
    Created: 19 Aug 2023 5:28:55pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include "ScrollAndDragSelectorComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class TempoSelectorComponent  : public ScrollAndDragSelectorComponent<double>
    {
    public:
        TempoSelectorComponent (te::TempoSequence& tempoSequence);
        ~TempoSelectorComponent() override;

        void onValueChange() override;

    private:
        te::TempoSequence& mTempoSequence;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TempoSelectorComponent)
    };
}

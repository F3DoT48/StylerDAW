/*
  ==============================================================================

    BarsInViewSelectorComponent.h
    Created: 19 May 2023 6:54:46pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ScrollAndDragSelectorComponent.h"

namespace styler_app
{
    class BarsInViewSelectorComponent : public ScrollAndDragSelectorComponent<float>
    {
    public:
        BarsInViewSelectorComponent (float minBarsInView
                                   , float maxBarsInView
                                   , float stepInBars
                                   , float startValue);
        ~BarsInViewSelectorComponent() override;

        void onValueChange() override;
        float getCurrentNumBarsInView() const;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BarsInViewSelectorComponent)
    };
}


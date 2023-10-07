/*
  ==============================================================================

    StepwiseRightEdgeBoundConstsrainer.h
    Created: 17 Sep 2023 12:50:10pm
    Author:  User

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "PRToolBarComponent.h"

namespace styler_app
{
    class StepwiseRightEdgeBoundConstsrainer
        : public juce::ComponentBoundsConstrainer
    {
    public:
        StepwiseRightEdgeBoundConstsrainer (int numPixelsPerBeat
                                          , const PRToolBarComponent& toolBar);

        void updateNumPixelsPerBeat (int newNumPixelsPerBeat);

        void checkBounds (juce::Rectangle<int>& bounds
                        , const juce::Rectangle<int>& previousBounds
                        , const juce::Rectangle<int>& limits
                        , bool isStretchingTop
                        , bool isStretchingLeft
                        , bool isStretchingBottom
                        , bool isStretchingRight) override;

    private:
        int mNumPixelsPerBeat;
        const PRToolBarComponent& mToolBar;
    };
}

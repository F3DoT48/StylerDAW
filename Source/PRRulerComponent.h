/*
  ==============================================================================

    PRRulerComponent.h
    Created: 9 Sep 2023 4:23:38pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PRToolBarComponent.h"
#include "ArrangementSection.h"
#include "MeasureRulerComponent.h"

namespace styler_app
{
    class PRRulerComponent  : public juce::Component
                            , private juce::ScrollBar::Listener
    {
    public:
        PRRulerComponent (ArrangementSection::Ptr section
                        , PRToolBarComponent& toolbar
                        , juce::Viewport& gridViewport);
        ~PRRulerComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        std::unique_ptr<juce::Viewport> mRulerViewport;
        std::unique_ptr<MeasureRulerComponent> mMeasureRulerComponent;

        juce::Viewport& mPRGridViewport;

        void scrollBarMoved (juce::ScrollBar*, double) override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PRRulerComponent)
    };
}

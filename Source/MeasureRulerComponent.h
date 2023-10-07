/*
  ==============================================================================

    MeasureRulerComponent.h
    Created: 9 Sep 2023 5:06:45pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PRToolBarComponent.h"
#include "ArrangementSection.h"

namespace styler_app
{
    class MeasureRulerComponent : public juce::Component
    {
    public:
        MeasureRulerComponent (ArrangementSection::Ptr
                             , PRToolBarComponent&);
        ~MeasureRulerComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        static inline constexpr int sDefaultHeightInPixels { 20 };

    private:
        ArrangementSection::Ptr mArrangementSection;
        PRToolBarComponent& mToolBar;


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MeasureRulerComponent)
    };
}

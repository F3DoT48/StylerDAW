/*
  ==============================================================================

    SectionLengthSelectorComponent.h
    Created: 4 Sep 2023 7:04:17pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include "ScrollAndDragSelectorComponent.h"
#include "ArrangementSection.h"

namespace styler_app
{
    class SectionLengthSelectorComponent : public ScrollAndDragSelectorComponent<int>
    {
    public:
        SectionLengthSelectorComponent (ArrangementSection::Ptr arrangementSection);
        ~SectionLengthSelectorComponent() override;

        void onValueChange() override;

    private:
        ArrangementSection::Ptr mArrangementSection;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SectionLengthSelectorComponent)
    };
}

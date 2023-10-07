/*
  ==============================================================================

    SectionLengthSelectorComponent.cpp
    Created: 4 Sep 2023 7:04:18pm
    Author:  User

  ==============================================================================
*/

#include "SectionLengthSelectorComponent.h"

using namespace styler_app;

SectionLengthSelectorComponent::SectionLengthSelectorComponent (ArrangementSection::Ptr arrangementSection)
    : ScrollAndDragSelectorComponent { juce::Range<int> { ArrangementSection::sMinLengthInBars
                                                        , ArrangementSection::sMaxLengthInBars }
                                     , arrangementSection->getLengthInBars()
                                     , 1
                                     , 4 }
    , mArrangementSection { arrangementSection }
{

}

SectionLengthSelectorComponent::~SectionLengthSelectorComponent()
{

}

void SectionLengthSelectorComponent::onValueChange()
{
    mArrangementSection->setLengthInBars (getCurrentValue(), nullptr);
}



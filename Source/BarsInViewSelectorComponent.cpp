/*
  ==============================================================================

    BarsInViewSelectorComponent.cpp
    Created: 19 May 2023 6:54:46pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BarsInViewSelectorComponent.h"
#include "PRMainComponent.h"

using namespace styler_app;

//==============================================================================
BarsInViewSelectorComponent::BarsInViewSelectorComponent (float minBarsdInView
                                                        , float maxBarsInView
                                                        , float stepInBars
                                                        , float startValue)
    : ScrollAndDragSelectorComponent<float> { { minBarsdInView, maxBarsInView }
                                              , startValue
                                              , stepInBars 
                                              , 1.0f }
{

}

BarsInViewSelectorComponent::~BarsInViewSelectorComponent()
{
}

void BarsInViewSelectorComponent::onValueChange()
{
    DBG ("onMouseWheelMove BarsSelector");
    findParentComponentOfClass<PRMainComponent>()->resized();
}

float BarsInViewSelectorComponent::getCurrentNumBarsInView() const
{
    return getCurrentValue();
}

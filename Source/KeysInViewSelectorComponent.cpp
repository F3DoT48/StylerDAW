/*
  ==============================================================================

    KeysInViewSelectorComponent.cpp
    Created: 19 May 2023 6:55:15pm
    Author:  User

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KeysInViewSelectorComponent.h"
#include "PRMainComponent.h"

using namespace styler_app;

//==============================================================================
KeysInViewSelectorComponent::KeysInViewSelectorComponent (float minKeysInView
                                                        , float maxKeysInView
                                                        , float stepInKeys
                                                        , float startValue)
    : ScrollAndDragSelectorComponent<float> { { minKeysInView, maxKeysInView }
                                              , startValue
                                              , stepInKeys
                                              , 1.0f }
{

}

KeysInViewSelectorComponent::~KeysInViewSelectorComponent()
{
}

void KeysInViewSelectorComponent::onValueChange()
{
    DBG ("onMouseWheelMove KeysSelector");
    findParentComponentOfClass<PRMainComponent>()->resized();
}

float KeysInViewSelectorComponent::getCurrentNumKeysInView() const
{
    return getCurrentValue();
}

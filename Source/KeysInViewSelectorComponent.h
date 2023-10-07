/*
  ==============================================================================

    KeysInViewSelectorComponent.h
    Created: 19 May 2023 6:55:15pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ScrollAndDragSelectorComponent.h"

namespace styler_app
{
    class KeysInViewSelectorComponent : public ScrollAndDragSelectorComponent<float>
    {
    public:
        KeysInViewSelectorComponent (float minKeysInView
                                   , float maxKeysInView
                                   , float stepInKeys
                                   , float startValue);
        ~KeysInViewSelectorComponent() override;

        void onValueChange() override;
        float getCurrentNumKeysInView() const;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeysInViewSelectorComponent)
    };
}

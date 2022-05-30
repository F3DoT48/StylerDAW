/*
  ==============================================================================

    StylerTracktionEngineBehaviour.h
    Created: 29 Apr 2022 12:50:53pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace te = tracktion_engine;

namespace styler_app
{
    class StylerTracktionEngineBehaviour : public te::EngineBehaviour
    {
    public:
        StylerTracktionEngineBehaviour();

        te::EditLimits getEditLimits() override;

    private:
        te::EditLimits mEditLimits;
    };
} // namespace styler_app
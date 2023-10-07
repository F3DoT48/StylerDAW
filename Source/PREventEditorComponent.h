/*
  ==============================================================================

    PREventEditorComponent.h
    Created: 3 May 2023 3:05:43pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
namespace styler_app
{
    class PREventEditorComponent  : public juce::Component
    {
    public:
        PREventEditorComponent();
        ~PREventEditorComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PREventEditorComponent)
    };
}

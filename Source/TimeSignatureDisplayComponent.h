/*
  ==============================================================================

    TimeSignatureDisplayComponent.h
    Created: 19 Aug 2023 5:30:29pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace te = tracktion_engine;

namespace styler_app
{
    class TimeSignatureDisplayComponent  : public juce::Component
    {
    public:
        TimeSignatureDisplayComponent (te::TempoSequence& tempoSequence);
        ~TimeSignatureDisplayComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        static inline const juce::StringArray mSignaturesStrings { juce::String {"3/4"}
                                                                 , juce::String {"4/4"}
                                                                 , juce::String {"6/8"} 
                                                                 , juce::String {"12/8"} };

        te::TempoSequence& mTempoSequence;
        juce::ComboBox mComboBox;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TimeSignatureDisplayComponent)
    };
}
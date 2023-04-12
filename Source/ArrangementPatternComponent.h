/*
  ==============================================================================

    ArrangementPatternComponent.h
    Created: 11 Apr 2023 8:05:02pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ArrangementPatternComponent  : public juce::Component
{
public:
    ArrangementPatternComponent();
    ~ArrangementPatternComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementPatternComponent)
};

/*
  ==============================================================================

    ArrangerAreaComponent.h
    Created: 4 Jun 2022 10:37:22pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ArrangerAreaComponent  : public juce::Component
{
public:
    ArrangerAreaComponent();
    ~ArrangerAreaComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangerAreaComponent)
};

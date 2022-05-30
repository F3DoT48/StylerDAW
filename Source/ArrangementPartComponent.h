/*
  ==============================================================================

    ArrangementPartComponent.h
    Created: 3 May 2022 11:34:36am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class ArrangementPartComponent  : public juce::Component
{
public:
    ArrangementPartComponent();
    ~ArrangementPartComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementPartComponent)
};

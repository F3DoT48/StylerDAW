/*
  ==============================================================================

    MixerParametersAreaComponent.h
    Created: 3 May 2022 11:33:26am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MixerParametersAreaComponent  : public juce::Component
{
public:
    MixerParametersAreaComponent();
    ~MixerParametersAreaComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerParametersAreaComponent)
};

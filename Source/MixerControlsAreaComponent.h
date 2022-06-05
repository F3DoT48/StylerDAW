/*
  ==============================================================================

    MixerControlsAreaComponent.h
    Created: 4 Jun 2022 10:37:55pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class MixerControlsAreaComponent  : public juce::Component
{
public:
    MixerControlsAreaComponent();
    ~MixerControlsAreaComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MixerControlsAreaComponent)
};

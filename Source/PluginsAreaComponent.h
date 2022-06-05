/*
  ==============================================================================

    PluginsAreaComponent.h
    Created: 4 Jun 2022 10:37:39pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PluginsAreaComponent  : public juce::Component
{
public:
    PluginsAreaComponent();
    ~PluginsAreaComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginsAreaComponent)
};

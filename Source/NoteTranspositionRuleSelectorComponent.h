/*
  ==============================================================================

    NoteTranspositionRuleSelectorComponent.h
    Created: 11 Sep 2023 11:52:34am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class NoteTranspositionRuleSelectorComponent  : public juce::Component
{
public:
    NoteTranspositionRuleSelectorComponent();
    ~NoteTranspositionRuleSelectorComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoteTranspositionRuleSelectorComponent)
};

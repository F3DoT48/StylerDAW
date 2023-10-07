/*
  ==============================================================================

    InputsAreaComponent.h
    Created: 4 Jun 2022 10:36:47pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "TrackComponent.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class InputsAreaAudioTrackComponent  : public styler_app::TrackComponent
                                         , private te::ValueTreeAllEventListener
    {
    public:
        InputsAreaAudioTrackComponent (EditViewState&, te::Track::Ptr);
        ~InputsAreaAudioTrackComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        te::AudioTrack::Ptr mAudioTrack;

        juce::Label mNameLabel;
        juce::TextButton mDeleteTrackButton;
        juce::TextButton mMidiInputSelectorButton;
        juce::TextButton mAudioInputSelectorButton;

        juce::ValueTree mInputsState;
    
        void valueTreeChanged() override;
        void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InputsAreaAudioTrackComponent)
    };
}

/*
  ==============================================================================

    EditComponent.h
    Created: 2 May 2022 7:50:20pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "EditViewState.h"
//#include "TrackComponent.h"
#include "InputsAreaComponent.h"
#include "PluginsAreaComponent.h"
#include "MixerControlsAreaComponent.h"
#include "Utilities.h"

namespace te = tracktion_engine;

namespace styler_app
{
   class EditComponent : public juce::Component
                        , private te::ValueTreeAllEventListener
                        , private styler_app::FlaggedAsyncUpdater
                        , private juce::ChangeListener
    {
    public:
        EditComponent (te::Edit&
                     , te::SelectionManager&);
        ~EditComponent() override;

        void paint (juce::Graphics&) override;
        void resized () override;

        EditViewState& getEditViewState();

    private:
        te::Edit& mEdit;
        EditViewState mEditViewState;

        juce::OwnedArray<TrackComponent> mAlwaysPresentTracks;
        juce::OwnedArray<InputsAreaComponent> mTrackInputsAreas;
        juce::OwnedArray<PluginsAreaComponent> mTrackPluginsAreas;
        juce::OwnedArray<MixerControlsAreaComponent> mTrackMixerControlsAreas;

        bool mUpdateTracks;

        std::unique_ptr<juce::TextButton> mNewTrackButton;

        void valueTreeChanged() override {}
        void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override {}
        void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override;
        void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override;
        void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override {}

        void handleAsyncUpdate() override;

        void changeListenerCallback (juce::ChangeBroadcaster*) override;

        void buildTracks();
        
        void setupNewTrackButton();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EditComponent)
    };
}

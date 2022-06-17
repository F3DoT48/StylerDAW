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
#include "InputsAreaMasterTrackComponent.h"
#include "InputsAreaTempoTrackComponent.h"
#include "InputsAreaChordTrackComponent.h"
#include "InputsAreaAudioTrackComponent.h"
#include "PluginsAreaMasterTrackComponent.h"
#include "PluginsAreaChordTrackComponent.h"
#include "PluginsAreaTempoTrackComponent.h"
#include "PluginsAreaAudioTrackComponent.h"
#include "MixerControlsAreaMasterTrackComponent.h"
#include "MixerControlsAreaTempoTrackComponent.h"
#include "MixerControlsAreaChordTrackComponent.h"
#include "MixerControlsAreaAudioTrackComponent.h"
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

        /*std::unique_ptr<InputsAreaMasterTrackComponent> mInputsAreaMasterTrack;
        std::unique_ptr<PluginsAreaMasterTrackComponent> mPluginsAreaMasterTrack;
        std::unique_ptr<MixerControlsAreaMasterTrackComponent> mMixerControlsAreaMasterTrack;

        std::unique_ptr<InputsAreaChordTrackComponent> mInputsAreaChordTrack;
        std::unique_ptr<PluginsAreaChordTrackComponent> mPluginsAreaChordTrack;
        std::unique_ptr<MixerControlsAreaChordTrackComponent> mMixerControlsAreaChordTrack;

        std::unique_ptr<InputsAreaTempoTrackComponent> mInputsAreaTempoTrack;
        std::unique_ptr<PluginsAreaTempoTrackComponent> mPluginsAreaTempoTrack;
        std::unique_ptr<MixerControlsAreaTempoTrackComponent> mMixerControlsAreaTempoTrack;*/

        juce::OwnedArray<TrackComponent> mInputsAreasPermanentTracks;
        juce::OwnedArray<TrackComponent> mPluginsAreasPermanentTracks;
        juce::OwnedArray<TrackComponent> mMixerControlsAreasPermanentTracks;

        juce::OwnedArray<TrackComponent> mInputsAreasAudioTracks;
        juce::OwnedArray<TrackComponent> mPluginsAreasAudioTracks;
        juce::OwnedArray<TrackComponent> mMixerControlsAreasAudioTracks;

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

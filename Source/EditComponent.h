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
#include "Arrangement.h"
#include "ArrangementSectionsArea.h"

namespace te = tracktion_engine;

namespace styler_app
{
   class EditComponent : public juce::Component
                       , private te::ValueTreeAllEventListener
                       , private FlaggedAsyncUpdater
                       , private juce::ChangeListener
                       , private juce::ScrollBar::Listener
    {
    public:
        EditComponent (te::Edit&
                     , te::SelectionManager&
                     , Arrangement&
                     , juce::Viewport&);
        ~EditComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        EditViewState& getEditViewState();

        juce::Viewport* getSectionsAreaViewport() const noexcept;

        static constexpr int sArrangementSectionControlsHeightInPixels { 80 };

    private:
        te::Edit& mEdit;
        EditViewState mEditViewState;

        Arrangement& mArrangement;

        juce::Viewport& mArrangementControlsAreaViewport;

        juce::OwnedArray<TrackComponent> mInputsAreasPermanentTracks;
        juce::OwnedArray<TrackComponent> mPluginsAreasPermanentTracks;
        juce::OwnedArray<TrackComponent> mMixerControlsAreasPermanentTracks;

        juce::OwnedArray<TrackComponent> mInputsAreasAudioTracks;
        juce::OwnedArray<TrackComponent> mPluginsAreasAudioTracks;
        juce::OwnedArray<TrackComponent> mMixerControlsAreasAudioTracks;

        std::unique_ptr<ArrangementSectionsArea> mArrangementSectionsArea;
        std::unique_ptr<juce::Viewport> mSectionsAreaViewport;

        bool mUpdateTracks;

        void valueTreeChanged() override {}
        void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override;
        void valueTreeChildAdded (juce::ValueTree&, juce::ValueTree&) override;
        void valueTreeChildRemoved (juce::ValueTree&, juce::ValueTree&, int) override;
        void valueTreeChildOrderChanged (juce::ValueTree&, int, int) override {}

        void handleAsyncUpdate() override;

        void changeListenerCallback (juce::ChangeBroadcaster*) override;

        void buildTracks();

        void scrollBarMoved (juce::ScrollBar*, double) override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditComponent)
    };
}

#pragma once

#include <JuceHeader.h>
#include "EditComponent.h"
#include "CommandIDs.h"
#include "MainComponentMenuBar.h"
#include "Arrangement.h"
#include "ArrangementControlsArea.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class StylerMainComponent : public juce::Component
                              , public juce::ApplicationCommandTarget
                              , private juce::ChangeListener
    {
    public:
        //==============================================================================
        StylerMainComponent();
        ~StylerMainComponent() override;

        //==============================================================================
        void paint(juce::Graphics&) override;
        void resized() override;

        //==============================================================================
        
        ApplicationCommandTarget* getNextCommandTarget() override;
        void getAllCommands (juce::Array<juce::CommandID>&) override;
        void getCommandInfo (juce::CommandID, juce::ApplicationCommandInfo&) override;
        bool perform (const InvocationInfo&) override;

        void setTopLevelComponentName();

        juce::Viewport* getEditComponentViewport() const noexcept;
        juce::Viewport* getArrangementControlsAreaViewport() const noexcept;

        static constexpr int sAreaForButtonsHeight { 30 };

    private:
        te::Engine mTracktionEngine;
        te::SelectionManager mSelectionManager;
        std::unique_ptr<te::Edit> mEdit;
        juce::ApplicationCommandManager mCommandManager;
        std::unique_ptr<MainComponentMenuBar> mMenuBar;
        std::unique_ptr<Arrangement> mArrangement;

        std::unique_ptr<juce::TextButton> mNewTrackButton;
        std::unique_ptr<juce::TextButton> mNewArrangementSectionButton;

        std::unique_ptr<ArrangementControlsArea> mArrangementControlsArea;
        std::unique_ptr<juce::Viewport> mArrangementControlsAreaViewport;

        std::unique_ptr<EditComponent> mEditComponent;
        std::unique_ptr<juce::Viewport> mEditComponentViewport;

        void changeListenerCallback (juce::ChangeBroadcaster*) override;

        void createOrLoadEdit (juce::File);
        void enableAllInputs();

        void setupNewTrackButton();
        void setupNewArrangementSectionButton();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StylerMainComponent)
    };
}

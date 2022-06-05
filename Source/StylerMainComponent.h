#pragma once

#include <JuceHeader.h>
#include "EditComponent.h"
#include "CommandIDs.h"
#include "MainComponentMenuBar.h"

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

    private:
        te::Engine mTracktionEngine;
        te::SelectionManager mSelectionManager;
        std::unique_ptr<te::Edit> mEdit;
        juce::ApplicationCommandManager mCommandManager;

        std::unique_ptr<styler_app::EditComponent> mEditComponent;
        std::unique_ptr<juce::Viewport> mEditComponentViewport;
        std::unique_ptr<styler_app::MainComponentMenuBar> mMenuBar;

        void changeListenerCallback (juce::ChangeBroadcaster*) override;

        void createOrLoadEdit (juce::File);
        void enableAllInputs();

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StylerMainComponent)
    };
}

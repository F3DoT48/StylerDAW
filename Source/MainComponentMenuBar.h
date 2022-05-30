/*
  ==============================================================================

    MainComponentMenuBar.h
    Created: 5 May 2022 12:05:20pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace styler_app
{
    class MainComponentMenuBar  : public juce::Component
                                , public juce::MenuBarModel
    {
    public:
        MainComponentMenuBar (juce::ApplicationCommandManager&);
        ~MainComponentMenuBar() override;

        void paint (juce::Graphics&) override;
        void resized() override;

        juce::StringArray getMenuBarNames() override;
        juce::PopupMenu getMenuForIndex (int, const juce::String&) override;
        void menuItemSelected (int, int) override;

    private:
        juce::ApplicationCommandManager& mCommandManager;

        std::unique_ptr<juce::MenuBarComponent> mPopupMenu;


        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponentMenuBar)
    };
}

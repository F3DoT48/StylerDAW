/*
  ==============================================================================

    StylerUIBehaviour.cpp
    Created: 29 Apr 2022 12:51:15pm
    Author:  User

  ==============================================================================
*/

#include "StylerUIBehaviour.h"

using namespace styler_app;

bool isDPIAware (te::Plugin&)
{
	// You should keep a DB of if plugins are DPI aware or not and recall that value
	// here. You should let the user toggle the value if the plugin appears tiny
	return true;
}

#if JUCE_LINUX
 constexpr bool shouldAddPluginWindowToDesktop = false;
#else
 constexpr bool shouldAddPluginWindowToDesktop = true;
#endif

 

PluginWindow::PluginWindow (te::Plugin& plug)
    : DocumentWindow (plug.getName(), juce::Colours::black, DocumentWindow::closeButton, shouldAddPluginWindowToDesktop),
      plugin (plug), windowState (*plug.windowState)
{
    getConstrainer()->setMinimumOnscreenAmounts (0x10000, 50, 30, 50);

    auto position = plugin.windowState->lastWindowBounds.getPosition();
    setBounds (getLocalBounds() + position);

    setResizeLimits (100, 50, 4000, 4000);
    setBoundsConstrained (getLocalBounds() + position);
    
    recreateEditor();

    #if JUCE_LINUX
     setAlwaysOnTop (true);
     addToDesktop();
    #endif
}

PluginWindow::~PluginWindow()
{
    plugin.edit.flushPluginStateIfNeeded (plugin);
    setEditor (nullptr);
}

void PluginWindow::show()
{
    setVisible (true);
    toFront (false);
    setBoundsConstrained (getBounds());
}

void PluginWindow::setEditor (std::unique_ptr<PluginEditor> newEditor)
{
    JUCE_AUTORELEASEPOOL
    {
        setConstrainer (nullptr);
        editor.reset();

        if (newEditor != nullptr)
        {
            editor = std::move (newEditor);
            setContentNonOwned (editor.get(), true);
        }

        setResizable (editor == nullptr || editor->allowWindowResizing(), false);

        if (editor != nullptr && editor->allowWindowResizing())
            setConstrainer (editor->getBoundsConstrainer());
    }
}

std::unique_ptr<juce::Component> PluginWindow::create (te::Plugin& plugin)
{
    if (auto externalPlugin = dynamic_cast<te::ExternalPlugin*> (&plugin))
        if (externalPlugin->getAudioPluginInstance() == nullptr)
            return nullptr;

    std::unique_ptr<PluginWindow> w;

    {
        struct Blocker : public juce::Component { void inputAttemptWhenModal() override {} };

        Blocker blocker;
        blocker.enterModalState (false);

       #if JUCE_WINDOWS && JUCE_WIN_PER_MONITOR_DPI_AWARE
        if (! isDPIAware (plugin))
        {
            juce::ScopedDPIAwarenessDisabler disableDPIAwareness;
            w = std::make_unique<PluginWindow> (plugin);
        }
        else
       #endif
        {
            w = std::make_unique<PluginWindow> (plugin);
        }
    }

    if (w == nullptr || w->getEditor() == nullptr)
        return {};

    w->show();

    return w;
}

std::unique_ptr<PluginEditor> PluginWindow::createContentComp()
{
    if (auto ex = dynamic_cast<te::ExternalPlugin*> (&plugin))
        return std::make_unique<AudioProcessorEditorContentComp> (*ex);

    return nullptr;
}

void PluginWindow::recreateEditor()
{
    setEditor (nullptr);
    setEditor (createContentComp());
}

void PluginWindow::recreateEditorAsync()
{
    setEditor (nullptr);

    juce::Timer::callAfterDelay (50, [this, sp = SafePointer<Component> (this)]
                                     {
                                         if (sp != nullptr)
                                             recreateEditor();
                                     });
}

void PluginWindow::moved()
{
    plugin.windowState->lastWindowBounds = getBounds();
    plugin.edit.pluginChanged (plugin);
}



std::unique_ptr<juce::Component> StylerUIBehaviour::createPluginWindow (te::PluginWindowState& pws)
{
    if (auto ws = dynamic_cast<te::Plugin::WindowState*> (&pws))
        return PluginWindow::create (ws->plugin);

    return {};
}

void StylerUIBehaviour::recreatePluginWindowContentAsync (te::Plugin& p)
{
    if (auto* w = dynamic_cast<PluginWindow*> (p.windowState->pluginWindow.get()))
        return w->recreateEditorAsync();

    UIBehaviour::recreatePluginWindowContentAsync (p);
}
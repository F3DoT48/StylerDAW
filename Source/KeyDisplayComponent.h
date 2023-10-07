/*
  ==============================================================================

    KeyDisplayComponent.h
    Created: 15 Aug 2023 10:21:20am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Arrangement.h"
//==============================================================================
/*
*/
namespace te = tracktion_engine;

namespace styler_app
{
    class KeyDisplayComponent  : public juce::Component
                               , private juce::ValueTree::Listener
                               , private juce::AsyncUpdater
    {
    public:
        KeyDisplayComponent (Arrangement&);
        ~KeyDisplayComponent() override;

        void paint (juce::Graphics&) override;
        void resized() override;

    private:
        Arrangement& mArrangement;
        juce::Label mKeySymbolLabel;

        void valueTreePropertyChanged (juce::ValueTree& treeWhosePropertyHasChanged
                                     , const juce::Identifier& property) override;

        void handleAsyncUpdate() override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyDisplayComponent)
    };
}

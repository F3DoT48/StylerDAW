/*
  ==============================================================================

    Utilities.h
    Created: 3 May 2022 5:09:29pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace te = tracktion_engine;

namespace styler_app
{
    class FlaggedAsyncUpdater : public juce::AsyncUpdater
    {
    public:
        void markAndUpdate (bool& flag)
        {
            flag = true;
            triggerAsyncUpdate();
        }

        bool compareAndReset (bool& flag) noexcept
        {
            if (! flag)
            {
                return false;
            }

            flag = false;
            return true;
        }
    };

    static inline juce::File findRecentEdit (const juce::File& directory)
    {
        auto files = directory.findChildFiles (juce::File::findFiles, false, "*.edit");

        if (files.size() > 0)
        {
            files.sort();
            return files.getLast();
        }

        return {};
    }
}
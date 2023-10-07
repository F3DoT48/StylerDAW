/*
  ==============================================================================

    ArrangementForAudioThreadGenerator.h
    Created: 6 Sep 2023 5:21:25pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "GarbagePoolOfSharedPtr.h"
#include "Arrangement.h"

namespace styler_app
{
    class ArrangementForAudioThreadGenerator
        : private te::ValueTreeAllEventListener
        , private juce::AsyncUpdater
    {
    public:
        ArrangementForAudioThreadGenerator (Arrangement& arrangement);
        ~ArrangementForAudioThreadGenerator();

        std::shared_ptr<const ArrangementSectionList> getSectionListForAudioThread();

    private:
        Arrangement& mArrangement;
        GarbagePoolOfSharedPtr<const ArrangementSectionList> mGarbagePool;
        std::atomic<std::shared_ptr<const ArrangementSectionList>> mPreparedSectionList;

        void deleteUnusedAndUpdateCurrent();

        void valueTreeChanged() override;

        void handleAsyncUpdate() override;
    };
}
/*
  ==============================================================================

    Arrangement.h
    Created: 20 Nov 2022 6:20:04pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ArrangementSectionList.h"
#include "ArrangementController.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class Arrangement
    {
    public:
        Arrangement (te::Edit&);        

        static juce::ValueTree createArrangement (const te::Edit& edit);

        te::Edit& getEdit();

        juce::ValueTree mState;
        /* !!! hack for te::ValueTreeUtilities */
        juce::ValueTree& state { mState };

        static constexpr int sMaxNumArrangementSections { 20 };

        void addNewSection();
        void removeSection (ArrangementSection*);

        ArrangementSectionList& getAllSections();

        ArrangementController* getPtrToController();

    private:
        te::Edit& mEdit;

        std::unique_ptr<ArrangementSectionList> mSections;
        ArrangementController mController;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Arrangement)
    };
}

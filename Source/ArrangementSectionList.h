/*
  ==============================================================================

    ArrangementSectionList.h
    Created: 5 Apr 2023 8:15:40pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ArrangementSection.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class ArrangementSectionList : public te::ValueTreeObjectList<ArrangementSection>
    {
    public:
        ArrangementSectionList() = delete;
        ArrangementSectionList (te::Edit&, const juce::ValueTree&, juce::UndoManager*);
        ~ArrangementSectionList() override;

        bool isSuitableType (const juce::ValueTree&) const override;
        ArrangementSection* createNewObject (const juce::ValueTree&) override;
        void deleteObject (ArrangementSection*) override;
        void newObjectAdded (ArrangementSection*) override;
        void objectRemoved (ArrangementSection*) override;
        void objectOrderChanged() override;

    private:
        te::Edit& mEdit;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementSectionList)
    };
}
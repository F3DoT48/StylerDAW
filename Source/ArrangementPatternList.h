/*
  ==============================================================================

    ArrangementPatternList.h
    Created: 20 Nov 2022 6:14:10pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "ArrangementPattern.h"

namespace styler_app
{
    class ArrangementPatternList : public te::ValueTreeObjectList<ArrangementPattern>
    {
    public:
        ArrangementPatternList() = delete;
        ArrangementPatternList (te::Edit&
                              //, juce::ReferenceCountedObjectPtr<ArrangementSection> arrangementSection
                              , const juce::ValueTree&
                              , juce::UndoManager*);
        ~ArrangementPatternList() override;

        bool isSuitableType (const juce::ValueTree&) const override;
        ArrangementPattern* createNewObject (const juce::ValueTree&) override;
        void deleteObject (ArrangementPattern*) override;
        void newObjectAdded (ArrangementPattern*) override;
        void objectRemoved (ArrangementPattern*) override;
        void objectOrderChanged() override;

    private:
        te::Edit& mEdit;
        //juce::ReferenceCountedObjectPtr<ArrangementSection> mSection;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ArrangementPatternList)
    };
}

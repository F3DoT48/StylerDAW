/*
  ==============================================================================

    ArrangementPatternList.cpp
    Created: 20 Nov 2022 6:14:10pm
    Author:  User

  ==============================================================================
*/

#include "ArrangementPatternList.h"
#include "ArrangerIDs.h"
//#include "ArrangementSection.h"

using namespace styler_app;

ArrangementPatternList::ArrangementPatternList (te::Edit& edit
                                              //, juce::ReferenceCountedObjectPtr<ArrangementSection> arrangementSection
                                              , const juce::ValueTree& parentState
                                              , juce::UndoManager* undoManager)
    : te::ValueTreeObjectList<ArrangementPattern> {parentState}
    , mEdit {edit}
    //, mSection {arrangementSection}
{
    rebuildObjects();
}

ArrangementPatternList::~ArrangementPatternList()
{
    freeObjects();
}

bool ArrangementPatternList::isSuitableType (const juce::ValueTree& arrPatternState) const
{
    return arrPatternState.hasType (ArrangerIDs::arrangementPattern);
}

ArrangementPattern* ArrangementPatternList::createNewObject (const juce::ValueTree& arrPatternState)
{
    /*ArrangementPattern::Ptr arrPatternPtr {new ArrangementPattern (mEdit
                                                                  //, mSection
                                                                  , arrPatternState
                                                                  , nullptr) };
    arrPatternPtr->incReferenceCount();

    return arrPatternPtr.get();*/

    ArrangementPattern* arrPatternPtr { new ArrangementPattern (mEdit
                                                              , arrPatternState
                                                              , nullptr) };
    arrPatternPtr->incReferenceCount();

    return arrPatternPtr;
}

void ArrangementPatternList::deleteObject (ArrangementPattern* arrPatternPtr)
{
    jassert (arrPatternPtr != nullptr);

    arrPatternPtr->decReferenceCount();
}

void ArrangementPatternList::newObjectAdded (ArrangementPattern*)
{

}

void ArrangementPatternList::objectRemoved (ArrangementPattern*)
{

}

void ArrangementPatternList::objectOrderChanged()
{

}

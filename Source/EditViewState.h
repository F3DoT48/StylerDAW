/*
  ==============================================================================

    EditViewState.h
    Created: 3 May 2022 8:21:50pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace te = tracktion_engine;

namespace styler_app
{
    class EditViewState
    {
    public:
        EditViewState (te::Edit& 
                     , te::SelectionManager&);

        te::Edit& mEdit;
        te::SelectionManager& mSelectionManager;

        //juce::ValueTree mState;
    };
}
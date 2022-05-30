/*
  ==============================================================================

    EditViewState.cpp
    Created: 3 May 2022 8:21:50pm
    Author:  User

  ==============================================================================
*/

#include "EditViewState.h"

using namespace styler_app;

EditViewState::EditViewState (te::Edit& edit
                              , te::SelectionManager& selectionManager)
    : mEdit{ edit }
    , mSelectionManager{ selectionManager }
{

}
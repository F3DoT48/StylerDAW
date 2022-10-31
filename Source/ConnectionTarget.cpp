/*
  ==============================================================================

    ConnectionTarget.cpp
    Created: 16 Oct 2022 11:34:05am
    Author:  User

  ==============================================================================
*/

#include "ConnectionTarget.h"

using namespace styler_app;

ConnectionTarget::ConnectionTarget (const te::EditItemID editItemID
                                  , const int pinIndex
                                  , const bool isInputPin)
    : mEditItemID {editItemID}
    , mPinIndex {pinIndex}
    , mIsInputPin {isInputPin}
{

}

const te::EditItemID& ConnectionTarget::getEditItemID() const noexcept
{
    return mEditItemID;
}

int ConnectionTarget::getPinIndex() const noexcept
{
    return mPinIndex;
}
        
bool ConnectionTarget::isInputPin() const noexcept
{
    return mIsInputPin;
}

bool ConnectionTarget::isOutputPin() const noexcept
{
    return ! isInputPin();
}
/*
  ==============================================================================

    ConnectionTarget.h
    Created: 16 Oct 2022 11:34:05am
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace te = tracktion_engine;

namespace styler_app
{
    class ConnectionTarget
    {
    public:

        ConnectionTarget (const te::EditItemID editItemID
                        , const int pinIndex
                        , const bool isInputPin);

        const te::EditItemID& getEditItemID() const noexcept;

        int getPinIndex() const noexcept;
        
        bool isInputPin() const noexcept;

        bool isOutputPin() const noexcept;

    protected:
        const te::EditItemID mEditItemID;
        const int mPinIndex;
        const bool mIsInputPin;

    private:
        JUCE_DECLARE_NON_COPYABLE(ConnectionTarget)
    };
}
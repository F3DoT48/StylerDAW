/*
  ==============================================================================

    Chord.h
    Created: 20 Aug 2023 4:27:53pm
    Author:  User

  ==============================================================================
*/

#pragma once

namespace styler_app
{
    class Chord
    {
    public:
        enum class Type
        {
            maj
          , min
        };

        Chord (int rootIndex, Type type);

        void setRootIndex (int newRootIndex);
        int getRootIndex() const;

        void setType (Type newType);
        Type getType() const;

    private:
        int mRootIndex;
        Type mType;
    };
}

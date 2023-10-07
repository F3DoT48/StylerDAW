/*
  ==============================================================================

    GarbagePoolOfSharedPtr.h
    Created: 6 Sep 2023 5:07:47pm
    Author:  User

  ==============================================================================
*/

#pragma once

namespace styler_app
{
    template <typename Type>
    class GarbagePoolOfSharedPtr
    {
    public:
        GarbagePoolOfSharedPtr() = default;

        void addSharedInstance (const std::shared_ptr<Type>& newSharedInsance)
        {
            TRACKTION_ASSERT_MESSAGE_THREAD

            mList.push_back (newSharedInsance);
        }

        void cleanUp()
        {
            TRACKTION_ASSERT_MESSAGE_THREAD

            DBG ("Garbage pool size before delete = "  << mList.size());

            auto toEraseIt 
            {
                std::remove_if (mList.begin()
                              , mList.end()
                              , [] (const std::shared_ptr<Type>& elem)
                                {
                                    return elem.use_count() == 1;
                                })
            };

            mList.erase (toEraseIt, mList.end());
            DBG ("Garbage pool size after delete = " << mList.size());
        }

    private:
        std::list<std::shared_ptr<Type>> mList;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GarbagePoolOfSharedPtr)
    };
}
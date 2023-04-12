/*
  ==============================================================================

    MidiList.h
    Created: 29 Mar 2023 5:59:39pm
    Author:  User

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "MidiNote.h"

namespace te = tracktion_engine;

namespace styler_app
{
    namespace
    {
        template<typename VarType>
        inline void convertPropertyToType (juce::ValueTree& valueTree
                                         , const juce::Identifier& identifier)
        {
            if (const auto* prop = valueTree.getPropertyPointer (identifier))
            {
                if (prop->isString())
                {
                    (*const_cast<juce::var*> (prop)) = static_cast<VarType> (*prop);
                }
            }
        }

        template <typename Type>
        static void removeMidiEventFromSelection (Type* event)
        {
            //for (te::SelectionManager::Iterator sm; sm.next();)
            //    if (auto sme = sm->getFirstItemOfType<SelectedMidiEvents>())
            //        sme->removeSelectedEvent (event);
        }

        template <typename Type>
        class GarbagePoolOfSharedPtr
        {
        public:
            GarbagePoolOfSharedPtr() = default;
            ~GarbagePoolOfSharedPtr() = default;

            void addSharedInstance (const std::shared_ptr<Type>& newSharedInsance)
            {
                TRACKTION_ASSERT_MESSAGE_THREAD

                mList.push_back (newSharedInsance);
            }

            void cleanUp()
            {
                TRACKTION_ASSERT_MESSAGE_THREAD

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
            }

        private:
            std::list<std::shared_ptr<Type>> mList;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GarbagePoolOfSharedPtr)
        };
    }

    class MidiSequence : private juce::Timer
    {
    public:
        MidiSequence() = delete;
        MidiSequence (const juce::ValueTree&, juce::UndoManager*);
        ~MidiSequence();

        static juce::ValueTree createMidiSequence (te::MidiChannel midiChannel);

        const juce::Array<MidiNote*>& getNotes() const;

        te::MidiChannel getMidiChannel() const noexcept;
        void setMidiChannel (te::MidiChannel newMidiChannel);

        template <typename Type>
        static void sortMidiEventsByTime (juce::Array<Type>& events)
        {
            std::sort (events.begin()
                     , events.end()
                     , [](const Type& a, const Type& b)
            {
                return a->getStartBeat() < b->getStartBeat();
            });
        }

        void generateAndCacheForAudioThread();

        juce::ValueTree mState;

    private:
        juce::CachedValue<te::MidiChannel> mMidiChannel;

        template <typename EventType>
        class EventDelegate
        {
        public:
            static bool isSuitableType (const juce::ValueTree&);

            /** Return true if the order may have changed. */
            static bool updateObject (EventType&, const juce::Identifier&);

            static void removeFromSelection (EventType*);
        };

        template <typename EventType>
        class EventList : public te::ValueTreeObjectList<EventType>
        {
        public:
            EventList (const juce::ValueTree& valueTree)
              : te::ValueTreeObjectList<EventType> (valueTree)
            {
                te::ValueTreeObjectList<EventType>::rebuildObjects();
            }

            ~EventList() override
            {
                te::ValueTreeObjectList<EventType>::freeObjects();
            }

            EventType* getEventFor (const juce::ValueTree& valueTree)
            {
                for (auto event : te::ValueTreeObjectList<EventType>::objects)
                {
                    if (event->state == valueTree)
                    {
                        return event;
                    }
                }

                return {};
            }

            bool isSuitableType (const juce::ValueTree& valueTree) const override
            {
                return EventDelegate<EventType>::isSuitableType (valueTree);
            }

            EventType* createNewObject (const juce::ValueTree& valueTree) override
            {
                return new EventType (valueTree);
            }

            void deleteObject (EventType* event) override
            {
                delete event;
            }

            void newObjectAdded (EventType* event) override
            {
                triggerSort();
            }

            void objectRemoved (EventType* event) override
            {
                EventDelegate<EventType>::removeFromSelection (event);
                triggerSort();
            }

            void objectOrderChanged() override
            {
                triggerSort();
            }

            void valueTreePropertyChanged (juce::ValueTree& valueTree
                                        , const juce::Identifier& identifier) override
            {
                if (auto event = getEventFor (valueTree))
                {
                    if (EventDelegate<EventType>::updateObject (*event, identifier))
                    {
                        triggerSort();
                    }
                }
            }

            void triggerSort()
            {
                const juce::ScopedLock lock (mCriticalSection);
                mNeedsSorting = true;
            }

            const juce::Array<EventType*>& getSortedList()
            {
                TRACKTION_ASSERT_MESSAGE_THREAD

                const juce::ScopedLock lock (mCriticalSection);

                if (mNeedsSorting)
                {
                    mNeedsSorting = false;
                    mSortedEvents = te::ValueTreeObjectList<EventType>::objects;
                    sortMidiEventsByTime (mSortedEvents);
                }

                return mSortedEvents;
            }

        private:
            bool mNeedsSorting;
            juce::Array<EventType*> mSortedEvents;

            juce::CriticalSection mCriticalSection;

            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EventList)
        };

        static constexpr int sTimerPeriodInMilliseconds { 100 };

        std::unique_ptr<EventList<MidiNote>> mNoteList;

        GarbagePoolOfSharedPtr<MidiSequence> mGarbagePool;
        std::atomic<std::shared_ptr<MidiSequence>> mAtomicSharedForAudioThread; // perhaps <MidiMessageSequence> directly?

        void timerCallback() override;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiSequence)
    };
}

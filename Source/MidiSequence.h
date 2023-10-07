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
#include "ArrangerIDs.h"

namespace te = tracktion_engine;

namespace styler_app
{
    class MidiSequence
    {
    public:
        MidiSequence() = delete;
        MidiSequence (const juce::ValueTree&, juce::UndoManager*);

        static juce::ValueTree createMidiSequence (te::MidiChannel midiChannel);

        const juce::Array<MidiNote*>& getNotes() const;

        te::MidiChannel getMidiChannel() const;
        void setMidiChannel (te::MidiChannel newMidiChannel);

        void addNote (double startBeat
                    , double lengthInBeats
                    , int noteIndex
                    , int velocity
                    , bool isMute
                    , NoteTranspositionRule::Type transpositionRuleType
                    , juce::UndoManager* undoManager);

        void removeNote (MidiNote& note, juce::UndoManager* um);

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
              , mNeedsSorting { true }
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
                return MidiSequence::EventDelegate<EventType>::isSuitableType (valueTree);
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
                MidiSequence::EventDelegate<EventType>::removeFromSelection (event);
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
                    if (MidiSequence::EventDelegate<EventType>::updateObject (*event, identifier))
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
                //TRACKTION_ASSERT_MESSAGE_THREAD

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

        std::unique_ptr<MidiSequence::EventList<MidiNote>> mNoteList;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiSequence)
    };
}

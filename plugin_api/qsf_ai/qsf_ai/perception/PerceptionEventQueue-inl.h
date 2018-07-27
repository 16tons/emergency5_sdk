// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline void PerceptionEventQueue::serialize(BinarySerializer& serializer)
		{
			serializer.serialize(mEvents);
		}

		inline void PerceptionEventQueue::clear()
		{
			mEvents.clear();
		}

		inline void PerceptionEventQueue::push(const PerceptionEvent& perceptionEvent)
		{
			mEvents.push_back(perceptionEvent);
		}

		inline const PerceptionEventContainer& PerceptionEventQueue::getEvents() const
		{
			return mEvents;
		}

		inline uint32 PerceptionEventQueue::getNumEvents() const
		{
			return static_cast<uint32>(mEvents.size());
		}

		inline PerceptionEventQueue::PerceptionEventIterator PerceptionEventQueue::begin(uint32 eventId) const
		{
			return ++PerceptionEventIterator(eventId,mEvents);
		}

		inline PerceptionEventQueue::PerceptionEventIterator PerceptionEventQueue::end() const
		{
			return PerceptionEventIterator();
		}

		// inline methods PerceptionEventQueue::PerceptionEventIterator
		inline PerceptionEventQueue::PerceptionEventIterator::PerceptionEventIterator() :
			mPerceptionEvent(nullptr),
			mIndex(0),
			mEventId(0),
			mEventContainer(nullptr)
		{
		}

		inline PerceptionEventQueue::PerceptionEventIterator::PerceptionEventIterator(uint32 eventId,const PerceptionEventContainer& eventContainer) :
			mPerceptionEvent(nullptr),
			mIndex(0),
			mEventId(eventId),
			mEventContainer(&eventContainer)
		{
		}

		inline const PerceptionEvent* PerceptionEventQueue::PerceptionEventIterator::getPerceptionEvent() const
		{
			return mPerceptionEvent;
		}

		inline const PerceptionEvent& PerceptionEventQueue::PerceptionEventIterator::operator*() const
		{
			return *mPerceptionEvent;
		}

		inline const PerceptionEvent* PerceptionEventQueue::PerceptionEventIterator::operator->() const
		{
			return mPerceptionEvent;
		}

		inline bool PerceptionEventQueue::PerceptionEventIterator::operator==(const PerceptionEventIterator& rho) const
		{
			return mPerceptionEvent==rho.mPerceptionEvent;
		}

		inline bool PerceptionEventQueue::PerceptionEventIterator::operator!=(const PerceptionEventIterator& rho) const
		{
			return mPerceptionEvent!=rho.mPerceptionEvent;
		}

		inline PerceptionEventQueue::PerceptionEventIterator::operator bool() const
		{
			return mPerceptionEvent!=nullptr;
		}

		inline PerceptionEventQueue::PerceptionEventIterator& PerceptionEventQueue::PerceptionEventIterator::operator++()
		{
			if (mEventContainer)
			{
				mPerceptionEvent=nullptr;
				const uint32 containerSize=static_cast<const uint32>(mEventContainer->size());
				while (mIndex<containerSize)
				{
					if (!mEventId || (*mEventContainer)[mIndex].getEventId()==mEventId)
					{
						mPerceptionEvent=&(*mEventContainer)[mIndex++];
						break;
					}
					++mIndex;
				}
			}
			return *this;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

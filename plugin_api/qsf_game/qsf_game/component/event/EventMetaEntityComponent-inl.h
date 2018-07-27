// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <camp/arraymapper.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline EventMetaEntityComponent::EventMetaEntityComponent(Prototype* prototype) :
			Component(prototype),
			EventEntityIdArray(*this, mEventEntityIdArray.entityIdArray)
		{
			// Nothing here
		}

		inline EventMetaEntityComponent::~EventMetaEntityComponent()
		{
			// Nothing here
		}

		inline const std::vector<uint64>& EventMetaEntityComponent::getEntityIdArray() const
		{
			return mEventEntityIdArray.entityIdArray;
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		inline EventMetaEntityComponent::EventEntityIdArrayData::EventEntityIdArrayData()
		{
			// Nothing here
		}


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		inline std::size_t EventMetaEntityComponent::EventEntityIdArrayInterface::size() const
		{
			return getVariable().size();
		}

		inline const uint64& EventMetaEntityComponent::EventEntityIdArrayInterface::get(std::size_t index) const
		{
			return getVariable()[index];
		}

		inline void EventMetaEntityComponent::EventEntityIdArrayInterface::set(std::size_t index, const uint64& value)
		{
			getVariable()[index] = value;
		}

		inline void EventMetaEntityComponent::EventEntityIdArrayInterface::insert(std::size_t before, const uint64& value)
		{
			getVariable().insert(getVariable().begin() + before, value);
		}

		inline void EventMetaEntityComponent::EventEntityIdArrayInterface::remove(std::size_t index)
		{
			getVariable().erase(getVariable().begin() + index);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf

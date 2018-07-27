// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <camp/arraymapper.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BuryComponent::BuryComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mIsHidden(false),
		mDraggable(false),
		mCutableBySaw(false),
		mIsBlocked(false),
		BuriedEntitiesArray(*this, mBuriedEntities)
	{
		// Nothing here
	}

	inline bool BuryComponent::getIsHidden() const
	{
		return mIsHidden;
	}

	inline void BuryComponent::setIsHidden(bool isHidden)
	{
		mIsHidden = isHidden;
	}

	inline bool BuryComponent::getDraggable() const
	{
		return mDraggable;
	}

	inline void BuryComponent::setDraggable(bool draggable)
	{
		mDraggable = draggable;
	}

	inline bool BuryComponent::getCutableBySaw() const
	{
		return mCutableBySaw;
	}

	inline void BuryComponent::setCutableBySaw(bool cutableBySaw)
	{
		mCutableBySaw = cutableBySaw;

		if (isSimulating())
		{
			showCuttableBySawIcon(cutableBySaw);
		}
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t BuryComponent::EntityIdArrayInterface::size() const
	{
		return getVariable().size();
	}

	inline const uint64& BuryComponent::EntityIdArrayInterface::get(std::size_t index) const
	{
		return getVariable()[index];
	}

	inline void BuryComponent::EntityIdArrayInterface::set(std::size_t index, const uint64& value)
	{
		getVariable()[index] = value;
	}

	inline void BuryComponent::EntityIdArrayInterface::insert(std::size_t before, const uint64& value)
	{
		getVariable().insert(getVariable().begin() + before, value);
	}

	inline void BuryComponent::EntityIdArrayInterface::remove(std::size_t index)
	{
		getVariable().erase(getVariable().begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

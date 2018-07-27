// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint64 BoatDropPointComponent::getTransporterEntityId() const
	{
		return mTransporterEntityId;
	}

	inline void BoatDropPointComponent::setTransporterEntityId(uint64 transporterEntityId)
	{
		mTransporterEntityId = transporterEntityId;
	}

	inline uint64 BoatDropPointComponent::getBoatEntityId() const
	{
		return mBoatEntityId;
	}

	inline void BoatDropPointComponent::setBoatEntityId(uint64 boatEntityId)
	{
		mBoatEntityId = boatEntityId;
	}

	inline uint64 BoatDropPointComponent::getLakeId() const
	{
		return mLakeId;
	}

	inline void BoatDropPointComponent::setLakeId(uint64 lakeId)
	{
		mLakeId = lakeId;
	}

	inline size_t BoatDropPointComponent::getDropMarkerIndex() const
	{
		return mDropMarkerIndex;
	}

	inline void BoatDropPointComponent::setDropMarkerIndex(size_t dropMarkerIndex)
	{
		mDropMarkerIndex = dropMarkerIndex;
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t BoatDropPointComponent::EntityIdArrayInterface::size() const
	{
		return getVariable().size();
	}

	inline const uint64& BoatDropPointComponent::EntityIdArrayInterface::get(std::size_t index) const
	{
		return getVariable()[index];
	}

	inline void BoatDropPointComponent::EntityIdArrayInterface::set(std::size_t index, const uint64& value)
	{
		getVariable()[index] = value;
	}

	inline void BoatDropPointComponent::EntityIdArrayInterface::insert(std::size_t before, const uint64& value)
	{
		getVariable().insert(getVariable().begin() + before, value);
	}

	inline void BoatDropPointComponent::EntityIdArrayInterface::remove(std::size_t index)
	{
		getVariable().erase(getVariable().begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

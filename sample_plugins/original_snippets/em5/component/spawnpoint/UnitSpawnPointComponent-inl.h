// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline UnitSpawnPointComponent::UnitSpawnPointComponent(qsf::Prototype* prototype) :
		SpawnPointComponent(prototype),
		mSpawnPointType(UNITSPAWNPOINTTYPE_ENTRY),
		UnitList(*this)
	{
		// Nothing to do in here
	}

	inline UnitSpawnPointComponent::~UnitSpawnPointComponent()
	{
		// Nothing to do in here
	}

	inline UnitSpawnPointComponent::UnitSpawnPointType UnitSpawnPointComponent::getUnitSpawnPointType() const
	{
		return mSpawnPointType;
	}

	inline void UnitSpawnPointComponent::setUnitSpawnPointType(UnitSpawnPointType unitSpawnPointType)
	{
		mSpawnPointType = unitSpawnPointType;
	}

	inline const std::vector<std::string>& UnitSpawnPointComponent::getUnitList() const
	{
		return mList;
	}

	inline void UnitSpawnPointComponent::setUnitList(const std::vector<std::string>& unitlist)
	{
		mList = unitlist;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::SpawnPointComponent methods       ]
	//[-------------------------------------------------------]
	inline bool UnitSpawnPointComponent::isEntry() const
	{
		return (mSpawnPointType == UNITSPAWNPOINTTYPE_ENTRY);
	}

	inline bool UnitSpawnPointComponent::isExit() const
	{
		return (mSpawnPointType == UNITSPAWNPOINTTYPE_EXIT);
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t UnitSpawnPointComponent::UnitListInterface::size() const
	{
		return getOwner().mList.size();
	}

	inline const std::string& UnitSpawnPointComponent::UnitListInterface::get(std::size_t index) const
	{
		return getOwner().mList.at(index);
	}

	inline void UnitSpawnPointComponent::UnitListInterface::set(std::size_t index, const std::string& value)
	{
		getOwner().mList.at(index) = value;
	}

	inline void UnitSpawnPointComponent::UnitListInterface::insert(std::size_t before, const std::string& value)
	{
		getOwner().mList.insert(getOwner().mList.begin() + before, value);
	}

	inline void UnitSpawnPointComponent::UnitListInterface::remove(std::size_t index)
	{
		getOwner().mList.erase(getOwner().mList.begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

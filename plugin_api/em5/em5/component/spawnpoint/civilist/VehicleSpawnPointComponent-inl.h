// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline VehicleSpawnPointComponent::VehicleSpawnPointComponent(qsf::Prototype* prototype) :
		CivilistSpawnPointComponent(prototype),
		mSpawnPointType(VEHICLESPAWNPOINTTYPE_ENTRY),
		VehicleList(*this)
	{
		// Nothing to do in here
	}

	inline VehicleSpawnPointComponent::~VehicleSpawnPointComponent()
	{
		// Nothing to do in here
	}

	inline VehicleSpawnPointComponent::VehicleSpawnPointType VehicleSpawnPointComponent::getVehicleSpawnPointType() const
	{
		return mSpawnPointType;
	}

	inline void VehicleSpawnPointComponent::setVehicleSpawnPointType(VehicleSpawnPointType vehicleSpawnPointType)
	{
		mSpawnPointType = vehicleSpawnPointType;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::SpawnPointComponent methods       ]
	//[-------------------------------------------------------]
	inline bool VehicleSpawnPointComponent::isEntry() const
	{
		return (mSpawnPointType == VEHICLESPAWNPOINTTYPE_ENTRY);
	}

	inline bool VehicleSpawnPointComponent::isExit() const
	{
		return (mSpawnPointType == VEHICLESPAWNPOINTTYPE_EXIT);
	}

	inline bool VehicleSpawnPointComponent::isTransit() const
	{
		return (mSpawnPointType == VEHICLESPAWNPOINTTYPE_TRANSIT);
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t VehicleSpawnPointComponent::VehicleListInterface::size() const
	{
		return getOwner().mList.size();
	}

	inline const std::string& VehicleSpawnPointComponent::VehicleListInterface::get(std::size_t index) const
	{
		return getOwner().mList.at(index);
	}

	inline void VehicleSpawnPointComponent::VehicleListInterface::set(std::size_t index, const std::string& value)
	{
		getOwner().mList.at(index) = value;
	}

	inline void VehicleSpawnPointComponent::VehicleListInterface::insert(std::size_t before, const std::string& value)
	{
		getOwner().mList.insert(getOwner().mList.begin() + before, value);
	}

	inline void VehicleSpawnPointComponent::VehicleListInterface::remove(std::size_t index)
	{
		getOwner().mList.erase(getOwner().mList.begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

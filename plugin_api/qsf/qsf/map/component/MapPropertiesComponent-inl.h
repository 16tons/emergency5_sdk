// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MapPropertiesComponent::~MapPropertiesComponent()
	{
		// Nothing to do in here
	}

	inline const glm::vec3& MapPropertiesComponent::getEventBoundaryTopLeft() const
	{
		return mEventBoundaryTopLeft;
	}

	inline const glm::vec3& MapPropertiesComponent::getEventBoundaryBottomRight() const
	{
		return mEventBoundaryBottomRight;
	}

	inline const glm::vec3& MapPropertiesComponent::getMapBoundaryTopLeft() const
	{
		return mMapBoundaryTopLeft;
	}

	inline const glm::vec3& MapPropertiesComponent::getMapBoundaryBottomRight() const
	{
		return mMapBoundaryBottomRight;
	}

	inline const glm::vec3& MapPropertiesComponent::getBoundingBoxTopLeft() const
	{
		return mBoundingBoxTopLeft;
	}

	inline const glm::vec3& MapPropertiesComponent::getBoundingBoxBottomRight() const
	{
		return mBoundingBoxBottomRight;
	}

	inline float MapPropertiesComponent::getCameraMarginWidth() const
	{
		return mCameraMarginWidth;
	}

	inline uint64 MapPropertiesComponent::getStartCameraEntityId() const
	{
		return mStartCameraEntityId;
	}

	inline void MapPropertiesComponent::setStartCameraEntityId(uint64 startCameraEntityId)
	{
		assignAndPromoteChange(mStartCameraEntityId, startCameraEntityId, START_CAMERA_ENTITY_ID);
	}

	inline int MapPropertiesComponent::getMapTimeHour() const
	{
		return mMapTimeHour;
	}

	inline void MapPropertiesComponent::setMapTimeHour(int mapTimeHour)
	{
		assignAndPromoteChange(mMapTimeHour, mapTimeHour, MAP_TIME_HOUR);
	}

	inline int MapPropertiesComponent::getMapTimeMinutes() const
	{
		return mMapTimeMinutes;
	}

	inline void MapPropertiesComponent::setMapTimeMinutes(int mapTimeMinutes)
	{
		assignAndPromoteChange(mMapTimeMinutes, mapTimeMinutes, MAP_TIME_MINUTES);
	}

	inline int MapPropertiesComponent::getMapTimeSeconds() const
	{
		return mMapTimeSeconds;
	}

	inline void MapPropertiesComponent::setMapTimeSeconds(int mapTimeSeconds)
	{
		assignAndPromoteChange(mMapTimeSeconds, mapTimeSeconds, MAP_TIME_SECONDS);
	}

	inline uint32 MapPropertiesComponent::getPersonSpawnCount() const
	{
		return mPersonSpawnCount;
	}

	inline void MapPropertiesComponent::setPersonSpawnCount(uint32 personSpawnCount)
	{
		assignAndPromoteChange(mPersonSpawnCount, personSpawnCount, PERSON_SPAWN_COUNT_ID);
	}

	inline uint32 MapPropertiesComponent::getVehicleSpawnCount() const
	{
		return mVehicleSpawnCount;
	}

	inline void MapPropertiesComponent::setVehicleSpawnCount(uint32 vehicleSpawnCount)
	{
		assignAndPromoteChange(mVehicleSpawnCount, vehicleSpawnCount, VEHICLE_SPAWN_COUNT_ID);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

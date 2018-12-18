// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Map& GroundMapManager::getMap() const
	{
		return mMap;
	}

	inline GroundMapManagerConfiguration& GroundMapManager::getGroundMapManagerConfiguration()
	{
		return mGroundMapManagerConfiguration;
	}

	inline const GroundMapManagerConfiguration& GroundMapManager::getGroundMapManagerConfiguration() const
	{
		return mGroundMapManagerConfiguration;
	}

	inline GroundMap* GroundMapManager::getGroundMapById(uint32 groundMapId) const
	{
		return findElement(groundMapId);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

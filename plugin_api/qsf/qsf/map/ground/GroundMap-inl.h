// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const GroundMap::Configuration& GroundMap::getConfiguration() const
	{
		return mConfiguration;
	}

	inline GroundMap::Configuration& GroundMap::getConfiguration()
	{
		return mConfiguration;
	}

	inline uint32 GroundMap::getId() const
	{
		return mId;
	}

	inline Map* GroundMap::getMap() const
	{
		return mMap;
	}

	inline bool GroundMap::isValid() const
	{
		return (!mGroundMapLevels.empty() && mWasInitialized);
	}

	inline const GroundMapLevel& GroundMap::getBaseLevel() const
	{
		QSF_CHECK(!mGroundMapLevels.empty(), "No ground map level inside the ground map", QSF_REACT_THROW);
		return mGroundMapLevels.front();
	}

	inline const std::vector<GroundMapLevel>& GroundMap::getLevels() const
	{
		return mGroundMapLevels;
	}

	inline const glm::vec3& GroundMap::getAabbSize() const
	{
		return mAabbSize;
	}

	inline const glm::vec3& GroundMap::getAabbMin() const
	{
		return mAabbMin;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

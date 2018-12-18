// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline GroundMapQuery::~GroundMapQuery()
	{
		// Nothing to do in here
	}

	inline bool GroundMapQuery::getHeightAtXZPosition(float xPosition, float zPosition, float& height, const EntitySelectionManager::IdSet* ignoreEntityIds)
	{
		// Use the more generic internal method
		return getGroundDataAtXZPosition(xPosition, zPosition, &height, nullptr, ignoreEntityIds);
	}

	inline bool GroundMapQuery::getHeightAndNormalAtXZPosition(float xPosition, float zPosition, float& height, glm::vec3& normal, const EntitySelectionManager::IdSet* ignoreEntityIds)
	{
		// Use the more generic internal method
		return getGroundDataAtXZPosition(xPosition, zPosition, &height, &normal, ignoreEntityIds);
	}

	inline bool GroundMapQuery::getTerrainHeightAtXZPosition(float xPosition, float zPosition, float& height)
	{
		// Use the more generic internal method
		return getTerrainDataAtXZPosition(xPosition, zPosition, &height, nullptr);
	}

	inline bool GroundMapQuery::getTerrainHeightAndNormalAtXZPosition(float xPosition, float zPosition, float& height, glm::vec3& normal)
	{
		// Use the more generic internal method
		return getTerrainDataAtXZPosition(xPosition, zPosition, &height, &normal);
	}

	inline bool GroundMapQuery::dropToGround(glm::vec3& position)
	{
		return getHeightAtXZPosition(position.x, position.z, position.y);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

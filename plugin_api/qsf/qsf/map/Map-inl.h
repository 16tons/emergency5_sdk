// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool Map::isRunning() const
	{
		return (nullptr != mOgreSceneManager);
	}

	inline bool Map::isSimulating() const
	{
		return mInternalFlags.isSet(SIMULATING_MODE);
	}

	inline bool Map::isInServerMode() const
	{
		return mInternalFlags.isSet(SERVER_MODE);
	}

	inline GlobalAssetId Map::getGlobalAssetId() const
	{
		return mGlobalAssetId;
	}

	inline void Map::setGlobalAssetId(GlobalAssetId globalAssetId)
	{
		mGlobalAssetId = globalAssetId;
	}

	inline const std::string& Map::getVersionString() const
	{
		return mVersionString;
	}

	inline void Map::setVersionString(const std::string& versionString)
	{
		mVersionString = versionString;
	}

	inline LayerManager& Map::getLayerManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mLayerManager, "The layer manager instance is invalid", QSF_REACT_THROW);
		return *mLayerManager;
	}

	inline GroundMapManager& Map::getGroundMapManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mGroundMapManager, "The ground map manager instance is invalid", QSF_REACT_THROW);
		return *mGroundMapManager;
	}

	inline DebugDrawManager& Map::getDebugDrawManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mDebugDrawManager, "The debug draw manager instance is invalid", QSF_REACT_THROW);
		return *mDebugDrawManager;
	}

	inline void Map::reserveEntities(size_t numberOfEntities)
	{
		reservePrototypes(numberOfEntities);
	}

	inline Entity* Map::getEntityById(uint64 id) const
	{
		// This reinterpret_cast is safe because the map's internal prototype list always contains entities only
		return reinterpret_cast<Entity*>(getPrototypeById(id));
	}

	inline Entity& Map::getEntityByIdSafe(uint64 id) const
	{
		Entity* entity = getEntityById(id);
		QSF_CHECK(nullptr != entity, "There is no entity with ID " << id << " in map " << getId(), QSF_REACT_THROW);
		return *entity;
	}

	inline const Map::EntityHashMapWrapper& Map::getEntities() const
	{
		return mEntityHashMapWrapper;
	}

	template<typename T>
	void Map::getEntitiesByIds(const T& entityIds, std::vector<Entity*>& outEntities, bool filterNullPointers) const
	{
		outEntities.clear();
		outEntities.reserve(entityIds.size());
		if (filterNullPointers)
		{
			for (uint64 id : entityIds)
			{
				Entity* entity = getEntityById(id);
				if (nullptr != entity)
					outEntities.push_back(entity);
			}
		}
		else
		{
			for (uint64 id : entityIds)
			{
				outEntities.push_back(getEntityById(id));
			}
		}
	}

	inline Ogre::SceneManager* Map::getOgreSceneManager() const
	{
		return reinterpret_cast<Ogre::SceneManager*>(mOgreSceneManager);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

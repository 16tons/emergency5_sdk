// Copyright (C) 2012-2018 Promotion Software GmbH


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
	inline LayerManager& Layer::getLayerManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mLayerManager, "The layer manager instance is invalid", QSF_REACT_THROW);
		return *mLayerManager;
	}

	inline Layer* Layer::getParentLayer() const
	{
		return mParentLayer;
	}

	inline uint32 Layer::getId() const
	{
		return mId;
	}

	inline const std::string& Layer::getName() const
	{
		return mName.getName();
	}

	inline uint32 Layer::getNameHash() const
	{
		return mName.getHash();
	}

	inline void Layer::setName(const std::string& name)
	{
		mName = name;
	}

	inline bool Layer::getLoadInGame() const
	{
		return mFlags.isSet(Flags::LOAD_IN_GAME);
	}

	inline void Layer::setLoadInGame(bool loadInGame)
	{
		// Backup the new state
		mFlags.set(Flags::LOAD_IN_GAME, loadInGame);

		// This flag has no direct effect
	}

	inline bool Layer::isHidden() const
	{
		return mFlags.isSet(Flags::HIDDEN);
	}

	inline const std::vector<Layer*>& Layer::getLayers() const
	{
		return mLayers;
	}

	inline void Layer::reserveEntities(size_t numberOfEntities)
	{
		mEntityIds.reserve(numberOfEntities);
	}

	inline const Layer::EntityIdSet& Layer::getEntityIds() const
	{
		return mEntityIds;
	}

	inline void Layer::unsafeSetEntityIds(const EntityIdSet& ids)
	{
		mEntityIds = ids;
	}

	inline bool Layer::isActive() const
	{
		return mFlags.isSet(Flags::ACTIVE);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

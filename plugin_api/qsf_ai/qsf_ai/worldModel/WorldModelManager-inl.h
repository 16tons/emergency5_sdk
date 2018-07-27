// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/worldModel/WorldModel.h"

#include <qsf/base/error/ErrorHandling.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		inline WorldModelManager::WorldModelManager(Prototype* prototype) :
			StandardSystem<DynamicCollisionConverterComponent, WorldModelManager>(prototype, NAME),
			mDebugSettings(nullptr),
			mAssetChangeListener(nullptr)
		{}

		inline WorldModelManager::~WorldModelManager()
		{
			clearPersistentDebugDrawings();
			clearWorldData();
		}

		inline void WorldModelManager::clearWorldData()
		{
			for (Maps::value_type& registered : mModels)
			{
				delete registered.second;
			}

			mModels.clear();
		}

		inline unsigned int WorldModelManager::getNextFreeWorldId() const
		{
			unsigned int nextId = 0;

			while (mModels.find(nextId) != mModels.end())
				++nextId;

			return nextId;
		}

		inline const WorldModelManager::Areas& WorldModelManager::getAreaTypes() const
		{
			return mAreaTypes;
		}

		inline WorldModelManager::AssetChangeListener::~AssetChangeListener()
		{}

		inline void WorldModelManager::setAssetChangeListener(AssetChangeListener* listener)
		{
			// No problem in setting either a nullptr here or overwriting with a new instance here although this is not expected.
			mAssetChangeListener = listener;
		}

		inline bool WorldModelManager::isLegalWorldId(unsigned int id) const
		{
			return mModels.count(id) != 0;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

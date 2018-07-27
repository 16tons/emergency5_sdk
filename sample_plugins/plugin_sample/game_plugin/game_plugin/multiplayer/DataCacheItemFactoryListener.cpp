// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/multiplayer/DataCacheItemFactoryListener.h"
#include "game_plugin/multiplayer/IndicatorData.h"
#include "game_plugin/component/IndicatorComponent.h"

#include <em5/command/component/CommandableComponent.h>

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		DataCacheItemFactoryListener::DataCacheItemFactoryListener()
		{
		}

		DataCacheItemFactoryListener::~DataCacheItemFactoryListener()
		{
		}


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheItemFactory::Listener methods ]
		//[-------------------------------------------------------]
		std::string DataCacheItemFactoryListener::componentIdToString(uint8 componentId)
		{
			if (componentId == ComponentTypeId_Indicator)
				return "indicator";

			return std::string();
		}

		bool DataCacheItemFactoryListener::isEntityCacheRelevant(qsf::Entity& entity)
		{
			// Handling synchronization of the indicator component:
			// As the indicator component can be created at run-time, we have to keep track of all entities that could get it created
			//  -> A good guess would be to track all commandable units, though we could even get more specific here
			if (nullptr != entity.getComponent<em5::CommandableComponent>())
			{
				return true;
			}

			return false;
		}

		void DataCacheItemFactoryListener::createCacheItemsForEntity(qsf::Entity& entity, em5::multiplayer::DataCacheItemFactory::ComponentCacheMap& componentCacheMap)
		{
			// Track the existence and properties of indicator components
			//  -> As denoted above already, we want to track this for all commandables
			if (nullptr != entity.getComponent<em5::CommandableComponent>())
			{
				componentCacheMap.emplace(ComponentTypeId_Indicator, std::unique_ptr<em5::multiplayer::DataCacheBase>(new IndicatorData(entity)));
			}
		}

		void DataCacheItemFactoryListener::createCacheItemsForCoreEntity(const qsf::Entity& entity, em5::multiplayer::DataCacheItemFactory::ComponentCacheMap& componentCacheMap)
		{
			// This is only for the map's core entity, which holds e.g. the map properties.
			// It is not relevant in our case at all, so do nothing here.
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // user

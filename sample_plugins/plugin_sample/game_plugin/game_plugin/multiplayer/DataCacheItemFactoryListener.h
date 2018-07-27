// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <em5/network/multiplayer/datacache/DataCacheItemFactory.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Indicator component data cache for multiplay
		*/
		class DataCacheItemFactoryListener : public em5::multiplayer::DataCacheItemFactory::Listener
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint8 ComponentTypeId_Indicator = (uint8)em5::multiplayer::DataCacheItemFactory::TrackedComponentType::_NumTypes;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			DataCacheItemFactoryListener();
			virtual ~DataCacheItemFactoryListener();


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheItemFactory::Listener methods ]
		//[-------------------------------------------------------]
		public:
			virtual std::string componentIdToString(uint8 componentId);
			virtual bool isEntityCacheRelevant(qsf::Entity& entity);
			virtual void createCacheItemsForEntity(qsf::Entity& entity, em5::multiplayer::DataCacheItemFactory::ComponentCacheMap& componentCacheMap);
			virtual void createCacheItemsForCoreEntity(const qsf::Entity& entity, em5::multiplayer::DataCacheItemFactory::ComponentCacheMap& componentCacheMap);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // user

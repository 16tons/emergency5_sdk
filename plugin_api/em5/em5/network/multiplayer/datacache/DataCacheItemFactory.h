// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <boost/noncopyable.hpp>

#include <memory> // for std::unique_ptr


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Entity;
	namespace game
	{
		class BitStream;
	}
}
namespace em5
{
	namespace multiplayer
	{
		class DataCacheBase;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    EMERGENCY 5 multiplayer data cache item factory
		*/
		class EM5_API_EXPORT DataCacheItemFactory : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			enum class TrackedComponentType : uint8
			{
				Hidden,
				Transform,
				Movable,
				Fire,
				Damage,
				Health,
				Navigation,
				Inventory,
				Link,
				BoneToBoneLink,
				GameLogic,
				TimeOfDay,
				MeshAnimation,
				VehicleCrane,
				Waterjet,
				BoneLink,
				WaterCannon,
				Mesh,
				Door,
				EventId,
				Tree,
				DlkLadder,
				Helicopter,
				Swing,
				Boat,
				Action,
				Ghost,
				BuildingOverlay,
				StatusOverlay,
				Particles,
				Building,
				Person,
				PersonMagnet,
				Vehicle,
				BarrierTape,
				RescueBus,
				SupportLegs,
				UsableByEngineer,
				SpeakToPerson,
				PoliceDogTarget,
				Bury,
				Weather,
				_NumTypes

				// Plugins may add additional component types in order to allow for their own "DataCacheBase" sub-classes.
				// These will not be part of this enum, of course, but have to be defined as constant uint8 values.
				// When doing this, please consider:
				//  - New component type IDs must not interfere with existing ones, so use at least a value of "TrackedComponentType::_NumTypes".
				//  - It is not a requirement to start with "TrackedComponentType::_NumTypes", higher values are okay as well.
				//  - There is a maximum allowed value of 63, as types are sent usings 6 bits.
				//  - Different plugins using the same component type IDs for different custom components will not work together.
			};

			typedef boost::container::flat_map<uint8, std::unique_ptr<DataCacheBase>> ComponentCacheMap;


			/**
			*  @brief
			*    Interface class definition for multiplayer data cache item factory listeners
			*/
			class Listener : public boost::noncopyable
			{
			public:
				virtual std::string componentIdToString(uint8 componentId)	{ return std::string(); }
				virtual bool isEntityCacheRelevant(qsf::Entity& entity)		{ return false; }
				virtual void createCacheItemsForEntity(qsf::Entity& entity, ComponentCacheMap& componentCacheMap)  {}
				virtual void createCacheItemsForCoreEntity(const qsf::Entity& entity, ComponentCacheMap& componentCacheMap)  {}
			};


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static std::string componentIdToString(uint8 componentId);
			static bool isEntityCacheRelevant(qsf::Entity& entity);
			static void createCacheItemsForEntity(qsf::Entity& entity, ComponentCacheMap& componentCacheMap);
			static void createCacheItemsForCoreEntity(const qsf::Entity& entity, ComponentCacheMap& componentCacheMap);

			static void addListener(Listener& listener);
			static void removeListener(Listener& listener);


		//[-------------------------------------------------------]
		//[ Private static data                                   ]
		//[-------------------------------------------------------]
		private:
			static std::vector<Listener*> mListeners;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

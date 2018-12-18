// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/MapCache.h"
#include "em5/network/multiplayer/datacache/DataCacheBase.h"
#include "em5/network/multiplayer/datacache/DataCacheItemFactory.h"
#include "em5/network/multiplayer/datacache/DataCacheItemHelper.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/objects/DivingSpotComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/component/vehicle/parts/WaterCannonComponent.h"
#include "em5/component/vehicle/parts/RescueBusComponent.h"
#include "em5/component/vegetation/TreeComponent.h"
#include "em5/game/Game.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf/component/base/MetadataComponent.h> // Needed to find the sub entities of a paramedic team
#include <qsf/renderer/mesh/MeshComponent.h> // Needed to find the sub entities of a diving spot
#include <qsf/renderer/particles/ParticlesComponent.h> // Needed to find the sub entities of a diving spot
#include <qsf/component/link/LinkComponent.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>

// Enable this, if you want that the value history should save the values to a file on shutdown
//#define MAPCACHE_SAVE_VALUE_HISTORY


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		MapCache::MapCache() :
			mInitialized(false),
			mCurrentMap(nullptr),
			mLastReceivedTickCount(0),
			mWaitTimeBetweenVisibleUpdates(qsf::Time::ZERO),
			mPeekDataPerUpdate(2, 0),
			mAverageDataPerUpdate(2, 0),
			mLowestDataPerUpdate(2, 0),
			mComponentValueHistory(20) // Limit value history to 2 seconds (each entry stands for 50ms)
		{
		}

		MapCache::~MapCache()
		{
			// Nothing to do in here
		}

		void MapCache::setMap(qsf::Map& map)
		{
			mCurrentMap = &map;
			initializeCache();
		}

		void MapCache::addEntity(qsf::Entity& entity)
		{
			// When this method is called the entity is "dynamic"
			addDynamicEntityInternal(entity);

			// TODO(ca) Replace all this special handling by a generic editable LinkComponent property and then edit all relevant prefabs properly (e.g. "IsLinkSynced")

			// Special handling for vehicles, add the special entity when exists
			VehicleComponent* vehicleComponent = entity.getComponent<VehicleComponent>();
			if (nullptr != vehicleComponent)
			{
				// We need here only the water cannon
				RoadVehicleComponent* roadVehicleComponent  = entity.getComponent<RoadVehicleComponent>();
				if (nullptr != roadVehicleComponent)
				{
					WaterCannonComponent* waterCannonComponent = roadVehicleComponent->getWaterCannonComponent();
					if (nullptr != waterCannonComponent)
					{
						addDynamicEntityInternal(waterCannonComponent->getEntity());
					}
				}

				BoatComponent* boatComponent  = entity.getComponent<BoatComponent>();
				if (nullptr != boatComponent)
				{
					WaterCannonComponent* waterCannonComponent = boatComponent->getWaterCannonComponent();
					if (nullptr != waterCannonComponent)
					{
						addDynamicEntityInternal(waterCannonComponent->getEntity());
					}
				}

				{ // Special handling for special entities, which are commandable entities
					qsf::Entity* specialEntity = mCurrentMap->getEntityById(vehicleComponent->getSpecialEntity());
					if (nullptr != specialEntity && nullptr != specialEntity->getComponent<CommandableComponent>())
					{
						addDynamicEntityInternal(*specialEntity);
					}
				}

				// Add door entities
				for (const uint64 doorEntityId : vehicleComponent->getVehicleDoors())
				{
					qsf::Entity* doorEntity = mCurrentMap->getEntityById(doorEntityId);
					if (nullptr != doorEntity)
					{
						addDynamicEntityInternal(*doorEntity);
					}
				}
			}

			// Special handling for team units (e.g. paramedics)
			CommandableComponent* commandableComponent = entity.getComponent<CommandableComponent>();
			if (nullptr != commandableComponent)
			{
				if (commandableComponent->isAmbulanceParamedics())
				{
					// To have working animations and such we need to add also the child entities (in case of paramedics: para1, stretcher, para2)
					// Get the paramedics and the stretcher childs
					qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
					if (nullptr != linkComponent)
					{
						// We test here for tags but it is also possible to use the array position to find the right entity
						for (const qsf::LinkComponent* childComponent : linkComponent->getChildLinks())
						{
							qsf::Entity& childEntity = childComponent->getEntity();

							// Get now the childs from the object
							qsf::LinkComponent* childLinkComponent = childEntity.getOrCreateComponent<qsf::LinkComponent>();
							if (nullptr != childLinkComponent)
							{
								const qsf::MetadataComponent* metadataComponent = childEntity.getOrCreateComponent<qsf::MetadataComponent>();
								// TODO(mk) Here are 3 hardcoded tags, move them to specs file (or something else)
								if (metadataComponent->getTags() == "stretcher")
								{
									addDynamicEntityInternal(childEntity);
								}
								else if (metadataComponent->getTags() == "first_paramedic")
								{
									addDynamicEntityInternal(childEntity);
								}
								else if (metadataComponent->getTags() == "second_paramedic")
								{
									addDynamicEntityInternal(childEntity);
								}
							}
						}
					}
				}
				else if (commandableComponent->isAmbulanceIMT())
				{
					// A child entity of the IMT has the rescue bus component and this component is cache relevant
					qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
					if (nullptr != linkComponent)
					{
						qsf::Component* component = linkComponent->getComponentFromEntityOrLinkedChild<RescueBusComponent>();

						if (nullptr != component)
						{
							addDynamicEntityInternal(component->getEntity());
						}
					}
				}
				else if (commandableComponent->isTechTowcar())
				{
					// It is the tow truck  add the special entity as well -> which is the tarpaulin and is used as replacement for too big to tow items
					RoadVehicleComponent* roadVehicleComponent = entity.getComponent<RoadVehicleComponent>();
					if (nullptr != roadVehicleComponent)
					{
						qsf::Entity* specialEntity = mCurrentMap->getEntityById(roadVehicleComponent->getSpecialEntity());
						if (nullptr != specialEntity)
						{
							// The special entity mesh component is inactive by default. But we don't track active state changes so make the entity active but mark it as hidden
							specialEntity->setActive(true);
							specialEntity->setHidden(true);
							addDynamicEntityInternal(*specialEntity);
						}
					}
				}
			}

			// Special handling for diving spots
			DivingSpotComponent* divingSpotComponent = entity.getComponent<DivingSpotComponent>();
			if (nullptr != divingSpotComponent)
			{
				// We need to sync the particle entity child of the diving spot entity, so the particle effect will be synchronized between clients
				const qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
				if (nullptr != linkComponent)
				{
					for (const qsf::LinkComponent* childComponent : linkComponent->getChildLinks())
					{
						qsf::Entity& childEntity = childComponent->getEntity();

						// We also need to sync the child-entity containing the particles for the particles to correctly turn on and off
						qsf::ParticlesComponent* particlesComponent = childEntity.getComponent<qsf::ParticlesComponent>();
						// We also need to sync the child-entity containing the invisible mesh component, since the status overlay component wants to attach to this one really badly for some reason beyond my understanding even though that just makes things a lot more complicated
						qsf::MeshComponent* meshComponent = childEntity.getComponent<qsf::MeshComponent>();
						if (nullptr != particlesComponent || nullptr != meshComponent)
						{
							addDynamicEntityInternal(childEntity);
						}
					}
				}
			}
		}

		void MapCache::addEntities(const std::unordered_set<uint64>& entityIdList)
		{
			for (const uint64 entityId : entityIdList)
			{
				qsf::Entity* entity = mCurrentMap->getEntityById(entityId);
				if (nullptr != entity)
				{
					addEntity(*entity);
				}
			}
		}

		void MapCache::removeEntity(const uint64 entityId)
		{
			auto iterator = mDataCache.find(entityId);
			if (iterator != mDataCache.end())
			{
				mDataCache.erase(iterator);
			}

			{
				auto findIterator = mEntityMap.find(entityId);
				if (findIterator != mEntityMap.end())
				{
					mEntityMap.erase(findIterator);
				}
			}
		}

		void MapCache::removeEntities(const std::unordered_set<uint64>& entityIdList)
		{
			for (const uint64 entityId : entityIdList)
			{
				removeEntity(entityId);
			}
		}

		void MapCache::updateCache(qsf::game::BitStream& bitStream, const qsf::Clock& clock)
		{
			updateCoreDataCache(bitStream, clock);
			updateDataCache(bitStream, clock);

			updatePerComponentStatistics();

			{ // Update entity visiblity
				if (mWaitTimeBetweenVisibleUpdates <= qsf::Time::ZERO)
				{
					mWaitTimeBetweenVisibleUpdates = qsf::Time::fromMilliseconds(500);

					for (const auto& pair : mEntityMap)
					{
						if (pair.second.valid())
						{
							if (EntityHelper(pair.second.getSafe()).isEntityVisibleForAnyPlayer())
							{
								mVisibleEntities.emplace(pair.first);
							}
							else
							{
								auto findIterator = mVisibleEntities.find(pair.first);
								if (findIterator != mVisibleEntities.end())
								{
									mVisibleEntities.erase(findIterator);
								}
							}
						}
					}
				}
				else
				{
					mWaitTimeBetweenVisibleUpdates -= clock.getTimePassed();
				}
			}
		}

		void MapCache::applyChanges(uint64 dataSize, const qsf::game::BitStream& bitStream)
		{
			// Read tick count
			bool tickChanged;
			if (bitStream.read(tickChanged))
			{
				if (tickChanged)
				{
					bitStream.read(mLastReceivedTickCount);
				}

				mReceivedDataHistoryList.emplace_back();
				auto& item = mReceivedDataHistoryList.back();
				item.tickCount = mLastReceivedTickCount;

				// Read the remaining data minus the already read data = 33 bits (bool + int32 value)
				bitStream.read(item.data, static_cast<size_t>(dataSize - 33));
			}
		}

		bool MapCache::interpolate(const qsf::Clock& clock)
		{
			if (!applyReadData(clock.getSignalCounter()))
			{
				throw std::logic_error("Error occurred while applying the received data");
			}

			const int32 tickDiff = mLastReceivedTickCount - clock.getSignalCounter();
		#ifndef ENDUSER
			mDebugDrawProxy.registerAt(mCurrentMap->getDebugDrawManager());
			mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("Tick difference between host and us: " + std::to_string(tickDiff), glm::vec2(15, 180)));
		#endif

			// Speed up game speed when we are too far behind
			EM5_GAME.setGameSpeed((tickDiff > 2) ? 5.0f : 1.0f);

			if (tickDiff >= 0)
			{
				// Only do interpolation when we have at least one tick left
				const int32 currentTick = clock.getSignalCounter();

				{ // Handle remove entities
					auto iterator = mRemoveEntityHistoryList.begin();
					while (iterator != mRemoveEntityHistoryList.end())
					{
						if (iterator->tickCount == currentTick)
						{
							// The host told that the entity was destroyed as child -> Remove the cache entries
							removeEntityFromCache(iterator->entityId, mDataCache);

							iterator = mRemoveEntityHistoryList.erase(iterator);
						}
						else
						{
							// The data is not for the current tick
							break;
						}
					}
				}

				{ // Interpolate core entity component data
					qsf::Entity& coreEntity = mCurrentMap->getCoreEntity();

					ComponentCacheMap& componentCacheMap = mCoreEntityCache;
					if (!componentCacheMap.empty())
					{
						auto dataCacheItemIterator = componentCacheMap.begin();
						while (dataCacheItemIterator != componentCacheMap.end())
						{
							// Let the cache item do its interpolation job
							dataCacheItemIterator->second->interpolate(coreEntity, clock, mLastReceivedTickCount);

							++dataCacheItemIterator;
						}
					}
					else
					{
						// TODO(sw) ERROR: We have a entity in the cache with an empty component cache map
					}
				}

				auto componentCacheMapIterator = mDataCache.begin();
				while (componentCacheMapIterator != mDataCache.end())
				{
					// TODO(sw) Remove cache entry when entity is gone on the map
					auto findIterator = mEntityMap.find(componentCacheMapIterator->first);
					if (findIterator != mEntityMap.end() && findIterator->second.valid())
					{
						qsf::Entity& entity = findIterator->second.getSafe();
						ComponentCacheMap& componentCacheMap = componentCacheMapIterator->second;
						if (!componentCacheMap.empty())
						{
							auto dataCacheItemIterator = componentCacheMap.begin();
							while (dataCacheItemIterator != componentCacheMap.end())
							{
								// Let the cache item do its interpolation job
								dataCacheItemIterator->second->interpolate(entity, clock, mLastReceivedTickCount);

								++dataCacheItemIterator;
							}
						}
						else
						{
							// TODO(sw) ERROR: We have a entity in the cache with an empty component cache map
						}
					}

					++componentCacheMapIterator;
				}
			}

			// Return indicator if an interpolation is still possible
			return (mLastReceivedTickCount > clock.getSignalCounter());
		}

		void MapCache::logStatistics()
		{
			QSF_LOG_PRINTS(INFO, "Entity counts: cache: "<<mDataCache.size());

			logStatistics(0, "core");
			logStatistics(1, "normal");

			for (const auto& pair : mComponentCount)
			{
				QSF_LOG_PRINTS(INFO, "Mapcache statistics component \""<<DataCacheItemFactory::componentIdToString(pair.first)<<"\" count:\t"<<pair.second);
			}

			logPerComponentStatistics(mPeakPerComponentData, "peak");
			logPerComponentStatistics(mAveragePerComponentData, "average");
			logPerComponentStatistics(mLowestPerComponentData, "lowest");

#if !defined(ENDUSER) && defined(MAPCACHE_SAVE_VALUE_HISTORY)
			mComponentValueHistory.saveToFile();
#endif
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		void MapCache::initializeCache()
		{
			mDataCache.clear();

			mPeakPerComponentData.clear();
			mAveragePerComponentData.clear();
			mLowestPerComponentData.clear();
			mCurrentPerComponentData.clear();
			mComponentCount.clear();

			if (nullptr != mCurrentMap)
			{
				DataCacheItemFactory::createCacheItemsForCoreEntity(mCurrentMap->getCoreEntity(), mCoreEntityCache);

				// Loops through all entities on the map
				for (qsf::Entity* entity : mCurrentMap->getEntities())
				{
					addEntityInternal(*entity);
				}

				for (const auto& componentPair : mCoreEntityCache)
				{
					++mComponentCount[componentPair.first];
				}

				//{ // Only for testing/debugging
				//	QSF_LOG_PRINTS(INFO, "Entity counts after init: normal cache: "<<mDataCache.size() <<" , dynamic: "<<mDynamicCache.size());
				//
				//	size_t max_count = 0;
				//	for (const auto& pair : mDataCache)
				//	{
				//		max_count += pair.second.size();
				//		QSF_LOG_PRINTS(INFO, "Component count in cache after init for entity "<<pair.first <<": "<<pair.second.size());
				//	}

				//	QSF_LOG_PRINTS(INFO, "overall component count in cache after init : "<<max_count);
				//	QSF_LOG_PRINTS(INFO, "average component count in cache after init : "<<max_count/mDataCache.size());

				//	max_count = 0;
				//	for (const auto& pair : mDynamicCache)
				//	{
				//		max_count += pair.second.size();
				//		QSF_LOG_PRINTS(INFO, "Component count in dynamic cache after init for entity "<<pair.first <<": "<<pair.second.size());
				//	}

				//	QSF_LOG_PRINTS(INFO, "overall component count in dynamic cache after init : "<<max_count);
				//	QSF_LOG_PRINTS(INFO, "average component count in dynamic cache after init : "<<max_count/mDynamicCache.size());

				//}

				mInitialized = true;
			}
		}

		void MapCache::addEntityInternal(qsf::Entity& entity)
		{
			if (DataCacheItemFactory::isEntityCacheRelevant(entity))
			{
				addDynamicEntityInternal(entity);
			}
		}

		void MapCache::addDynamicEntityInternal(qsf::Entity& entity)
		{
			uint64 entityId = entity.getId();

			auto iterator = mDataCache.find(entityId);
			if (iterator == mDataCache.end())
			{
				// If cache entries not already created for this entity now create them -> because this is dynamic
				ComponentCacheMap& componentCacheMap = mDataCache[entityId];
				DataCacheItemFactory::createCacheItemsForEntity(entity, componentCacheMap);

				for (const auto& componentPair : componentCacheMap)
				{
					++mComponentCount[componentPair.first];
				}

				mEntityMap.emplace(std::piecewise_construct,
									std::forward_as_tuple(entityId),
									std::forward_as_tuple(entity));

				mVisibleEntities.emplace(entityId);
			}

			// Special handling for trees
			TreeComponent* treeComponent = entity.getComponent<TreeComponent>();
			if (nullptr != treeComponent)
			{
				qsf::Entity* treeTop = treeComponent->getTreeTop();
				if (nullptr != treeTop)
				{
					addDynamicEntityInternal(*treeTop);
				}
			}
		}

		void MapCache::updateCoreDataCache(qsf::game::BitStream& bitStream, const qsf::Clock& clock)
		{
			/*
				Core Entity update binary stream format
				- Indicator if we have any core entity changes (bool)
				- Amount of changed core entity components (uint8)
				- Per component
					- Component id (uint32)
					- Cache data per component (variable)
			*/

			const size_t initialSize = bitStream.getBitLength();

			ChangedComponentCacheMap changedComponents;

			qsf::Entity& coreEntity = mCurrentMap->getCoreEntity();

			int32 currentTickCount = clock.getSignalCounter();

			// First phase gather all items which have changed
			for (auto& componentPair : mCoreEntityCache)
			{
				if (componentPair.second->includeForPrepare(currentTickCount, true))
				{
					if (componentPair.second->prepareForUpdate(coreEntity, clock))
					{
						changedComponents[componentPair.first] = componentPair.second.get();
					}
				}
			}

			// Second phase: Write the changed elements to the stream
			// Write if we have any changes
			bitStream.write(!changedComponents.empty());

			// Only write the data if we have changes
			if (!changedComponents.empty())
			{
				writeChangedComponentsToTheStream(changedComponents, bitStream);
			}

			updateStatistics(0, initialSize, bitStream);
		}

		void MapCache::updateDataCache(qsf::game::BitStream& bitStream, const qsf::Clock& clock)
		{
			/*
				Data update binary stream format
				- Indicator if we have any entity changes (bool)
				- Amount of entity entries (uint32)
				- Per entry
				  - Entity Id (uint64)
				  - Amount of changed components (uint8)
				  - Per component
					- Component id (uint32)
					- Cache data per component (variable)
			*/

			const size_t initialSize = bitStream.getBitLength();

			ChangedEntityCacheMap changedEntities;

			int32 currentTickCount = clock.getSignalCounter();

			// First phase gather all items which have changed
			auto componentCacheMapIterator = mDataCache.begin();
			while (componentCacheMapIterator != mDataCache.end())
			{
				const uint64 entityId = componentCacheMapIterator->first;
				auto findIterator = mEntityMap.find(entityId);
				if (findIterator != mEntityMap.end() && findIterator->second.valid())
				{
					qsf::Entity& entity = findIterator->second.getSafe();
					const bool isVisible = mVisibleEntities.find(findIterator->first) != mVisibleEntities.end();

					ComponentCacheMap& componentCacheMap = componentCacheMapIterator->second;

					for (auto& componentPair : componentCacheMap)
					{
						if (componentPair.second->includeForPrepare(currentTickCount, isVisible))
						{
							if (componentPair.second->prepareForUpdate(entity, clock))
							{
								ChangedComponentCacheMap& list = changedEntities[entityId];
								list[componentPair.first] = componentPair.second.get();
							}
						}
					}

					++componentCacheMapIterator;
				}
				else
				{
					// A destroyed entity has an empty component list
					changedEntities.emplace(std::piecewise_construct,
													std::forward_as_tuple(entityId),
													std::forward_as_tuple()); // We give an empty tuple -> The default constructor is used

					// The entity was destroyed as a child -> remove the cache entry
					componentCacheMapIterator = mDataCache.erase(componentCacheMapIterator);
				}
			}

			// Second phase: Write the changed elements to the stream
			writeChangedEntitiesToTheStream(changedEntities, bitStream, clock);

#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
			QSF_LOG_PRINTS(INFO, "MapCache write changed entities count: " << changedEntities.size());
#endif

			updateStatistics(1, initialSize, bitStream);
		}

		bool MapCache::applyReadData(int32 currentClientTick)
		{
			bool result = true;

			auto iterator = mReceivedDataHistoryList.begin();
			while (iterator != mReceivedDataHistoryList.end())
			{
				if (iterator->tickCount == currentClientTick)
				{
#ifdef ENABLE_MULTIPLAYER_CACHE_DEBUG
				QSF_LOG_PRINTS(INFO, "MapCache applyReadData tick: " << iterator->tickCount);
#endif

					const qsf::game::BitStream& bitStream = iterator->data;

					result = applyCoreChanges(bitStream, currentClientTick);
					if (result)
					{
						result = applyChanges(bitStream, currentClientTick);
					}

					iterator = mReceivedDataHistoryList.erase(iterator);
				}
				else
				{
					// The data is not for the current tick
					break;
				}
			}

			return result;
		}

		bool MapCache::applyCoreChanges(const qsf::game::BitStream& bitStream, int32 currentTick)
		{
			// Read if we have any changes for the core entity
			bool haveCoreEntityChanges = false;
			if (!bitStream.read(haveCoreEntityChanges))
			{
				QSF_ERROR("Could not read if we have changes for the core entity from data update stream. Abort apply changes", QSF_REACT_NONE);
				// Error reading data from stream
				return false;
			}

			if (!haveCoreEntityChanges)
			{
				// We have no changes
				return true;
			}

			// Read component count, we have at least one change
			uint8 componentCount = 0;
			if (!bitStream.read(componentCount))
			{
				QSF_ERROR("Could not read component count for core entity from data update stream. Abort apply changes", QSF_REACT_NONE);
				// Error reading data from stream
				return false;
			}

			// Read all component data
			for (uint8 componentIndex = 0; componentIndex < componentCount; ++ componentIndex)
			{
				// Read componentId
				uint8 componentId = 0;
				if (!readComponentIdFromStream(componentId, bitStream))
				{
					QSF_ERROR("Could not read component id for core entity from data update stream. Abort apply changes", QSF_REACT_NONE);
					// Error reading data from stream
					return false;
				}

				auto findResultIterator = mCoreEntityCache.find(componentId);
				if (findResultIterator == mCoreEntityCache.end())
				{
					QSF_ERROR("The component id \"" << componentId << "\" for core entity is unknown in local cache. Abort apply changes", QSF_REACT_NONE);
					// Error reading data from stream
					return false;
				}

				findResultIterator->second->setData(bitStream, currentTick);
			}
			return true;
		}

		bool MapCache::applyChanges(const qsf::game::BitStream& bitStream, int32 currentTick)
		{
			// Read if we have any changes for the "normal" cache
			bool haveEntityChanges = false;
			if (!bitStream.read(haveEntityChanges))
			{
				QSF_ERROR("Could not read if we have changes for the cache from data update stream. Abort apply changes", QSF_REACT_NONE);
				// Error reading data from stream
				return false;
			}

			if (!haveEntityChanges)
			{
				// We have no changes
				return true;
			}

			// Read entity count
			uint32 entityCount = 0;
			if (!bitStream.read(entityCount))
			{
				QSF_ERROR("Could not read entity count from data update stream. Abort apply changes", QSF_REACT_NONE);
				// Error reading data from stream
				return false;
			}

			QSF_CHECK(entityCount <= mDataCache.size(), "The entity count inside the stream exceeds the amount of entities in the local cache. Abort apply changes", return false);

			// Read data for all entities
			for (uint32 entityIndex = 0; entityIndex < entityCount; ++entityIndex)
			{
				// Read the update data for each entity
				uint64 entityId = 0;
				if (!bitStream.read(entityId))
				{
					QSF_ERROR("Could not read entity id from data update stream. Abort apply changes", QSF_REACT_NONE);
					// Error reading data from stream
					return false;
				}

				// Read component count
				uint8 componentCount = 0;
				if (!bitStream.read(componentCount))
				{
					QSF_ERROR("Could not read component count for entity \"" << entityId << "\" from data update stream. Abort apply changes", QSF_REACT_NONE);
					// Error reading data from stream
					return false;
				}

				if (0 == componentCount)
				{
					// Remove the entry from the cache on the next tick
					RemoveEntityHistory entry;
					entry.tickCount = currentTick + 1;
					entry.entityId = entityId;
					mRemoveEntityHistoryList.emplace_back(entry);
				}
				else
				{
					// Get component cache map and the entity
					auto componentCacheMapIterator = mDataCache.find(entityId);
					if (componentCacheMapIterator != mDataCache.end())
					{
						ComponentCacheMap& componentCacheMap = componentCacheMapIterator->second;

						// Read all component data
						for (uint8 componentIndex = 0; componentIndex < componentCount; ++componentIndex)
						{
							// Read componentId
							uint8 componentId = 0;
							if (!readComponentIdFromStream(componentId, bitStream))
							{
								QSF_ERROR("Could not read component id for entity \"" << entityId << "\" from data update stream. Abort apply changes", QSF_REACT_NONE);
								// Error reading data from stream
								return false;
							}

							auto findResultIterator = componentCacheMap.find(componentId);
							if (findResultIterator == componentCacheMap.end())
							{
								QSF_ERROR("The component id \"" << static_cast<uint16>(componentId) << "\" for entity \"" << entityId << "\" is unknown in local cache. Abort apply changes", QSF_REACT_NONE);
								// Error reading data from stream
								return false;
							}

							findResultIterator->second->setData(bitStream, currentTick);
						}
					}
					else
					{
						QSF_ERROR("Could not find cache entry for entity: "<<entityId<<" abort apply changes", QSF_REACT_NONE);
						return false;
					}
				}
			}

			return true;
		}

		void MapCache::removeEntityFromCache(const uint64 entityId, EntityCacheMap& cacheMap)
		{
			QSF_CHECK(mCurrentMap->getEntityById(entityId) == nullptr, "The host told that an entity was destroyed as child, but the entity exists still on client side: "<<entityId, QSF_REACT_NONE);

			auto iterator = cacheMap.find(entityId);
			if (iterator != cacheMap.end())
			{
				cacheMap.erase(iterator);
			}

			{
				auto findIterator = mEntityMap.find(entityId);
				if (findIterator != mEntityMap.end())
				{
					mEntityMap.erase(findIterator);
				}
			}
		}

		void MapCache::writeComponentIdToStream(const uint8 componentId, qsf::game::BitStream& bitStream)
		{
			// Write component id
			// TODO(sw) We have currently only ~42 components marked for tracking -> we can send the component id with less then 8 bits
			// With 6 bits we support up to 63 components
			bitStream.write(componentId, 6);
		}

		bool MapCache::readComponentIdFromStream(uint8& componentId, const qsf::game::BitStream& bitStream)
		{
			return bitStream.read(componentId, 6);
		}

		void MapCache::writeChangedEntitiesToTheStream(const ChangedEntityCacheMap& changedEntities, qsf::game::BitStream& bitStream, const qsf::Clock& clock)
		{
			// Write information that we have data for changed entities in "normal" cache
			bitStream.write(!changedEntities.empty());

			mChangedComponentIds.clear();

			if (!changedEntities.empty())
			{
				// Write amount of changed entities
				const uint32 entityCount = static_cast<uint32>(changedEntities.size());
				bitStream.write(entityCount);

				// Write entity data
				for (const auto& entityPair : changedEntities)
				{
					// Write entity id
					bitStream.write(entityPair.first);

					writeChangedComponentsToTheStream(entityPair.second, bitStream);
				}
			}

			// With this we support only a tick interval from 1ms to 65000ms ak 65s
			uint16 timePassedInMilliseconds = static_cast<uint16>(clock.getTimePassed().getMilliseconds());
			for (uint8 index = 0; index < static_cast<uint8>(DataCacheItemFactory::TrackedComponentType::_NumTypes); ++index)
			{
				if (std::find(mChangedComponentIds.begin(), mChangedComponentIds.end(), index) == mChangedComponentIds.end())
				{
					mComponentValueHistory.addValue(index, 0, timePassedInMilliseconds);
				}
				else
				{
					mComponentValueHistory.addValue(index, mCurrentPerComponentData[index], timePassedInMilliseconds);
				}
			}
		}

		void MapCache::writeChangedComponentsToTheStream(const ChangedComponentCacheMap& changedComponents, qsf::game::BitStream& bitStream)
		{
			// Write component count
			// We have currently only ~42 components marked for tracking -> size can be send as 8bit
			const uint8 componentCount = static_cast<uint8>(changedComponents.size());
			bitStream.write(componentCount);

			// Write the component data
			for (auto& componentPair : changedComponents)
			{
				if (std::find(mChangedComponentIds.begin(), mChangedComponentIds.end(), componentPair.first) == mChangedComponentIds.end())
				{
					mChangedComponentIds.push_back(componentPair.first);
				}

				// Write component id
				writeComponentIdToStream(componentPair.first, bitStream);

				size_t start = bitStream.getBitLength();

				// Write component data
				componentPair.second->updateData(bitStream);

				updatePerComponentCurrentValue(componentPair.first, start, bitStream);
			}
		}

		void MapCache::updateStatistics(size_t index, uint64 initialSize, const qsf::game::BitStream& bitStream)
		{
			const uint64 diff = bitStream.getBitLength() - initialSize;

			if (diff > mPeekDataPerUpdate[index])
			{
				mPeekDataPerUpdate[index] = diff;
			}

			if (0 == mAverageDataPerUpdate[index])
			{
				mAverageDataPerUpdate[index] = diff;
			}
			else
			{
				mAverageDataPerUpdate[index] += diff;
				mAverageDataPerUpdate[index] /= 2;
			}

			if (diff < mLowestDataPerUpdate[index] || 0 == mLowestDataPerUpdate[index])
			{
				mLowestDataPerUpdate[index] = diff;
			}
		}

		void MapCache::logStatistics(const size_t index, const std::string& prefix)
		{
			QSF_LOG_PRINTS(INFO, "Mapcache statistics for \""<<prefix<<"\" peak per update:    "<<mPeekDataPerUpdate[index]);
			QSF_LOG_PRINTS(INFO, "Mapcache statistics for \""<<prefix<<"\" average per update: "<<mAverageDataPerUpdate[index]);
			QSF_LOG_PRINTS(INFO, "Mapcache statistics for \""<<prefix<<"\" lowest per update:  "<<mLowestDataPerUpdate[index]);
		}

		void MapCache::initPerComponentStatistics(const std::vector<uint8>& list)
		{
			mChangedComponentIds.reserve(static_cast<uint8>(DataCacheItemFactory::TrackedComponentType::_NumTypes));
			for (const uint8 componentId : list)
			{
				mPeakPerComponentData.emplace(componentId, 0);
				mAveragePerComponentData.emplace(componentId, 0);
				mLowestPerComponentData.emplace(componentId, 0);
				mCurrentPerComponentData.emplace(componentId, 0);
				mComponentCount.emplace(componentId, 0);
			}
		}

		void MapCache::updatePerComponentCurrentValue(const uint8 componentId, uint64 initialSize, const qsf::game::BitStream& bitStream)
		{
			mCurrentPerComponentData[componentId] += bitStream.getBitLength() - initialSize;
		}

		void MapCache::updatePerComponentStatistics()
		{
			auto iteratorPair = mCurrentPerComponentData.begin();
			while (iteratorPair != mCurrentPerComponentData.end())
			{
				const uint64 value = iteratorPair->second;
				const uint32 componentId = iteratorPair->first;

				uint64& peak = mPeakPerComponentData[componentId];
				if (value > peak )
				{
					peak = value;
				}

				uint64& average = mAveragePerComponentData[componentId];
				if (0 == average)
				{
					average = value;
				}
				else
				{
					average = (average+value)/2;
				}

				uint64& lowest = mLowestPerComponentData[componentId];
				if (0 == lowest || value < lowest)
				{
					lowest = value;
				}

				iteratorPair->second = 0;

				++iteratorPair;
			}
		}

		void MapCache::logPerComponentStatistics(boost::container::flat_map<uint8, uint64>& dataList, const std::string& prefix)
		{
			for (auto& perComponentPair : dataList)
			{
				QSF_LOG_PRINTS(INFO, "Mapcache statistics "<<prefix<<" per update for component \""<<DataCacheItemFactory::componentIdToString(perComponentPair.first)<<"\"\t"<<perComponentPair.second);
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

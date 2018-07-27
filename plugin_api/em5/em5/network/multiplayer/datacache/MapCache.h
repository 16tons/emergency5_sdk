// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/network/multiplayer/datacache/statistics/MapCacheStatistics.h"

#include <qsf_game/network/BitStream.h>

#include <qsf/base/WeakPtr.h>
#include <qsf/time/Time.h>
#include <qsf/debug/DebugDrawProxy.h>

#include <boost/noncopyable.hpp>

#include <vector>
#include <unordered_map>
#include <memory> // for std::unique_ptr


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
	class Map;
	class Entity;
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
		*    EMERGENCY 5 multiplayer map cache
		*/
		class EM5_API_EXPORT MapCache : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			MapCache();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~MapCache();

			bool isInitialized() { return mInitialized; }

			/**
			*  @brief
			*    Sets the map for which the cache should be used. This causes also the initialization of the cache
			*/
			void setMap(qsf::Map& map);

			//void reset()

			/**
			*  @brief
			*    Adds an entity to the map cache
			*/
			void addEntity(qsf::Entity& entity);

			/**
			*  @brief
			*    Adds a list of entities to the map cache
			*/
			void addEntities(const std::unordered_set<uint64>& entityIdList);

			/**
			*  @brief
			*    Removes an entity from the map cache
			*/
			void removeEntity(const uint64 entityId);

			/**
			*  @brief
			*    Removes a list of entities from the map cache
			*/
			void removeEntities(const std::unordered_set<uint64>& entityIdList);

			/**
			*  @brief
			*    Updates the cache with the data from the map. Any changed data will be written to the bitStream
			*
			*  @param[in] bitStream
			*    The stream to which any changed data will be written to (This stream will contain the data for the "data update" message)
			*
			*  @note
			*    This method gets only called on the host side
			*/
			// TODO(sw) Might be changed, when any bandwith management should not be done inside this class
			void updateCache(qsf::game::BitStream& bitStream, const qsf::Clock& clock);

			/**
			*  @brief
			*    Reads the data from the bitStream and applies any changes to the cache items
			*
			*  @param[in] bitStream
			*    The stream from which the data will be read
			*
			*  @note
			*    This method gets only called on the client side
			*/
			void applyChanges(uint64 dataSize, const qsf::game::BitStream& bitStream);

			/**
			*  @brief
			*    Do any interpolation needed for applying the data from the cache to the entities on the map
			*
			*  @return
			*    "True", when interpolation still possible otherwise false
			*
			*  @note
			*    This method gets only called on the client side
			*/
			bool interpolate(const qsf::Clock& clock);

			void logStatistics();


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			// Don't use here unordered_map it cause troubles on client side (e.g. the fire hose equipment isn't visually linked to the hand of the firefighter. Instead it is visually linked to his feet)
			typedef boost::container::flat_map<uint8, std::unique_ptr<DataCacheBase>> ComponentCacheMap;
			typedef std::unordered_map<uint8, DataCacheBase*> ChangedComponentCacheMap;
			typedef std::unordered_map<uint64, ComponentCacheMap> EntityCacheMap;
			typedef std::unordered_map<uint64, ChangedComponentCacheMap> ChangedEntityCacheMap;
			typedef std::unordered_map<uint64, qsf::WeakPtr<qsf::Entity>> EntityMap;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void initializeCache();
			void addEntityInternal(qsf::Entity& entity);
			void addDynamicEntityInternal(qsf::Entity& entity);

			void updateCoreDataCache(qsf::game::BitStream& bitStream, const qsf::Clock& clock);
			void updateDataCache(qsf::game::BitStream& bitStream, const qsf::Clock& clock);

			bool applyReadData(int32 currentClientTick);
			bool applyCoreChanges(const qsf::game::BitStream& bitStream, int32 currentTick);
			bool applyChanges(const qsf::game::BitStream& bitStream, int32 currentTick);

			void removeEntityFromCache(const uint64 entityId, EntityCacheMap& cacheMap);

			void writeComponentIdToStream(const uint8 componentId, qsf::game::BitStream& bitStream);
			bool readComponentIdFromStream(uint8& componentId, const qsf::game::BitStream& bitStream);
			void writeChangedEntitiesToTheStream(const ChangedEntityCacheMap& changedEntities, qsf::game::BitStream& bitStream, const qsf::Clock& clock);
			void writeChangedComponentsToTheStream(const ChangedComponentCacheMap& changedComponents, qsf::game::BitStream& bitStream);

			// Statistics methods
			void updateStatistics(size_t index, uint64 initialSize, const qsf::game::BitStream& bitStream);
			void logStatistics(const size_t index, const std::string& prefix);
			void initPerComponentStatistics(const std::vector<uint8>& list);
			void updatePerComponentCurrentValue(const uint8 componentId, uint64 initialSize, const qsf::game::BitStream& bitStream);
			void updatePerComponentStatistics();
			void logPerComponentStatistics(boost::container::flat_map<uint8, uint64>& dataList, const std::string& prefix);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool				mInitialized;
			qsf::Map*			mCurrentMap;					///< The current map
			int32				mLastReceivedTickCount;			///< The last tick count from server
			EntityCacheMap		mDataCache;						///< The data cache for the entities
			ComponentCacheMap	mCoreEntityCache;				///< Component cache map for the core entity
			EntityMap			mEntityMap;						///< Holds all entities, which are currently tracked
			std::unordered_set<uint64> mVisibleEntities;
			qsf::Time			mWaitTimeBetweenVisibleUpdates;

			// Statistics
			std::vector<uint64>	mPeekDataPerUpdate;
			std::vector<uint64>	mAverageDataPerUpdate;
			std::vector<uint64>	mLowestDataPerUpdate;
			boost::container::flat_map<uint8, uint64> mPeakPerComponentData;
			boost::container::flat_map<uint8, uint64> mAveragePerComponentData;
			boost::container::flat_map<uint8, uint64> mLowestPerComponentData;
			boost::container::flat_map<uint8, uint64> mCurrentPerComponentData;
			boost::container::flat_map<uint8, uint64> mComponentCount;

			ComponentValueHistory	mComponentValueHistory;
			std::vector<uint8>		mChangedComponentIds;

			struct RemoveEntityHistory
			{
				int32 tickCount;
				uint64 entityId;
			};
			std::vector<RemoveEntityHistory> mRemoveEntityHistoryList;

			struct ReceivedDataHistory
			{
				int32 tickCount;
				qsf::game::BitStream data;
			};

			// We cannot use here an container, which copies elements around, when the container get resized or expectes the elements be copy/movable. Because qsf::game::BitStream is not copy and movable :(
			std::list<ReceivedDataHistory> mReceivedDataHistoryList;

			qsf::DebugDrawProxy mDebugDrawProxy;	///< Actually only for internal builds (additional check to find erroneous behavior), but we need to make sure end-user SDK is the same with or without ENDUSER define


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

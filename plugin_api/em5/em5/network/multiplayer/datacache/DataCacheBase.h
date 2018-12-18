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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
	class Entity;
	namespace game
	{
		class BitStream;
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
		*    EMERGENCY 5 multiplayer data cache base class
		*/
		class EM5_API_EXPORT DataCacheBase : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			DataCacheBase(const qsf::Entity& entity);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~DataCacheBase();

			/**
			*  @brief
			*    Returns if the data cache item should be generally included into the current update cycle
			*/
			virtual bool includeForPrepare(const int32 currentTickCount, bool entityIsVisible);

			/**
			*  @brief
			*    Prepares for an update
			*
			*  @return
			*    Returns true if data has change since last update
			*/
			virtual bool prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock) { return false; }

			/**
			*  @brief
			*    Updates the cache and writes any changed data to the given bit stream
			*/
			virtual void updateData(qsf::game::BitStream& bitStream, bool force = false) = 0;

			/**
			*  @brief
			*    Reads the data from the bit stream
			*/
			virtual void setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount) = 0;

			/**
			*  @brief
			*    Does any interpolation steps needed to apply the cached data to the entity
			*/
			virtual void interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount) = 0;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			uint64	mEntityId;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheBase-inl.h"

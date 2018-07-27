// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheBase.h"

#include <qsf/base/WeakPtr.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class VehicleCraneComponent;
}
namespace qsf
{
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
		*    EMERGENCY 5 entity with a vehicle crane component data cache for a single entity for multiplayer
		*/
		class VehicleCraneData: public DataCacheBase
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			VehicleCraneData(const qsf::Entity& targetEntity);
			virtual ~VehicleCraneData();


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		public:
			virtual bool includeForPrepare(const int32 currentTickCount, bool entityIsVisible) override { return true;}
			virtual bool prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock) override;
			virtual void updateData(qsf::game::BitStream& bitStream, bool force) override;
			virtual void setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount) override;
			virtual void interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			qsf::WeakPtr<VehicleCraneComponent> mComponent;
			glm::vec3	mTargetPosition;
			bool		mTargetPositionChanged;
			uint8		mLastAnimationType;
			bool		mLastAnimationTypeChanged;
			bool		mStrapVisible;
			uint8		mSequenceType;
			bool		mSequenceTypeChanged;
			uint8		mMoveHookDistancePercentage; // It is a percentage value and currently only full percentaqge is used
			bool		mMoveHookDistancePercentageChanged;
			uint64		mEntityIdLinkedToHook;
			bool		mEntityIdLinkedToHookChanged;

			// For client side only
			struct HistoryEntry
			{
				int32 tickCount;
				glm::vec3	mTargetPosition;
				bool		mTargetPositionChanged;
				uint8		mLastAnimationType;
				bool		mLastAnimationTypeChanged;
				bool		mStrapVisible;
				uint8		mSequenceType;
				bool		mSequenceTypeChanged;
				uint8		mMoveHookDistancePercentage; // It is a percentage value and currently only full percentaqge is used
				bool		mMoveHookDistancePercentageChanged;
				uint64		mEntityIdLinkedToHook;
				bool		mEntityIdLinkedToHookChanged;
			};

			std::vector<HistoryEntry>	mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

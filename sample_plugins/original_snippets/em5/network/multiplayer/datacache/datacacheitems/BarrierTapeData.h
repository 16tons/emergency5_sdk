// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheBase.h"
#include "em5/component/objects/BarrierTapeComponent.h"

#include <qsf/base/WeakPtr.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class BarrierTapeComponent;
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
		*    EMERGENCY 5 entity with a barrier tape component data cache for a single entity for multiplayer
		*/
		class BarrierTapeData : public DataCacheBase
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			BarrierTapeData(const qsf::Entity& targetEntity);
			virtual ~BarrierTapeData();


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		public:
			virtual bool prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock) override;
			virtual void updateData(qsf::game::BitStream& bitStream, bool force) override;
			virtual void setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount) override;
			virtual void interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			qsf::WeakPtr<BarrierTapeComponent> mComponent;

			struct PoleData
			{
				bool mHasPositionChanged;
				glm::vec3 mPosition;
				float mTransparency;
			};

			uint32 mNumberOfPoles;
			PoleData mPole[2];

			bool mHasTapeChanged;
			bool mTapeExists;
			BarrierTapeComponent::TapeConnection mTapeConnectionType;
			uint64 mTapeConnectionPoliceEntityId;
			float mTapeTransparency;

			bool mIsFinishedBuilding;
			bool mIsBarrier;

			// For client side only
			struct HistoryEntry
			{
				int32 tickCount;

				uint32 mNumberOfPoles;
				PoleData mPole[2];

				bool mHasTapeChanged;
				bool mTapeExists;
				BarrierTapeComponent::TapeConnection mTapeConnectionType;
				uint64 mTapeConnectionPoliceEntityId;
				float mTapeTransparency;

				bool mIsFinishedBuilding;
				bool mIsBarrier;
			};

			std::vector<HistoryEntry> mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

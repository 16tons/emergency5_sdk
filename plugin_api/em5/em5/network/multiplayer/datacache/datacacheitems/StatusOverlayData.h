// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheComponentExistenceTrackingBase.h"
#include "em5/component/overlay/StatusOverlayComponent.h"


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
		*    EMERGENCY 5 entity with a status overlay component data cache for a single entity for multiplayer
		*/
		class StatusOverlayData: public DataCacheComponentExistenceTrackingBase<StatusOverlayComponent>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			StatusOverlayData(const qsf::Entity& targetEntity);
			virtual ~StatusOverlayData();


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
			bool				mIconFlagsChanged;
			uint64				mIconFlags;
			bool				mBarChanged;
			uint8				mVisibleBars;
			std::unordered_map<uint8, uint8> mCurrentBarValues;
			std::vector<uint16> mChangedBarValues;

			struct HistoryEntry
			{
				int32 tickCount;
				bool		mIconFlagsChanged;
				uint64		mIconFlags;
				bool		mBarChanged;
				uint8		mVisibleBars;
				std::vector<uint16> mChangedBarValues;
			};

			std::vector<HistoryEntry>	mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

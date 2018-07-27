// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheComponentExistenceTrackingBase.h"

#include <qsf/math/Transform.h>
#include <qsf/renderer/animation/MeshAnimationComponent.h>


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
		*    EMERGENCY 5 mesh animation data cache for a single entity for multiplayer
		*/
		class MeshAnimationData: public DataCacheComponentExistenceTrackingBase<qsf::MeshAnimationComponent>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit MeshAnimationData(const qsf::Entity& targetEntity);
			virtual ~MeshAnimationData();


		//[-------------------------------------------------------]
		//[ Public virtual em5::multiplayer::DataCacheBase methods ]
		//[-------------------------------------------------------]
		public:
			virtual bool includeForPrepare(const int32 currentTickCount, bool entityIsVisible) override;
			virtual bool prepareForUpdate(const qsf::Entity& targetEntity, const qsf::Clock& clock) override;
			virtual void updateData(qsf::game::BitStream& bitStream, bool force) override;
			virtual void setData(const qsf::game::BitStream& bitStream, const int32 receivedHostTickCount) override;
			virtual void interpolate(qsf::Entity& targetEntity, const qsf::Clock& clock, const int32 receivedHostTickCount) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			bool checkIfDataHasChanged(const qsf::MeshAnimationComponent::LastAnimatonData& changedAnimationData);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			struct HistoryEntry
			{
				int32 tickCount;
				qsf::MeshAnimationComponent::LastAnimatonData AnimationData;
			};

			qsf::MeshAnimationComponent::LastAnimatonData	mLastAnimationData;
			bool											mDataChanged;
			bool											mAnimationTypeChanged;
			bool											mBlendSyncChanged;
			bool											mBlendTimeChanged;
			bool											mBoneRetargeting1Changed;
			bool											mBoneRetargeting2Changed;
			bool											mLocalAssetId1Changed;
			bool											mLocalAssetId2Changed;
			bool											mBoneMaskChanged;

			std::vector<HistoryEntry>						mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

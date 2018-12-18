// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheComponentExistenceTrackingBase.h"

#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class LinkComponent;
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
		*    EMERGENCY 5 entity with a link component data cache for a single entity for multiplayer
		*/
		class LinkData: public DataCacheComponentExistenceTrackingBase<qsf::LinkComponent>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			LinkData(const qsf::Entity& targetEntity);
			virtual ~LinkData();


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
			uint64		mLinkParentId;
			bool		mLinkParentIdChanged;
			int8		mLinkType;
			bool		mLinkTypeChanged;
			glm::vec3	mLocalPosition;
			bool		mLocalPositionChanged;
			glm::quat	mLocalRotation;
			bool		mLocalRotationChanged;

			struct HistoryEntry
			{
				int32 tickCount;
				uint64		mLinkParentId;
				bool		mLinkParentIdChanged;
				int8		mLinkType;
				bool		mLinkTypeChanged;
				glm::vec3	mLocalPosition;
				bool		mLocalPositionChanged;
				glm::quat	mLocalRotation;
				bool		mLocalRotationChanged;
			};

			std::vector<HistoryEntry>	mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

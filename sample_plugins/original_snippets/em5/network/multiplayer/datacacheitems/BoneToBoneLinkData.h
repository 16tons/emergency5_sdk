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
namespace em5
{
	class FireComponent;
}
namespace qsf
{
	namespace game
	{
		class BoneToBoneLinkComponent;
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
		*    EMERGENCY 5 entity with a bone to bone link component data cache for a single entity for multiplayer
		*/
		class BoneToBoneLinkData: public DataCacheComponentExistenceTrackingBase<qsf::game::BoneToBoneLinkComponent>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			BoneToBoneLinkData(const qsf::Entity& targetEntity);
			virtual ~BoneToBoneLinkData();


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
			glm::quat	mLocalRotation1;
			bool		mLocalRotation1Changed;
			glm::vec3	mLocalPosition1;
			bool		mLocalPosition1Changed;
			std::string	mBoneName;
			bool		mBoneNameChanged;
			std::string	mParentBoneName;
			bool		mParentBoneNameChanged;
			std::string	mChildBoneName1;
			bool		mChildBoneName1Changed;

			glm::quat	mLocalRotation2;
			bool		mLocalRotation2Changed;
			glm::vec3	mLocalPosition2;
			bool		mLocalPosition2Changed;
			std::string	mParentBoneName2;
			bool		mParentBoneName2Changed;
			std::string	mChildBoneName2;
			bool		mChildBoneName2Changed;
			uint64		mParentEntity2;
			bool		mParentEntity2Changed;

			struct HistoryEntry
			{
				int32 tickCount;
				glm::quat	mLocalRotation1;
				bool		mLocalRotation1Changed;
				glm::vec3	mLocalPosition1;
				bool		mLocalPosition1Changed;
				std::string	mBoneName;
				bool		mBoneNameChanged;
				std::string	mParentBoneName;
				bool		mParentBoneNameChanged;
				std::string	mChildBoneName1;
				bool		mChildBoneName1Changed;

				glm::quat	mLocalRotation2;
				bool		mLocalRotation2Changed;
				glm::vec3	mLocalPosition2;
				bool		mLocalPosition2Changed;
				std::string	mParentBoneName2;
				bool		mParentBoneName2Changed;
				std::string	mChildBoneName2;
				bool		mChildBoneName2Changed;
				uint64		mParentEntity2;
				bool		mParentEntity2Changed;
			};

			std::vector<HistoryEntry>	mHistoryList;

		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

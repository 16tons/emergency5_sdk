// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/DataCacheComponentExistenceTrackingBase.h"

#include <qsf/math/BitFlagSet.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class GameLogicComponent;
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
		*    EMERGENCY 5 entity with a game logic componet data cache for a single entity for multiplayer
		*/
		// NOTE: Currently this trackes only data for the CarryPersonLogic (only one instance) there only the information of the carried entity
		class GameLogicData: public DataCacheComponentExistenceTrackingBase<qsf::GameLogicComponent>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			GameLogicData(const qsf::Entity& targetEntity);
			virtual ~GameLogicData();


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
			enum GangsterInBuildingFlags
			{
				GangsterInBuildingLogicExists = 0x01,
				AllowNormalPolice = 0x02,
				CanSpeakWithHostageTaker = 0x04,
				InUseFromSpeaker = 0x08
			};

			uint64	mCarriedPersonId;
			bool	mCarriedPersonIdChanged;
			uint8	mActiveClothing;			// Only needed for firefighters
			bool	mActiveClothingChanged;		// Only needed for firefighters
			qsf::BitFlagSet<uint8, GangsterInBuildingFlags> mGangsterInBuildingFlags;
			bool	mGangsterInBuildingFlagsChanged;
			bool	mIsAreaExtinguishSkillReady;
			bool	mIsAreaExtinguishSkillReadyChanged;

			struct HistoryEntry
			{
				int32 tickCount;
				uint64	mCarriedPersonId;
				bool	mCarriedPersonIdChanged;
				uint8	mActiveClothing;			// Only needed for firefighters
				bool	mActiveClothingChanged;		// Only needed for firefighters
				qsf::BitFlagSet<uint8, GangsterInBuildingFlags> mGangsterInBuildingFlags;
				bool	mGangsterInBuildingFlagsChanged;
				bool	mIsAreaExtinguishSkillReady;
				bool	mIsAreaExtinguishSkillReadyChanged;
			};

			std::vector<HistoryEntry>	mHistoryList;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

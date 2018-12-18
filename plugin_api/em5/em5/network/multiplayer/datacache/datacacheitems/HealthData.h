// Copyright (C) 2012-2018 Promotion Software GmbH


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
	class HealthComponent;
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
		*    EMERGENCY 5 entity with a health component data cache for a single entity for multiplayer
		*/
		class HealthData: public DataCacheBase
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			HealthData(const qsf::Entity& targetEntity);
			virtual ~HealthData();


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
			qsf::WeakPtr<HealthComponent> mComponent;
			bool	mInjuryIdChanged;
			uint32	mInjuryId;				///< The id of the active injury
			bool	mHealthChanged;
			float	mHealthEnergy;			///< Health energy of the person
			bool	mLifeChanged;
			float	mLifeEnergy;			///< Life energy of the person
			bool	mDeathChanged;
			float	mDeathEnergy;			///< Death energy of the person, not reflected to camp
			bool	mStateChanged;
			uint8	mState;
			bool	mIsContaminated;

			bool	mTreatmentActiveChanged;
			bool	mIsTreatmentActive;
			bool	mDiagnosisActiveChanged;
			bool	mIsDiagnosisActive;
			bool	mDiagnosisCompleteChanged;
			bool	mIsDiagnosisComplete;

			// For client side only
			struct HistoryEntry
			{
				int32 tickCount;
				bool	mInjuryIdChanged;
				uint32	mInjuryId;				///< The id of the active injury
				bool	mHealthChanged;
				float	mHealthEnergy;			///< Health energy of the person
				bool	mLifeChanged;
				float	mLifeEnergy;			///< Life energy of the person
				bool	mDeathChanged;
				float	mDeathEnergy;			///< Death energy of the person, not reflected to camp
				bool	mStateChanged;
				uint8	mState;
				bool	mIsContaminated;

				bool	mTreatmentActiveChanged;
				bool	mIsTreatmentActive;
				bool	mDiagnosisActiveChanged;
				bool	mIsDiagnosisActive;
				bool	mDiagnosisCompleteChanged;
				bool	mIsDiagnosisComplete;
			};

			std::vector<HistoryEntry>	mHistoryList;

		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

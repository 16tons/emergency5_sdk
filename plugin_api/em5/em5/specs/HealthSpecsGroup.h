// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/specs/SpecsGroup.h"

#include <qsf/base/UniqueInstance.h>
#include <qsf/reflection/type/CampQsfTime.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Health specs group implementation
	*/
	class EM5_API_EXPORT HealthSpecsGroup : public SpecsGroup, public qsf::UniqueInstance<HealthSpecsGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Pluginable
		static const uint32 PLUGINABLE_ID;				///< "em5::HealthSpecsGroup" unique pluginable specs group ID

		static const uint32 FIREDAMAGE_CONVERT;			///< "FireDamageConvert" unique class property ID inside the class
		static const uint32 INJURYAUDIOTIME_MINIMUM;	///< "InjuryAudioTime_Minimum" unique class property ID inside the class
		static const uint32 INJURYAUDIOTIME_MAXIMUM;	///< "InjuryAudioTime_Maximum" unique class property ID inside the class
		static const uint32 PLAGUEQUICKTESTPOSTIVCHANCE;	///< "PlagueQuickTestPositivChance" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] specsGroupManager
		*    Optional pointer to the specs group manager this specs group should be registered to, can be a null pointer
		*/
		explicit HealthSpecsGroup(SpecsGroupManager* specsGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HealthSpecsGroup();

		float getRegenerationDelta() const;
		void setRegenerationDelta(float regeneration);

		float getReanimationDelta() const;
		void setReanimationDelta(float reanimation);

		float getDeathFightDelta() const;
		void setDeathFightDelta(float deathFightDelta);

		float getReanimationLife() const;
		void setReanimationLife(float lifeEnergy);

		inline float getFireDamageConvert() const;
		void setFireDamageConvert(float fireDamageConvert);

		inline qsf::Time getInjuryAudioTimeMinimum() const;
		void setInjuryAudioTimeMinimum(qsf::Time injuryAudioTimeMinimum);

		inline qsf::Time getInjuryAudioTimeMaximum() const;
		void setInjuryAudioTimeMaximum(qsf::Time injuryAudioTimeMaximum);

		inline float getPlagueQuickTestPositivChance() const;
		void setPlagueQuickTestPositivChance(float plagueQuickTestPositivChance);

		inline float getCQCGangsterPoliceAttackerDamage() const;
		inline void setCQCGangsterPoliceAttackerDamage(float damage);

		inline float getCQCGangsterPoliceVictimDamage() const;
		inline void setCQCGangsterPoliceVictimDamage(float damage);

		inline float getCQCGangsterGangsterAttackerDamage() const;
		inline void setCQCGangsterGangsterAttackerDamage(float damage);

		inline float getCQCGangsterGangsterVictimDamage() const;
		inline void setCQCGangsterGangsterVictimDamage(float damage);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		float		mRegenerationDelta;			///< Health regeneration rate per second when in healthy state
		float		mReanimationDelta;			///< Death fight energy increase rate per second while reanimation in death fight state
		float		mDeathFightDelta;			///< Death fight energy decrease rate per second when not reanimated
		float		mReanimationLife;			///< Initial life energy value after reanimation
		float		mFireDamageConvert;			///< Fire damage convert factor, multiplied in case human gets damage by fire. eg. [1.0]
		qsf::Time	mInjuryAudioTimeMinimum;
		qsf::Time	mInjuryAudioTimeMaximum;
		float		mPlagueQuickTestPositivChance;
		float       mCQCGangsterPoliceAttackerDamage;
		float       mCQCGangsterPoliceVictimDamage;
		float       mCQCGangsterGangsterAttackerDamage;
		float       mCQCGangsterGangsterVictimDamage;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/specs/HealthSpecsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::HealthSpecsGroup)

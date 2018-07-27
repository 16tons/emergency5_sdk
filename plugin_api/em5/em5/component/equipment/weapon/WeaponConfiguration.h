// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/component/equipment/weapon/Weapon.h"

#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace weapon
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		struct EM5_API_EXPORT WeaponConfiguration
		{
			Weapon		mWeaponType;
			float		mDamageHealth;
			float		mDamageVehicle;
			float		mShootRange;
			uint32		mMaxNumberShots;  // 0 for unlimited
			qsf::Time	mTimeToAim;
			qsf::Time	mTimeToWaitAfterShot;

			// Constructor uses the default values for the pistol weapon
			inline explicit WeaponConfiguration(Weapon weaponType = PISTOL) :
				mWeaponType(weaponType),
				mDamageHealth(0.45f),
				mDamageVehicle(0.25f),
				mShootRange(10.0f),
				mMaxNumberShots(0),
				mTimeToAim(qsf::Time::fromSeconds(0.5f)),
				mTimeToWaitAfterShot(qsf::Time::fromSeconds(1.0f))
			{}
		};



//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // weapon
} // em5

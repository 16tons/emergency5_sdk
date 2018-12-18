// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool GameComponent::isInTutorial() const
	{
		return mInTutorial;
	}

	inline bool GameComponent::getDamageSimulationActive() const
	{
		return mDamageSystemActive;
	}

	inline bool GameComponent::getUnitAutomatismActive() const
	{
		return mUnitAutomatismActive;
	}

	inline bool GameComponent::getCivilistBehaviorActive() const
	{
		return mCivilistBehaviorActive;
	}

	inline bool GameComponent::getCivilPersonMovement() const
	{
		return mCivilPersonMovement;
	}

	inline bool GameComponent::getCivilVehiclesDrive() const
	{
		return mCivilVehiclesDrive;
	}

	inline bool GameComponent::getAutoEnteringOfDoctorActive() const
	{
		return mAutoEnteringOfDoctorActive;
	}

	inline bool GameComponent::getShowGroundMarkersForAllOwnUnits() const
	{
		return mShowGroundMarkersForAllOwnUnits;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

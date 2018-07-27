// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline VehicleComponent::VehicleComponent(qsf::Prototype* prototype) :
		DamageComponent(prototype),
		qsf::game::PrototypeContainer(*prototype),
		mCutDuration(qsf::Time::fromSeconds(10.0f)), // TODO(oi) Hardcoded value. Taken from the CutRoadVehicleAction. Original Comment: TODO(mk) Hard coded value - Read timing from setting/specs file
		mIsOnEmergencyOperations(false),
		mLifeEnergy(1.0f),
		mDamageLevel1MaterialActive(false),
		mDamagePrefabActive(false),
		mFireResistance(0.0f),
		mDamageResistance(0.0f),
		mInvincible(false),
		mIsParkingEntity(true),
		mCurrentPrefabState(VehiclePrefabState_Normal),
		mIsSirenEnabled(true),
		mIsSirenSFXSet(false),
		mSpecialEntityId(qsf::getUninitialized<uint64>()),
		mMainDoorType(DoorComponent::DOORTYPE_CODRIVER),
		mSecondsPassedBetweenUpdates(0.0f),
		VehicleDoorsArray(*this, mVehicleDoorsIds),
		DamagedLevel1Particles(*this, mDamagedLevel1ParticleIds),
		DamagedLevel2Particles(*this, mDamagedLevel2ParticleIds),
		DestroyedParticles(*this, mDestroyedParticleIds)
	{
		// By default, vehicles can be repaired when their health is low
		mInternalFlags.set(CAN_BE_REPAIRED, true);
	}

	inline VehicleComponent::~VehicleComponent()
	{
		// Nothing to do in here
	}

	inline uint64 VehicleComponent::getSpecialEntity() const
	{
		return mSpecialEntityId;
	}

	inline DoorComponent::DoorType VehicleComponent::getMainDoorType() const
	{
		return mMainDoorType;
	}

	inline const std::vector<uint64>& VehicleComponent::getVehicleDoors() const
	{
		return mVehicleDoorsIds;
	}

	inline float VehicleComponent::getLifeEnergy() const
	{
		return mLifeEnergy;
	}

	inline bool VehicleComponent::getIsOnEmergencyOperations() const
	{
		return mIsOnEmergencyOperations;
	}

	inline void VehicleComponent::setIsOnEmergencyOperations(bool enable)
	{
		mIsOnEmergencyOperations = enable;
	}

	inline const qsf::AssetProxy& VehicleComponent::getSignalAudioAsset() const
	{
		return mSignalSound;
	}

	inline void VehicleComponent::setSignalAudioAsset(const qsf::AssetProxy& signalSound)
	{
		mSignalSound = signalSound;
		mIsSirenSFXSet = false;
	}

	inline bool VehicleComponent::isSirenEnabled() const
	{
		return mIsSirenEnabled;
	}

	inline bool VehicleComponent::isMovable() const
	{
		// The vehicle is movable when it is not in damage state level 2 or destroyed
		return (!isDamagedLevel2() && !isDestroyed());
	}

	inline bool VehicleComponent::getCanBeTowed() const
	{
		// Is a recovery by the crane vehicle necessary first?
		if (getMustBeRecovered())
			return false;

		// Is it burning?
		if (isBurning())
			return false;

		// The "CAN_BE_TOWED" flag is only an override for actually intact vehicles
		return (isDamagedLevel2() || isDestroyed() || mInternalFlags.isSet(CAN_BE_TOWED)) && !mInternalFlags.isSet(CAN_REALLY_NOT_BE_TOWED);
	}

	inline bool VehicleComponent::getCanBeTowedFlag() const
	{
		return mInternalFlags.isSet(CAN_BE_TOWED);
	}

	inline bool VehicleComponent::getCanReallyNotBeTowed() const
	{
		return mInternalFlags.isSet(CAN_REALLY_NOT_BE_TOWED);
	}

	inline bool VehicleComponent::getMustBeRecovered() const
	{
		return mInternalFlags.isSet(MUST_BE_RECOVERED);
	}

	inline bool VehicleComponent::getIsParkingEntity() const
	{
		return mIsParkingEntity;
	}

	inline void VehicleComponent::setIsParkingEntity(bool isParkingEntity)
	{
		mIsParkingEntity = isParkingEntity;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline VehicleComponent::InternalFlagSet& VehicleComponent::getInternalFlags()
	{
		return mInternalFlags;
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t VehicleComponent::EntityIdArrayInterface::size() const
	{
		return getVariable().size();
	}

	inline const uint64& VehicleComponent::EntityIdArrayInterface::get(std::size_t index) const
	{
		return getVariable()[index];
	}

	inline void VehicleComponent::EntityIdArrayInterface::set(std::size_t index, const uint64& value)
	{
		getVariable()[index] = value;
	}

	inline void VehicleComponent::EntityIdArrayInterface::insert(std::size_t before, const uint64& value)
	{
		getVariable().insert(getVariable().begin() + before, value);
	}

	inline void VehicleComponent::EntityIdArrayInterface::remove(std::size_t index)
	{
		getVariable().erase(getVariable().begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>

#include <camp/arraymapper.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Struct constructors                                   ]
	//[-------------------------------------------------------]
	inline BuildingComponent::DoorData::DoorData() :
		mMainDoorId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	inline BuildingComponent::LightData::LightData() :
		mLightsActive(false)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BuildingComponent::BuildingComponent(qsf::Prototype* prototype) :
		SimpleDamageComponent(prototype),
		PrototypeContainer(*prototype),
		mEvacuateBuildingId(qsf::getUninitialized<uint64>()),
		mClippingEnabled(true),
		mEventWindowId(qsf::getUninitialized<uint64>()),
		mPersonInWindowEntityId(qsf::getUninitialized<uint64>()),
		mEboxRepairTime(qsf::Time::fromSeconds(30.0f)),
		mFirefighterRescueTime(qsf::Time::fromSeconds(2.5f)),
		mCanCreateTrappedPersons(true),
		mSetupNumberOfPersonsTrapped(0),
		mSetupNumberOfInjuredPersonsTrapped(0),
		mTrappedPersonsInjuryId(qsf::getUninitialized<uint32>()),
		mTrappedPersonsDieTimerId(qsf::getUninitialized<uint32>()),
		mDoorParticleEffectId(qsf::getUninitialized<uint64>()),
		mIsDoorBlocked(false),
		mMainDoorIsDamaged(false),
		mCollapseAnimationEntityId(qsf::getUninitialized<uint64>()),
		mCollapseAnimationYDistance(0.0f),
		mCollapseElapsedTime(0.0f),
		mCollapseDampeningFactor(0.6f),
		mCollapseDelay(2.0f),
		mCollapseShowRuinsDelay(0.5f),
		mCollapseDelayTimeout(0.0f),
		mCollapseComplete(false),
		mDamageLevel1MaterialActive(false),
		mBuildingClippingManager(nullptr),
		mUseCameraClipping(false),
		mDirtyMaterialAabb(false),
		mCurrentCameraHeight(0.0f),
		mCurrentFadeOffset(0.0f),
		LightArray(*this, mLights.mLightIds),
		DamagedLevel1EntitiesArray(*this, mDamagedLevel1EntityIds),
		DamagedLevel2EntitiesArray(*this, mDamagedLevel2EntityIds),
		DestroyedEntitiesArray(*this, mDestroyedEntityIds),
		CollapseRubbleArray(*this, mCollapseRubbleEntityIds),
		EboxEntitiesArray(*this, mEboxEntityIds)
	{
		mInternalFlags.set(MESH_ACTIVE);
	}

	inline BuildingComponent::~BuildingComponent()
	{
		// Noting here
	}

	inline bool BuildingComponent::getClippingEnabled() const
	{
		return mClippingEnabled;
	}

	inline void BuildingComponent::setClippingEnabled(bool clippingEnabled)
	{
		assignAndPromoteChange(mClippingEnabled, clippingEnabled, CLIPPING_ENABLED);
	}

	inline uint16 BuildingComponent::getSetupNumberOfTrappedPersons() const
	{
		return mSetupNumberOfPersonsTrapped;
	}

	inline uint16 BuildingComponent::getSetupNumberOfTrappedInjuredPersons() const
	{
		return mSetupNumberOfInjuredPersonsTrapped;
	}

	inline bool BuildingComponent::isPersonWavingOutOfWindow() const
	{
		return qsf::isInitialized(mPersonInWindowEntityId);
	}

	inline const qsf::AssetProxy& BuildingComponent::getDamagedLevel1Material() const
	{
		return mDamagedLevel1Material;
	}

	inline const qsf::AssetProxy& BuildingComponent::getIntactMaterial() const
	{
		return mIntactMaterial;
	}

	inline float BuildingComponent::getCollapseDampeningFactor() const
	{
		return mCollapseDampeningFactor;
	}

	inline void BuildingComponent::setCollapseDampeningFactor(float collapseDampeningFactor)
	{
		mCollapseDampeningFactor = collapseDampeningFactor;
	}

	inline float BuildingComponent::getCollapseDelay() const
	{
		return mCollapseDelay;
	}

	inline void BuildingComponent::setCollapseDelay(float collapseDelay)
	{
		mCollapseDelay = collapseDelay;
	}

	inline float BuildingComponent::getCollapseShowRuinsDelay() const
	{
		return mCollapseShowRuinsDelay;
	}

	inline void BuildingComponent::setCollapseShowRuinsDelay(float collapseShowRuinsDelay)
	{
		mCollapseShowRuinsDelay = collapseShowRuinsDelay;
	}

	inline bool BuildingComponent::isClipped() const
	{
		return mUseCameraClipping;
	}

	inline bool BuildingComponent::getIsDoorBlocked() const
	{
		return mIsDoorBlocked;
	}

	inline void BuildingComponent::setIsDoorBlocked(bool isDoorBlocked)
	{
		mIsDoorBlocked = isDoorBlocked;
	}

	inline qsf::Time BuildingComponent::getEboxRepairTime() const
	{
		return mEboxRepairTime;
	}

	inline void BuildingComponent::setEboxRepairTime(qsf::Time time)
	{
		mEboxRepairTime = time;
	}

	inline qsf::Time BuildingComponent::getFirefighterRescueTime() const
	{
		return mFirefighterRescueTime;
	}

	inline void BuildingComponent::setFirefighterRescueTime(qsf::Time time)
	{
		mFirefighterRescueTime = time;
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t BuildingComponent::EntityIdArrayInterface::size() const
	{
		return getVariable().size();
	}

	inline const uint64& BuildingComponent::EntityIdArrayInterface::get(std::size_t index) const
	{
		return getVariable()[index];
	}

	inline void BuildingComponent::EntityIdArrayInterface::set(std::size_t index, const uint64& value)
	{
		getVariable()[index] = value;
	}

	inline void BuildingComponent::EntityIdArrayInterface::insert(std::size_t before, const uint64& value)
	{
		getVariable().insert(getVariable().begin() + before, value);
	}

	inline void BuildingComponent::EntityIdArrayInterface::remove(std::size_t index)
	{
		getVariable().erase(getVariable().begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

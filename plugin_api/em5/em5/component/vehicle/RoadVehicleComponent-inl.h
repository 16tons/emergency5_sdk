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
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline RoadVehicleComponent::RoadVehicleComponent(qsf::Prototype* prototype) :
		VehicleComponent(prototype),
		mFrontWheelDirection(0.0f),
		mChainAngle(0.0f),
		mLightTypeToEntityIdsMapInitialized(false),
		mBlinkerState(BLINKERS_OFF),
		mBlinkerTime(qsf::Time::ZERO),
		mBlueLightActive(false),
		mBlueLightTime(0.0f),
		mIsBrakeLightActive(false),
		mBrakeLightTimeout(qsf::Time::ZERO),
		mIsHeadLightActive(false),
		mSpecialEntityId(qsf::getUninitialized<uint64>()),
		mIsCommandable(false),
		mTooBigToTow(false),
		mCanSpawnDriver(false),
		VehicleWheelsArray(*this, mVehicleWheelIds),
		VehicleLightsArray(*this, mVehicleLightIds),
		StateDamagedParticles(*this, mStateDamagedParticleIds),
		StateHeavilyDamagedParticles(*this, mStateHeavilyDamagedParticleIds),
		StateExplodedParticles(*this, mStateExplodedParticleIds)
	{
		// Nothing to do in here
	}

	inline RoadVehicleComponent::~RoadVehicleComponent()
	{
		// Nothing to do in here
	}

	inline void RoadVehicleComponent::setFrontWheelDirection(float degrees)
	{
		mFrontWheelDirection = degrees;
	}

	inline bool RoadVehicleComponent::getTooBigToTow() const
	{
		return mTooBigToTow;
	}

	inline void RoadVehicleComponent::setTooBigToTow(bool tooBigToTow)
	{
		mTooBigToTow = tooBigToTow;
	}

	inline bool RoadVehicleComponent::getCanSpawnDriver() const
	{
		return mCanSpawnDriver;
	}

	inline void RoadVehicleComponent::setCanSpawnDriver(bool canSpawnDriver)
	{
		mCanSpawnDriver = canSpawnDriver;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

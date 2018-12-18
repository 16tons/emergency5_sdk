// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline VehicleCraneComponent::VehicleCraneComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mCraneEntityId(qsf::getUninitialized<uint64>()),
		mDistancePercentage(0.0f),
		mTimePassed(qsf::Time::ZERO),
		mPauseTimePassed(qsf::Time::ZERO),
		mPauseTime(qsf::Time::fromSeconds(0.5f)),
		mSequenceState(0),
		mSequenceType(SEQUENCE_NONE),
		mLastAnimationState(STATE_NONE),
		mStrapVisible(false),
		mEntityIdLinkedToHook(qsf::getUninitialized<uint64>()),
		mCraneEntity(nullptr),
		mJibEntity(nullptr),
		mCockpitEntity(nullptr),
		mHookDummy(nullptr),
		mCraneTransformComponent(nullptr),
		mCraneLinkComponent(nullptr),
		mCraneMeshAnimationComponent(nullptr),
		mJibMeshAnimationComponent(nullptr),
		mCockpitMeshAnimationComponent(nullptr),
		mStageBoneToBoneLinkComponent(nullptr),
		mExtendAnimationChannel(nullptr),
		mLowToHighAnimationChannel(nullptr),
		mCockpitAnimationChannel(nullptr),
		mRotateYawAngle(0.0f),
		mExtendAnimationOffset(0.0f),
		mLowToHighAnimationOffset(0.0f),
		mCockpitAnimationOffset(0.0f),
		mDistance(0.0f),
		mMinLength(5.0f),
		mMaxLength(10.0f),
		mMinAngle(-2.0f),
		mMaxAngle(0.0f),
		mCraneHeight(1.65f),
		mArmRotateSpeed(0.1f),
		mExtendSpeed(0.1f),
		mLowToHighSpeed(0.1f),
		mHookSpeed(0.1f),
		mCockpitSpeed(0.1f),
		mTimeUltimatum(30.0f),
		mCraneType(VehicleCraneComponent::TOW_CAR)
	{
		// Nothing to do in here
	}

	inline VehicleCraneComponent::~VehicleCraneComponent()
	{
		// Nothing to do in here
	}

	inline VehicleCraneComponent::SequenceType VehicleCraneComponent::getSequenceType() const
	{
		return mSequenceType;
	}

	inline uint64 VehicleCraneComponent::getCraneEntityId() const
	{
		return mCraneEntityId;
	}

	inline void VehicleCraneComponent::setMinLength(float minLength)
	{
		mMinLength = minLength;
	}

	inline float VehicleCraneComponent::getMinLength() const
	{
		return mMinLength;
	}

	inline void VehicleCraneComponent::setMaxLength(float maxLength)
	{
		mMaxLength = maxLength;
	}

	inline float VehicleCraneComponent::getMaxLength() const
	{
		return mMaxLength;
	}

	inline void VehicleCraneComponent::setMinAngle(float minAngle)
	{
		mMinAngle = minAngle;
	}

	inline float VehicleCraneComponent::getMinAngle() const
	{
		return mMinAngle;
	}

	inline void VehicleCraneComponent::setMaxAngle(float maxAngle)
	{
		mMaxAngle = maxAngle;
	}

	inline float VehicleCraneComponent::getMaxAngle() const
	{
		return mMaxAngle;
	}

	inline void VehicleCraneComponent::setCraneHeight(float craneHeight)
	{
		mCraneHeight = craneHeight;
	}

	inline float VehicleCraneComponent::getCraneHeight() const
	{
		return mCraneHeight;
	}

	inline void VehicleCraneComponent::setArmRotateSpeed(float armRotateSpeed)
	{
		mArmRotateSpeed = armRotateSpeed;
	}

	inline float VehicleCraneComponent::getArmRotateSpeed() const
	{
		return mArmRotateSpeed;
	}

	inline void VehicleCraneComponent::setExtendSpeed(float extendSpeed)
	{
		mExtendSpeed = extendSpeed;
	}

	inline float VehicleCraneComponent::getExtendSpeed() const
	{
		return mExtendSpeed;
	}

	inline void VehicleCraneComponent::setLowToHighSpeed(float lowToHighSpeed)
	{
		mLowToHighSpeed = lowToHighSpeed;
	}

	inline float VehicleCraneComponent::getLowToHighSpeed() const
	{
		return mLowToHighSpeed;
	}

	inline void VehicleCraneComponent::setHookSpeed(float hookSpeed)
	{
		mHookSpeed = hookSpeed;
	}

	inline float VehicleCraneComponent::getHookSpeed() const
	{
		return mHookSpeed;
	}

	inline void VehicleCraneComponent::setCockpitSpeed(float cockpitSpeed)
	{
		mCockpitSpeed = cockpitSpeed;
	}

	inline float VehicleCraneComponent::getCockpitSpeed() const
	{
		return mCockpitSpeed;
	}

	inline void VehicleCraneComponent::setTimeUltimatum(float timeUltimatum)
	{
		mTimeUltimatum = timeUltimatum;
	}

	inline float VehicleCraneComponent::getTimeUltimatum() const
	{
		return mTimeUltimatum;
	}

	inline VehicleCraneComponent::CraneType VehicleCraneComponent::getCraneType() const
	{
		return mCraneType;
	}

	inline void VehicleCraneComponent::setCraneType(VehicleCraneComponent::CraneType craneType)
	{
		mCraneType = craneType;
	}

	inline VehicleCraneComponent::AnimationState VehicleCraneComponent::getLastAnimationState() const
	{
		return mLastAnimationState;
	}

	inline const glm::vec3& VehicleCraneComponent::getTargetPosition() const
	{
		return mTargetPosition;
	}

	inline bool VehicleCraneComponent::isStrapVisible() const
	{
		return mStrapVisible;
	}

	inline float VehicleCraneComponent::getHookMoveDistancePercentage() const
	{
		return mDistancePercentage;
	}

	inline uint64 VehicleCraneComponent::getEntityIdLinkedToHook() const
	{
		return mEntityIdLinkedToHook;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

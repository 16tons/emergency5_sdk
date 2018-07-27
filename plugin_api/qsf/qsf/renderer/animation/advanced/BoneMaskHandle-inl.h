// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BoneMaskHandle::BoneMaskHandle(SkeletonGroup& skeletonGroup, uint16 boneMaskId) :
		mSkeletonGroup(&skeletonGroup),
		mBoneMaskId(boneMaskId)
	{
		// Nothing to do

	}

	inline BoneMaskHandle::BoneMaskHandle() :
		mSkeletonGroup(nullptr),
		mBoneMaskId(getUninitialized<uint16>())
	{
		// Nothing to do
	}

	inline BoneMaskHandle::~BoneMaskHandle()
	{
		// Nothing to do in here
	}

	inline bool BoneMaskHandle::isValid() const
	{
		return (mSkeletonGroup != nullptr && mSkeletonGroup->checkBoneMaskId(mBoneMaskId));
	}

	inline bool BoneMaskHandle::isInvalid() const
	{
		return !isValid();
	}

	inline void BoneMaskHandle::setInvalid()
	{
		mSkeletonGroup = nullptr;
		mBoneMaskId = getUninitialized<uint16>();
	}

	inline uint16 BoneMaskHandle::getId() const
	{
		return mBoneMaskId;
	}


	//[-------------------------------------------------------]
	//[ Private methods for friends                           ]
	//[-------------------------------------------------------]
	inline SkeletonGroup* BoneMaskHandle::getSkeletonGroup()
	{
		return mSkeletonGroup;
	}

	inline const SkeletonGroup* BoneMaskHandle::getSkeletonGroup() const
	{
		return mSkeletonGroup;
	}

	inline float BoneMaskHandle::getBoneWeight(uint16 boneId) const
	{
		return (isValid() && mSkeletonGroup->checkBoneId(boneId)) ? mSkeletonGroup->getBoneMaskWeight(mBoneMaskId, boneId) : 0.0f;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

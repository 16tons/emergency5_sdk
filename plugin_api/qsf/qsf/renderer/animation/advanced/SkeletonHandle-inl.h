// Copyright (C) 2012-2018 Promotion Software GmbH


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
	inline SkeletonHandle::SkeletonHandle(SkeletonGroup& skeletonGroup, uint16 skeletonId) :
		mSkeletonGroup(skeletonGroup),
		mSkeletonId(skeletonId)
	{
		QSF_CHECK(mSkeletonGroup.checkSkeletonId(skeletonId), "Skeleton handle with an invalid skeleton ID", QSF_REACT_THROW);
	}

	inline SkeletonHandle::~SkeletonHandle()
	{
		// Nothing to do in here
	}

	inline uint16 SkeletonHandle::getBoneNumber() const
	{
		return mSkeletonGroup.getBoneNumber();
	}

	inline void SkeletonHandle::copySkeleton(const SkeletonHandle& fromSkeleton)
	{
		mSkeletonGroup.copySkeletonData(fromSkeleton.getId(), mSkeletonId);
	}

	inline void SkeletonHandle::copyBindingPose()
	{
		mSkeletonGroup.copySkeletonDataFromBindingPose(mSkeletonId);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline SkeletonGroup& SkeletonHandle::getSkeletonGroup()
	{
		return mSkeletonGroup;
	}

	inline const SkeletonGroup& SkeletonHandle::getSkeletonGroup() const
	{
		return mSkeletonGroup;
	}

	inline uint16 SkeletonHandle::getId() const
	{
		return mSkeletonId;
	}

	inline Transform& SkeletonHandle::getBoneTransform(uint16 boneId)
	{
		return mSkeletonGroup.getBoneTransform(mSkeletonId, boneId);
	}

	inline const Transform& SkeletonHandle::getBoneDerivedTransform(uint16 boneId)
	{
		return mSkeletonGroup.getBoneDerivedTransform(mSkeletonId, boneId);
	}

	inline void SkeletonHandle::setDerivedTransformDirty(uint16 boneId)
	{
		return mSkeletonGroup.setDerivedTransformDirty(mSkeletonId, boneId);
	}

	inline void SkeletonHandle::setId(uint16 skeletonId)
	{
		// Just remember this checks and use them when necessary
		//QSF_CHECK(mSkeletonGroup.checkSkeletonId(skeletonId), "Skeleton handle with an invalid skeleton Id", QSF_REACT_THROW);

		mSkeletonId = skeletonId;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

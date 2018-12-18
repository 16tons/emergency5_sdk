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
	//[ Iterator subclasses                                   ]
	//[-------------------------------------------------------]
	inline BoneHandle::ChildIterator::ChildIterator(const SkeletonGroup& skeletonGroup, uint16 boneId) :
		mSkeletonGroup(skeletonGroup),
		mBoneId(boneId)
	{
		// Nothing to do
	}

	inline uint16& BoneHandle::ChildIterator::operator*()
	{
		return mBoneId;
	}

	inline BoneHandle::ChildIterator& BoneHandle::ChildIterator::operator++()
	{
		mBoneId = mSkeletonGroup.getBoneSiblingId(mBoneId);
		return *this;
	}

	inline bool BoneHandle::ChildIterator::operator != (const BoneHandle::ChildIterator& iterator) const
	{
		return (mBoneId != iterator.mBoneId);
	}

	inline BoneHandle::ChildIterator BoneHandle::ChildIterator::begin() const
	{
		return ChildIterator(mSkeletonGroup, mBoneId);
	}

	inline BoneHandle::ChildIterator BoneHandle::ChildIterator::end() const
	{
		return ChildIterator(mSkeletonGroup, getUninitialized<uint16>());
	}


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline BoneHandle BoneHandle::getConstBoneHandle(const SkeletonHandle& skeletonHandle, uint16 boneId)
	{
		return BoneHandle(const_cast<SkeletonHandle&>(skeletonHandle), boneId);
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BoneHandle::BoneHandle(SkeletonHandle& skeletonHandle, uint16 boneId) :
		mSkeletonHandle(skeletonHandle),
		mBoneId(boneId)
	{
		QSF_CHECK(mSkeletonHandle.getSkeletonGroup().checkBoneId(boneId), "Bone handle with an invalid bone Id", QSF_REACT_THROW);
	}

	inline BoneHandle::~BoneHandle()
	{
		// Nothing to do in here
	}

	inline const SkeletonHandle& BoneHandle::getSkeleton() const
	{
		return mSkeletonHandle;
	}

	inline uint16 BoneHandle::getId() const
	{
		return mBoneId;
	}

	inline uint16 BoneHandle::getParentId() const
	{
		return mSkeletonHandle.getSkeletonGroup().getBoneParentId(mBoneId);
	}

	inline bool BoneHandle::isRoot() const
	{
		return mSkeletonHandle.getSkeletonGroup().isRootBone(mBoneId);
	}

	inline BoneHandle::ChildIterator BoneHandle::getChildIds() const
	{
		const SkeletonGroup& skeletonGroup = mSkeletonHandle.getSkeletonGroup();
		return ChildIterator(skeletonGroup, skeletonGroup.getBoneChildId(mBoneId));
	}

	inline void BoneHandle::setDerivedTransformDirty() const
	{
		mSkeletonHandle.setDerivedTransformDirty(mBoneId);
	}

	inline const Transform& BoneHandle::getTransform() const
	{
		return mSkeletonHandle.getBoneTransform(mBoneId);
	}

	inline Transform& BoneHandle::getTransformRef()
	{
		setDerivedTransformDirty();

		return mSkeletonHandle.getBoneTransform(mBoneId);
	}

	inline void BoneHandle::setTransform(const Transform& transform)
	{
		Transform& oldTransform = mSkeletonHandle.getBoneTransform(mBoneId);
		if (oldTransform != transform)
		{
			oldTransform = transform;
			setDerivedTransformDirty();
		}
	}

	inline const glm::vec3& BoneHandle::getPosition() const
	{
		return getTransform().getPosition();
	}

	inline void BoneHandle::setPosition(const glm::vec3& position)
	{
		Transform& oldTransform = mSkeletonHandle.getBoneTransform(mBoneId);
		if (oldTransform.getPosition() != position)
		{
			oldTransform.setPosition(position);
			setDerivedTransformDirty();
		}
	}

	inline const glm::quat& BoneHandle::getRotation() const
	{
		return getTransform().getRotation();
	}

	inline void BoneHandle::setRotation(const glm::quat& rotation)
	{
		Transform& oldTransform = mSkeletonHandle.getBoneTransform(mBoneId);
		if (oldTransform.getRotation() != rotation)
		{
			oldTransform.setRotation(rotation);
			setDerivedTransformDirty();
		}
	}

	inline const glm::vec3 BoneHandle::getScale() const
	{
		return getTransform().getScale();
	}

	inline void BoneHandle::setScale(const glm::vec3& scale)
	{
		Transform& oldTransform = mSkeletonHandle.getBoneTransform(mBoneId);
		if (oldTransform.getScale() != scale)
		{
			oldTransform.setScale(scale);
			setDerivedTransformDirty();
		}
	}

	inline const Transform& BoneHandle::getDerivedTransform() const
	{
		return mSkeletonHandle.getBoneDerivedTransform(mBoneId);
	}

	inline glm::vec3 BoneHandle::getDerivedPosition() const
	{
		return getDerivedTransform().getPosition();
	}

	inline void BoneHandle::setDerivedPosition(const glm::vec3& position)
	{
		setPosition(isRoot() ? position : mSkeletonHandle.getBoneDerivedTransform(getParentId()).vec3PositionWorldToLocal(position));
	}

	inline glm::quat BoneHandle::getDerivedRotation() const
	{
		return getDerivedTransform().getRotation();
	}

	inline void BoneHandle::setDerivedRotation(const glm::quat& rotation)
	{
		setRotation(glm::normalize(isRoot() ? rotation : mSkeletonHandle.getBoneDerivedTransform(getParentId()).quatRotationWorldToLocal(rotation)));
	}

	inline const Transform& BoneHandle::getBindingPoseTransform() const
	{
		return mSkeletonHandle.getSkeletonGroup().getBindingPoseBoneTransform(mBoneId);
	}

	inline const glm::vec3& BoneHandle::getBindingPosePosition() const
	{
		return getBindingPoseTransform().getPosition();
	}

	inline const glm::quat& BoneHandle::getBindingPoseRotation() const
	{
		return getBindingPoseTransform().getRotation();
	}

	inline const glm::vec3& BoneHandle::getBindingPoseScale() const
	{
		return getBindingPoseTransform().getScale();
	}

	inline const Transform& BoneHandle::getBindingPoseDerivedTransform() const
	{
		return mSkeletonHandle.getSkeletonGroup().getBindingPoseBoneDerivedTransform(mBoneId);
	}

	inline glm::vec3 BoneHandle::getBindingPoseDerivedPosition() const
	{
		return getBindingPoseDerivedTransform().getPosition();
	}

	inline glm::quat BoneHandle::getBindingPoseDerivedRotation() const
	{
		return getBindingPoseDerivedTransform().getRotation();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

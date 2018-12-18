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
	inline SkeletonGroup::SkeletonGroup(const std::string& ogreSkeletonName) :
		mReferenceCount(1),
		mOgreSkeletonName(ogreSkeletonName),
		mBoneNumber(0),
		mNoMappingNeeded(true)
	{
		// Nothing to do in here
	}

	inline SkeletonGroup::~SkeletonGroup()
	{
		// Nothing to do
	}

	inline uint16 SkeletonGroup::getSkeletonNumber() const
	{
		return static_cast<uint16>(mSkeletonInstances.size() / getBoneNumber());
	}

	inline uint16 SkeletonGroup::getBoneNumber() const
	{
		return mBoneNumber;		// TODO(tl): A little faster as mBoneData.size()
	}

	inline uint16 SkeletonGroup::getBoneMaskNumber() const
	{
		return static_cast<uint16>(mBoneMasks.size() / getBoneNumber());
	}

	inline bool SkeletonGroup::isRootBone(uint16 boneId) const
	{
		return isUninitialized(mBoneData[boneId].parent);
	}

	inline const std::string& SkeletonGroup::getOgreSkeletonName()
	{
		return mOgreSkeletonName;
	}

	inline uint16 SkeletonGroup::getBoneParentId(uint16 boneId) const
	{
		// Just remember this checks and use them when necessary
		//QSF_CHECK(checkBoneId(boneId), "Invalid bone ID found", QSF_REACT_THROW);

		return mBoneData[boneId].parent;
	}

	inline uint16 SkeletonGroup::getBoneChildId(uint16 boneId) const
	{
		// Just remember this checks and use them when necessary
		//QSF_CHECK(checkBoneId(boneId), "Invalid bone ID found", QSF_REACT_THROW);

		return mBoneData[boneId].child;
	}

	inline uint16 SkeletonGroup::getBoneSiblingId(uint16 boneId) const
	{
		// Just remember this checks and use them when necessary
		//QSF_CHECK(checkBoneId(boneId), "Invalid bone ID found", QSF_REACT_THROW);

		return mBoneData[boneId].sibling;
	}

	inline uint16 SkeletonGroup::getOgreHandle(uint16 boneId) const
	{
		// Just remember this checks and use them when necessary
		//QSF_CHECK(checkBoneId(boneId), "Invalid bone ID found", QSF_REACT_THROW);
		if (mNoMappingNeeded)
			return boneId;

		return mBoneData[boneId].ogreHandle;
	}

	inline uint16 SkeletonGroup::getBoneIdFromOgreHandle(uint16 ogreHandle) const
	{
		if (mNoMappingNeeded)
			return ogreHandle;

		// Search in bone mapping
		std::unordered_map<uint16, uint16>::const_iterator findIterator = mBoneIdMapping.find(ogreHandle);
		return (findIterator != mBoneIdMapping.end() ? findIterator->second : getUninitialized<uint16>());
	}

	inline const Transform& SkeletonGroup::getBindingPoseBoneTransform(uint16 boneId) const
	{
		// Just remember this checks and use them when necessary
		//QSF_CHECK(checkBoneId(boneId), "Invalid bone ID found", QSF_REACT_THROW);

		return mBoneData[boneId].bindingPoseTransform;
	}

	inline Transform& SkeletonGroup::getBoneTransform(uint16 skeletonId, uint16 boneId)
	{
		// Just remember this checks and use them when necessary
		//QSF_CHECK(checkSkeletonId(skeletonId), "Invalid skeleton ID found", QSF_REACT_THROW);
		//QSF_CHECK(checkBoneId(boneId), "Invalid bone ID found", QSF_REACT_THROW);

		return getBoneInstance(skeletonId, boneId).transform;
	}

	inline const Transform& SkeletonGroup::getBindingPoseBoneDerivedTransform(uint16 boneId) const
	{
		// Just remember this checks and use them when necessary
		//QSF_CHECK(checkBoneId(boneId), "Invalid bone ID found", QSF_REACT_THROW);

		return mBoneData[boneId].bindingPoseDerivedTransform;
	}

	inline bool SkeletonGroup::checkBoneId(uint16 boneId) const
	{
		return (boneId < getBoneNumber());
	}

	inline SkeletonGroup::BoneTransformData& SkeletonGroup::getBoneInstance(uint16 skeletonId, uint16 boneId)
	{
		return mSkeletonInstances[skeletonId * getBoneNumber() + boneId];
	}

	inline float SkeletonGroup::getBoneMaskWeight(uint16 boneMaskId, uint16 boneId)
	{
		// Just remember this checks and use them when necessary
		// QSF_CHECK(checkBoneMaskId(boneMaskId), "Invalid bone mask ID found", QSF_REACT_THROW);
		// QSF_CHECK(checkBoneId(boneId), "Invalid bone ID found", QSF_REACT_THROW);

		return mBoneMasks[boneMaskId * getBoneNumber() + boneId];
	}

	inline void SkeletonGroup::setBoneMaskWeight(uint16 boneMaskId, uint16 boneId, float weight)
	{
		// Just remember this checks and use them when necessary
		// QSF_CHECK(checkBoneMaskId(boneMaskId), "Invalid bone mask ID found", QSF_REACT_THROW);
		// QSF_CHECK(checkBoneId(boneId), "Invalid bone ID found", QSF_REACT_THROW);

		mBoneMasks[boneMaskId * getBoneNumber() + boneId] = weight;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

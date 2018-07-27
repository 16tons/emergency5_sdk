// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/SkeletonGroup.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Skeleton handle class, holds the transformation state of a skeleton
	*/
	class QSF_API_EXPORT SkeletonHandle
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class BoneHandle;	// We have prepared private methods for qsf::BoneHandle and qsf::AnimationNode
		friend class AnimationNode;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] skeletonGroup
		*    Skeleton group who manages this skeleton instance
		*  @param[in] skeletonId
		*    ID of the skeleton instance we want to handle
		*/
		inline SkeletonHandle(SkeletonGroup& skeletonGroup, uint16 skeletonId);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SkeletonHandle();

		/**
		*  @brief
		*    Give back the number of bones
		*/
		inline uint16 getBoneNumber() const;

		/**
		*  @brief
		*    Copy the skeleton data from another skeleton
		*
		*  @param[in] fromSkeleton
		*    A reference to another skeleton where we want to take the transform data
		*/
		inline void copySkeleton(const SkeletonHandle& fromSkeleton);

		/**
		*  @brief
		*    Copy the skeleton data from binding pose
		*/
		inline void copyBindingPose();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Give back a reference of the skeleton group
		*/
		inline SkeletonGroup& getSkeletonGroup();
		inline const SkeletonGroup& getSkeletonGroup() const;

		/**
		*  @brief
		*    Give back the skeleton instance ID
		*
		*  @note
		*    - Binding pose skeletons have an undefined ID
		*/
		inline uint16 getId() const;

		/**
		*  @brief
		*    Give back a reference to the transform data of a bone
		*
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*/
		inline Transform& getBoneTransform(uint16 boneId);

		/**
		*  @brief
		*    Give back the derived transform data of a bone
		*
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*/
		inline const Transform& getBoneDerivedTransform(uint16 boneId);

		/**
		*  @brief
		*    Setting the derived transform data to dirty
		*/
		inline void setDerivedTransformDirty(uint16 boneId);

		/**
		*  @brief
		*    Change this handle to a different skeleton instance
		*
		*  @param[in] skeletonId
		*    ID of the skeleton instance we want to handle
		*/
		inline void setId(uint16 skeletonId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SkeletonGroup&	mSkeletonGroup;
		uint16			mSkeletonId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/SkeletonHandle-inl.h"

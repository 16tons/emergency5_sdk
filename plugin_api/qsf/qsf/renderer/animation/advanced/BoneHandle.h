// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/SkeletonHandle.h"


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
	*    Bone handle class, use it to access bones from skeleton group
	*/
	class QSF_API_EXPORT BoneHandle
	{


		/**
		*  @brief
		*    Gives back an iterator over all childs of this bone
		*
		*  @note
		*    - Standard C++11 iterator, use it with: for (const uint16& childBone : bone.getChildIds()) {...}
		*/
		class ChildIterator
		{
		public:
			inline ChildIterator(const SkeletonGroup& mSkeletonGroup, uint16 boneId);
			inline uint16& operator*();
			inline ChildIterator& operator++();
			inline bool operator!=(const ChildIterator& iterator) const;

			inline ChildIterator begin() const;
			inline ChildIterator end() const;

		private:
			const SkeletonGroup& mSkeletonGroup;
			uint16 mBoneId;
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Const fake constructor
		*
		*  @param[in] skeletonHandle
		*    Const reference to the skeleton handle of this bone
		*  @param[in] boneId
		*    ID of this bone
		*
		*  @note
		*    - Use this function to get a const BoneHandle from a const SkeletonHandle
		*/
		inline static BoneHandle getConstBoneHandle(const SkeletonHandle& skeletonHandle, uint16 boneId);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] skeletonHandle
		*    Reference to the skeleton handle of this bone
		*  @param[in] boneId
		*    ID of this bone
		*/
		inline BoneHandle(SkeletonHandle& skeletonHandle, uint16 boneId);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BoneHandle();

		/**
		*  @brief
		*    Give back skeleton handle reference
		*/
		inline const SkeletonHandle& getSkeleton() const;

		/**
		*  @brief
		*    Give back the bone ID
		*/
		inline uint16 getId() const;

		/**
		*  @brief
		*    Give back the parent bone ID (unidentified with no parent)
		*/
		inline uint16 getParentId() const;

		/**
		*  @brief
		*    Check if this is the root bone
		*/
		inline bool isRoot() const;

		/**
		*  @brief
		*    Give back an iterator to all child bones of this parent bone
		*/
		inline ChildIterator getChildIds() const;

		/**
		*  @brief
		*    Declare our derived transform as dirty
		*/
		inline void setDerivedTransformDirty() const;

		//[-------------------------------------------------------]
		//[ Transform, position, rotation and scaling             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Transform data of this bone (in parent bone space)
		*/
		inline const Transform& getTransform() const;
		inline void setTransform(const Transform& transform);

		/**
		*  @brief
		*    Get reference on transform for full access
		*/
		inline Transform& getTransformRef();

		/**
		*  @brief
		*    Position of this bone (in parent bone space)
		*
		*  @note
		*    - use getTransform() or getTransformRef() if you want to set or get position, rotation and scale for better performance
		*/
		inline const glm::vec3& getPosition() const;
		inline void setPosition(const glm::vec3& position);

		/**
		*  @brief
		*    Rotation/orientation of this bone (in parent bone space)
		*
		*  @note
		*    - We don't normalize the rotation, keep in mind to normalize it before
		*    - Use getTransform() or getTransformRef() if you want to set or get position, rotation and scale for better performance
		*/
		inline const glm::quat& getRotation() const;
		inline void setRotation(const glm::quat& rotation);

		/**
		*  @brief
		*    Scaling of this bone (in parent bone space)
		*
		*  @note
		*    - Use getTransform() or getTransformRef() if you want to set or get position, rotation and scale for better performance
		*/
		inline const glm::vec3 getScale() const;
		inline void setScale(const glm::vec3& scale);

		//[-------------------------------------------------------]
		//[ Derived transform, position, rotation and scaling     ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Derived transformation of this bone (in object space)
		*/
		inline const Transform& getDerivedTransform() const;

		/**
		*  @brief
		*    Derived position of this bone (in object space)
		*/
		inline glm::vec3 getDerivedPosition() const;
		inline void setDerivedPosition(const glm::vec3& position);

		/**
		*  @brief
		*    Derived rotation/orientation of this bone (in object space)
		*/
		inline glm::quat getDerivedRotation() const;
		inline void setDerivedRotation(const glm::quat& rotation);

		//[-------------------------------------------------------]
		//[ Binding pose transform, position, rotation and scaling]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Binding pose transform data of this bone (in parent bone space)
		*/
		inline const Transform& getBindingPoseTransform() const;

		/**
		*  @brief
		*    Binding pose position of this bone (in parent bone space)
		*/
		inline const glm::vec3& getBindingPosePosition() const;

		/**
		*  @brief
		*    Binding pose rotation/orientation of this bone (in parent bone space)
		*/
		inline const glm::quat& getBindingPoseRotation() const;

		/**
		*  @brief
		*    Binding pose scaling of this bone (in parent bone space)
		*/
		inline const glm::vec3& getBindingPoseScale() const;

		/**
		*  @brief
		*    Derived transformation of this bones binding pose (in object space)
		*/
		inline const Transform& getBindingPoseDerivedTransform() const;

		/**
		*  @brief
		*    Derived rotation/orientation of this bones binding pose (in object space)
		*
		*  @note
		*    - Use this functions with care, derived transformations are not stored
		*/
		inline glm::vec3 getBindingPoseDerivedPosition() const;

		/**
		*  @brief
		*    Derived rotation/orientation of this bones binding pose (in object space)
		*
		*  @note
		*    Use this functions with care, derived transformations are not stored
		*/
		inline glm::quat getBindingPoseDerivedRotation() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SkeletonHandle&		mSkeletonHandle;
		uint16				mBoneId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/BoneHandle-inl.h"

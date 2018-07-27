// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Transform.h"
#include "qsf/base/GetUninitialized.h"

#include <unordered_map>
#include <vector>


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
	*    Skeleton group for advanced animation components
	*
	*  @note
	*    - A skeleton group has a basic structure which is shared between many skeletons of entities (for example a person skeleton)
	*    - Don't use this class outside of the core classes of the advanced animation system
	*/
	class SkeletonGroup
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class SkeletonManager;	// Skeleton manager manages this class and uses the reference count


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct BoneBaseData			// Basic data for a bone, we don't need to store it in every skeleton instance
		{
			uint16			ogreHandle;
			uint16			parent;
			uint16			child;			// We use the child/sibling ID for faster traversion of childs
			uint16			sibling;
			Transform		bindingPoseTransform;
			Transform		bindingPoseDerivedTransform;

			inline BoneBaseData(uint16 ogreHandle, uint16 parent, uint16 child, uint16 sibling, const Transform& bindingPoseTransform, const Transform& bindingPoseDerivedTransform) :
				ogreHandle(ogreHandle), parent(parent), child(child), sibling(sibling), bindingPoseTransform(bindingPoseTransform), bindingPoseDerivedTransform(bindingPoseDerivedTransform){};
		};

		struct BoneTransformData		// Transform data for a bone, used for every skeleton instance
		{
			Transform	transform;

			// Derived transformation subdata
			bool		isDerivedUpToDate;
			Transform	derivedTransform;

			inline BoneTransformData() :
				isDerivedUpToDate(false)
			{};

			inline BoneTransformData(const Transform& transform) :
				transform(transform),
				isDerivedUpToDate(false)
			{};
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline explicit SkeletonGroup(const std::string& ogreSkeletonName);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SkeletonGroup();

		/**
		*  @brief
		*    Give back the number of skeleton instances
		*/
		inline uint16 getSkeletonNumber() const;

		/**
		*  @brief
		*    Give back the number of bones
		*/
		inline uint16 getBoneNumber() const;

		/**
		*  @brief
		*    Give back the number of bone masks
		*/
		inline uint16 getBoneMaskNumber() const;

		/**
		*  @brief
		*    Give back if the bone is a root bone
		*
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*/
		inline bool isRootBone(uint16 boneId) const;

		/**
		*  @brief
		*    Give back the ID of the bone parent, undefined if we have a root bone
		*
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*/
		inline uint16 getBoneParentId(uint16 boneId) const;

		/**
		*  @brief
		*    Give back the ID of one of the bone child, undefined if we don't have childs
		*
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*/
		inline uint16 getBoneChildId(uint16 boneId) const;

		/**
		*  @brief
		*    Give back the ID of the bone sibling, undefined if we are the last sibling
		*
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*/
		inline uint16 getBoneSiblingId(uint16 boneId) const;

		/**
		*  @brief
		*    Give back the ogre handle
		*
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*/
		inline uint16 getOgreHandle(uint16 boneId) const;

		/**
		*  @brief
		*    Give back the bone id from an ogreHandle, uninitialized if nothing is found
		*
		*  @param[in] ogreHandle
		*    OGRE handle of a bone
		*/
		inline uint16 getBoneIdFromOgreHandle(uint16 ogreHandle) const;

		/**
		*  @brief
		*    Give back the transform data for a bone of the binding pose
		*
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*/
		inline const Transform& getBindingPoseBoneTransform(uint16 boneId) const;

		/**
		*  @brief
		*    Give back a reference to the transform data for a bone of a skeleton instance
		*
		*  @param[in] skeletonId
		*    ID of the skeleton instance
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*/
		inline Transform& getBoneTransform(uint16 skeletonId, uint16 boneId);

		/**
		*  @brief
		*    Give back the derived transform data for a bone of a skeleton instance
		*
		*  @param[in] skeletonId
		*    ID of the skeleton instance
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*
		*  @note
		*    - Check if derived data is updated, before you use this data
		*/
		const Transform& getBoneDerivedTransform(uint16 skeletonId, uint16 boneId);

		/**
		*  @brief
		*    Give back the derived transform data for a bone of the binding pose
		*
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*/
		inline const Transform& getBindingPoseBoneDerivedTransform(uint16 boneId) const;

		/**
		*  @brief
		*    Inform a bone that his derived transformation is dirty and not updated
		*
		*  @param[in] skeletonId
		*    ID of the skeleton instance
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*/
		void setDerivedTransformDirty(uint16 skeletonId, uint16 boneId);

		/**
		*  @brief
		*    Set the derived transform for a skeleton dirty (A lot faster then dirtying individual bones, when every bone is dirtied anyway)
		*
		*  @param[in] skeletonId
		*    ID of the skeleton instance
		*/
		void setDerivedTransformForSkeletonDirty(uint16 skeletonId);

		/**
		*  @brief
		*    Check if bone ID is valid
		*
		*  @param[in] boneId
		*    ID of a bone of the skeleton
		*/
		inline bool checkBoneId(uint16 boneId) const;

		/**
		*  @brief
		*    Check if skeleton ID is valid, can be uninitialized as an ID for the binding pose
		*
		*  @param[in] boneId
		*    ID of a skeleton from this skeleton group
		*/
		bool checkSkeletonId(uint16 skeletonId) const;

		/**
		*  @brief
		*    Check if bone mask ID is valid
		*
		*  @param[in] boneId
		*    ID of a bone mask of the skeleton group
		*/
		bool checkBoneMaskId(uint16 skeletonId) const;

		/**
		*  @brief
		*    Copy the skeleton transform data from one skeleton instance to another
		*
		*  @param[in] fromSkeletonId
		*    ID of the skeleton instance we want to read the data from
		*  @param[in] toSkeletonId
		*    ID of the skeleton instance we want to copy the data to
		*/
		void copySkeletonData(uint16 fromSkeletonId, uint16 toSkeletonId);

		/**
		*  @brief
		*    Copy the skeleton transform data from binding pose to a skeleton instance
		*
		*  @param[in] skeletonId
		*    ID of the skeleton instance we want to copy the data to
		*/
		void copySkeletonDataFromBindingPose(uint16 skeletonId);

		/**
		*  @brief
		*    Add a new root bone to all skeletons
		*
		*  @param[in] ogreHandle
		*    Handle of the bone in OGRE (don't confuse with bone ID)
		*  @param[in] bindingPoseTransform
		*    Binding pose transformation of this bone
		*
		*  @return
		*    ID of the new bone(number of bones - 1)
		*/
		uint16 addRootBone(uint16 ogreHandle, const Transform& bindingPoseTransform);

		/**
		*  @brief
		*    Add a new bone to all skeletons
		*
		*  @param[in] ogreHandle
		*    Handle of the bone in ogre (don't confuse with bone ID)
		*  @param[in] parentId
		*    ID of the parent bone who needs to be already added, if unsigned we will add a root bone
		*  @param[in] bindingPoseTransform
		*    Binding pose transformation of this bone
		*  @param[in] bindingPoseDerivedTransform
		*    Binding pose derived transformation of this bone (in object space)
		*
		*  @return
		*    ID of the new bone (number of bones -1)
		*
		*  @note
		*    This function can be slow if we add bones after we created skeleton instances and bone masks
		*/
		uint16 addBone(uint16 ogreHandle, uint16 parentId, const Transform& bindingPoseTransform, const Transform& bindingPoseDerivedTransform);

		/**
		*  @brief
		*    Remove all bones without changing skeleton instances
		*/
		void removeAllBones();

		/**
		*  @brief
		*    Register a skeleton instance, if we have a unregistered skeleton we will use them or we add a new instance
		*/
		uint16 registerSkeletonId();

		/**
		*  @brief
		*    Unregister a skeleton instance without freeing memory
		*
		*  @param[in] skeletonId
		*    The skeleton ID we want to unregister, cannot be the binding pose with an uninitialized value
		*/
		void unregisterSkeletonId(uint16 skeletonId);

		/**
		*  @brief
		*    Remove all skeletons
		*/
		void removeAllSkeletons();

		/**
		*  @brief
		*    Give back the OGRE skeleton definition name
		*/
		inline const std::string& getOgreSkeletonName();

		/**
		*  @brief
		*    Get a bone instance from our skeleton instance vector
		*
		*  @param[in] skeletonId
		*     The skeleton ID where we want the bone from
		*  @param[in] boneId
		*    ID of the bone
		*
		*  @return
		*    Bone transformation data
		*/
		inline BoneTransformData& getBoneInstance(uint16 skeletonId, uint16 boneId);

		/**
		*  @brief
		*    Register a new bone mask
		*
		*  @return
		*    New bone mask ID
		*/
		uint16 registerBoneMaskId();

		/**
		*  @brief
		*    Unregister a bone mask
		*
		*  @param[in] boneMaskId
		*    ID of the bone mask we want to remove
		*/
		void unregisterBoneMaskId(uint16 boneMaskId);

		/**
		*  @brief
		*    Get a bone mask weight
		*
		*  @param[in] boneMaskId
		*    ID of the bone mask in skeleton group
		*  @param[in] boneId
		*    ID of the bone of the skeleton
		*
		*  @return
		*    Weight between 0.0 and 1.0 or more
		*/
		inline float getBoneMaskWeight(uint16 boneMaskId, uint16 boneId);

		/**
		*  @brief
		*    Set a bone mask weight
		*
		*  @param[in] boneMaskId
		*    ID of the bone mask in skeleton group
		*  @param[in] boneId
		*    ID of the bone of the skeleton
		*  @param[in] weight
		*    Weight between 0.0 and 1.0 or more
		*/
		inline void setBoneMaskWeight(uint16 boneMaskId, uint16 boneId, float weight);

		/**
		*  @brief
		*    Remove all bone masks
		*/
		void removeAllBoneMasks();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Add a new skeleton instance
		*
		*  @return
		*    ID of the new skeleton instance (number of skeletons -1)
		*/
		uint16 addSkeletonInstance();

		/**
		*  @brief
		*    Add a new bone mask and fill it with 0
		*
		*  @return
		*    ID of the new bone mask
		*/
		uint16 addBoneMask();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64								mReferenceCount;				///< This does count the number of components who use this group
		std::string							mOgreSkeletonName;				///< OGRE skeleton definition name
		uint16								mBoneNumber;					///< Number of bones, as it is constant after initializing all bones, it should be faster as std::vector::size()
		std::vector<BoneBaseData>			mBoneData;						///< Basic data and declaration of bones, size is taken as number of bones
		bool								mNoMappingNeeded;				///< If OgreBoneHandle.getId() == BoneId we don't need a mapping
		std::unordered_map<uint16, uint16>	mBoneIdMapping;					///< Bone ID mapping for faster lookup with OGRE handles

		// TODO(tl): Is this skeleton instance data cache friendly enough ?
		std::vector<BoneTransformData>		mSkeletonInstances;				///< An array of instances of skeleton bones
		std::vector<uint16>					mUnregisteredSkeletonInstances;	///< All skeleton instance which are free to get registered

		std::vector<float>					mBoneMasks;						///< All weights of bone masks from this skeleton group
		std::vector<uint16>					mUnregisteredBoneMasks;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/SkeletonGroup-inl.h"

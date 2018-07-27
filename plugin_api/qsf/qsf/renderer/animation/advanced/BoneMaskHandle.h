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
	*    Bone mask handle class, use it to access a bone mask of a skeleton
	*/
	class QSF_API_EXPORT BoneMaskHandle
	{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class AdvancedAnimationComponent;


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
		*/
		inline BoneMaskHandle(SkeletonGroup& skeletonGroup, uint16 boneMaskId);

		/**
		*  @brief
		*    Default constructor for invalid handles
		*/
		inline BoneMaskHandle();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BoneMaskHandle();

		/**
		*  @brief
		*    Check if this handle is valid
		*/
		inline bool isValid() const;

		/**
		*  @brief
		*    Check if this handle is invalid
		*/
		inline bool isInvalid() const;

		/**
		*  @brief
		*    Set this handle to invalid
		*/
		inline void setInvalid();

		/**
		*  @brief
		*    Give back the bone ID
		*
		*  @note
		*    - Undefined for invalid bone masks
		*/
		inline uint16 getId() const;

		/**
		*  @brief
		*    Remove and unregister this bone mask
		*
		*  @note
		*    - This bone mask will be invalid at the end
		*/
		void remove();

		/**
		*  @brief
		*    Give back the weight of a bone
		*
		*  @return
		*    Float between 0.0 and 1.0 or more
		*
		*  @note
		*    - Invalid boneIds or an already unregistered BoneMask will result in a weight of 0.0f
		*/
		inline float getBoneWeight(uint16 boneId) const;

		/**
		*  @brief
		*    Set the weight of a bone
		*
		*  @param[in] boneId
		*    ID of the bone
		*  @param[in] weight
		*    Weight between 0.0 and 1.0 or more
		*/
		void setBoneWeight(uint16 boneId, float weight = 1.0f);

		/**
		*  @brief
		*    Add the weight of a bone and all its childs and their childs
		*
		*  @param[in] boneId
		*    ID of the bone
		*  @param[in] weight
		*    weight between 0.0 and 1.0 or more
		*/
		void addInheritedBoneWeight(uint16 boneId, float weight = 1.0f);

		/**
		*  @brief
		*    Set the weight of all bones
		*/
		void setAllWeights(float weight);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Give back a pointer of the skeleton group
		*/
		inline SkeletonGroup* getSkeletonGroup();
		inline const SkeletonGroup* getSkeletonGroup() const;

		/**
		*  @brief
		*    No need to check the handle and bone anymore
		*/
		void addInheritedBoneWeightValid(uint16 boneId, float weight);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		SkeletonGroup*	mSkeletonGroup;
		uint16			mBoneMaskId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/BoneMaskHandle-inl.h"

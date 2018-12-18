// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/AnimationNode.h"


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
	*    Two bone inverse kinematik animation node, move a bone to a target position by rotating the parents as close as possible
	*/
	class QSF_API_EXPORT TwoBoneInverseKinematicNode : public AnimationNode
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] advancedAnimationComponent
		*    A reference to an advanced animation component where this animation node lies
		*/
		explicit TwoBoneInverseKinematicNode(AdvancedAnimationComponent& advancedAnimationComponent);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TwoBoneInverseKinematicNode();

		/**
		*  @brief
		*    Return the bone ID of the bone we want to move to target position
		*/
		inline uint16 getBoneId() const;

		/**
		*  @brief
		*    Set the bone ID of the bone we want to move to target position
		*/
		inline void setBoneId(uint16 boneId);

		/**
		*  @brief
		*    Return the target position we want to move our bones to
		*/
		inline const glm::vec3& getTargetPosition() const;

		/**
		*  @brief
		*    Set the target position we want to move our bones to
		*/
		inline void setTargetPosition(const glm::vec3& targetPosition);

		/**
		*  @brief
		*    Get the minimum and maximum constrain of the alpha angle at the middle bone
		*/
		inline const glm::vec2& getAlphaAngleConstrainMinMax() const;

		/**
		*  @brief
		*    Set the minimum and maximum constrain of the alpha angle at the middle bone
		*/
		inline void setAlphaAngleConstrainMinMax(const glm::vec2& alphaAngleConstrainMinMax);

		/**
		*  @brief
		*    Return weight of blending the kinematic result to our output skeleton
		*/
		inline float getWeight() const;

		/**
		*  @brief
		*    Set the weight to blend kinematic result to our output skeleton
		*
		*  @param[in] weight
		*    Blending weight
		*/
		inline void setWeight(float weight);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::AnimationNode methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual void calculate() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input parameter
		uint16		mBoneId;
		glm::vec3	mTargetPosition;
		glm::vec2	mAlphaAngleConstrainMinMax;
		float		mWeight;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/node/TwoBoneInverseKinematicNode-inl.h"

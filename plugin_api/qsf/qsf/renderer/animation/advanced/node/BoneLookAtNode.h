// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/AnimationNode.h"
#include "qsf/math/DampedSpring.h"

#include <glm/gtc/quaternion.hpp>


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
	*    Bone lookat animation node, rotate a bone to look at a target
	*/
	class QSF_API_EXPORT BoneLookAtNode : public AnimationNode
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
		explicit BoneLookAtNode(AdvancedAnimationComponent& advancedAnimationComponent);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~BoneLookAtNode();

		/**
		*  @brief
		*    Return the bone ID of the bone we want to rotate
		*/
		inline uint16 getBoneId() const;

		/**
		*  @brief
		*    Set the bone ID of the bone we want to rotate
		*/
		void setBoneId(uint16 boneId);

		/**
		*  @brief
		*    Return the target position in object space we want to rotate our bone to
		*/
		inline const glm::vec3& getTargetPosition() const;

		/**
		*  @brief
		*    Set the target position in object space we want to rotate our bone to
		*/
		void setTargetPosition(const glm::vec3& targetPosition);

		/**
		*  @brief
		*    Return the rotation constrain in euler angles (yaw, pitch, roll)
		*/
		inline const glm::vec3& getRotationConstrain() const;

		/**
		*  @brief
		*    Set the rotation constrain in euler angles (yaw, pitch, roll)
		*
		*  @note
		*    0 for full constrains to rotations, pi/2 for rotations from -pi/2 to pi/2, pi no constrains to rotations
		*/
		void setRotationConstrain(const glm::vec3& rotationConstrain);

		/**
		*  @brief
		*    Return the time step in sec. for springs
		*/
		inline float getTimeStep() const;

		/**
		*  @brief
		*    Set the time step in sec. for springs
		*/
		inline void setTimeStep(float timeStep);

		/**
		*  @brief
		*    Return the damped spring for our yaw and pitch angle
		*/
		inline const DampedSpring& getDampedYawAngleSpring() const;
		inline const DampedSpring& getDampedPitchAngleSpring() const;

		/**
		*  @brief
		*    Set the damped spring for our yaw and pitch angle
		*
		*  @note
		*    The value of the spring(yaw/pitch) is not set, we will retain the value, we only set spring properties
		*/
		inline void setDampedYawAngleSpring(const DampedSpring& dampedSpring);
		inline void setDampedPitchAngleSpring(const DampedSpring& dampedSpring);

		/**
		*  @brief
		*    Return if we are moving to our default rotation
		*/
		inline bool isMovingToDefault() const;

		/**
		*  @brief
		*    Set if we want to move to our default position
		*/
		void setMovingToDefault(bool movingToDefault);

		/**
		*  @brief
		*    Return if we are at our default rotation
		*/
		inline bool isAtDefaultPosition() const;

		/**
		*  @brief
		*    Return the rotation constrains in euler angles (yaw, pitch, roll), if broken we move to default rotation
		*/
		inline const glm::vec3& getMovingToDefaultRotationConstrain() const;

		/**
		*  @brief
		*    Set the rotation constrains in euler angles (yaw, pitch, roll), if broken we move to default rotation
		*
		*  @note
		*    - 0 for full constrains to rotations, pi/2 for rotations from -pi/2 to pi/2, pi no constrains to rotations
		*/
		void setMovingToDefaultRotationConstrain(const glm::vec3& rotationConstrain);

		/**
		*  @brief
		*    Return the animation influence between 0.0 and 1.0
		*/
		inline float getAnimationInfluence() const;

		/**
		*  @brief
		*    Set the animation influence
		*
		*  @param[in] animationInfluence
		*    Should we use the binding pose (0.0f) or the input animation (1.0f) as our reference when we rotate our head
		*/
		inline void setAnimationInfluence(float animationInfluence);

		/**
		*  @brief
		*    Return the animation influence blend speed in influence change per sec.
		*/
		inline float getAnimationInfluenceBlendSpeed() const;

		/**
		*  @brief
		*    Set the animation influence blend speed
		*
		*  @param[in] animationInfluenceBlendSpeed
		*    In influence change per sec. (1.0f means we will blend to a influence of 0.0f in 1 second)
		*/
		inline void setAnimationInfluenceBlendSpeed(float animationInfluenceBlendSpeed);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::AnimationNode methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual void calculate() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Enforce "mRotationConstrain" over a given rotation
		*/
		void constrain(glm::vec3& eulerRotation);

		/**
		*  @brief
		*    Enforce "mMovingToDefaultRotationConstrain" over a given rotation, if constrains are broken move to default
		*/
		void movingToDefaultConstrain(glm::vec3& eulerRotation);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input parameter
		uint16				mBoneId;
		glm::vec3			mTargetPosition;
		glm::vec3			mRotationConstrain;

		float				mTimeStep;
		DampedSpring		mDampedYawAngleSpring;
		DampedSpring		mDampedPitchAngleSpring;

		bool				mMovingToDefault;
		glm::vec3			mMovingToDefaultRotationConstrain;

		float				mAnimationInfluence;
		float				mAnimationInfluenceBlendSpeed;

		// Internal
		bool				mFinishedAtDefault;
		float				mSmoothedAnimationInfluence;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/node/BoneLookAtNode-inl.h"

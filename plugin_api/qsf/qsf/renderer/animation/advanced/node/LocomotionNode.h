// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/AnimationNode.h"
#include "qsf/renderer/animation/advanced/node/TwoBoneInverseKinematicNode.h"
#include "qsf/renderer/animation/advanced/node/BoneControlNode.h"


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
	*    Locomotion animation node, node which should control the walking of a person
	*
	*  @todo
	*    - TODO(tl) Experimental node
	*/
	class QSF_API_EXPORT LocomotionNode : public AnimationNode
	{


	//[-------------------------------------------------------]
	//[ Public definition                                     ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Walking cycle data of an animation
		*/
		struct WalkingCycle
		{
			float numberOfCycles;	///< Number of walking cycles in an animation
			float cycleOffset;		///< Offset from the start of the animation, until the right foot touches the ground
			float footOffset;		///< Offset between feet, how long takes the left foot to touches the ground after the right

			WalkingCycle() : numberOfCycles(1.0f), cycleOffset(0.0f), footOffset(0.5f) {};
			WalkingCycle(float numberOfCycles, float cycleOffset, float footOffset) : numberOfCycles(numberOfCycles), cycleOffset(cycleOffset), footOffset(footOffset) {};
		};


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
		explicit LocomotionNode(AdvancedAnimationComponent& advancedAnimationComponent);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LocomotionNode();

		/**
		*  @brief
		*    Return the bone ID of the left foot bone
		*/
		inline uint16 getLeftFootBoneId() const;

		/**
		*  @brief
		*    Set the bone ID of the left foot bone
		*/
		void setLeftFootBoneId(uint16 leftFootBoneId);

		/**
		*  @brief
		*    Return the bone ID of the left foot bone
		*/
		inline uint16 getRightFootBoneId() const;

		/**
		*  @brief
		*    Set the bone ID of the left foot bone
		*/
		void setRightFootBoneId(uint16 rightFootBoneId);

		/**
		*  @brief
		*    Return animation progress
		*
		*  @return
		*    Value between 0.0 and 1.0
		*/
		inline float getProgress() const;

		/**
		*  @brief
		*    Set animation progress (will be wrapped into [0.0, 1.0])
		*/
		void setProgress(float Progress);

		/**
		*  @brief
		*    Return the walking cycle data we set for our animation
		*/
		inline const WalkingCycle& getWalkingCycle() const;

		/**
		*  @brief
		*    Set the walking cycle data we set for our animation
		*
		*  @param[in] walkingCycle
		*    Walking cycle data of current animation(number of cycles, offset until right foot touches the ground and offset between feet)
		*/
		inline void setWalkingCycle(const WalkingCycle& walkingCycle);

		/**
		*  @brief
		*    Return the minimum and maximum constrain on the knee angle
		*/
		inline const glm::vec2& getKneeAngleConstrainMinMax() const;

		/**
		*  @brief
		*    Set the minimum and maximum constrain on the knee angle
		*/
		void setKneeAngleConstrainMinMax(const glm::vec2& kneeAngleConstrainMinMax);

		/**
		*  @brief
		*    Return the minimum and maximum constrain on height
		*/
		inline const glm::vec2& getHeightConstrainMinMax() const;

		/**
		*  @brief
		*    Set the minimum and maximum constrain on height
		*/
		inline void setHeightConstrainMinMax(const glm::vec2& heightConstrainMinMax);


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
		*    Walking cycle for the left foot between 0 and 1
		*
		*  @remarks
		*    Walking cycle:
		*    0.0 - 0.5 foot on the ground
		*    0.5 - 1.0 foot in the air
		*/
		float getLeftFootWalkingCycle() const;

		/**
		*  @brief
		*    Walking cycle for the right foot between 0 and 1
		*/
		float getRightFootWalkingCycle() const;

		/**
		*  @brief
		*    Foot on ground cycle between 0 and 1
		*/
		float getFootOnGroundWave(float walkingCycle) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input parameter
		uint16			mLeftFootBoneId;
		uint16			mRightFootBoneId;
		glm::vec2		mKneeAngleConstrainMinMax;
		glm::vec2		mHeightConstrainMinMax;
		WalkingCycle	mWalkingCycle;
		float			mProgress;

		// Internal nodes
		TwoBoneInverseKinematicNode	mLeftFootBoneInverseKinematik;
		BoneControlNode				mLeftFootBoneControl;
		TwoBoneInverseKinematicNode	mRightFootBoneInverseKinematik;
		BoneControlNode				mRightFootBoneControl;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/node/LocomotionNode-inl.h"

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"

#include <qsf/component/Component.h>
#include <qsf/job/JobProxy.h>

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class TransformComponent;
	class MeshAnimationChannel;
	class MeshAnimationComponent;
	namespace game
	{
		class BoneToBoneLinkComponent;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 DLK ladder component
	*
	*  @remarks
	*    EMERGENCY 5 DLK units can move the ladder and align it to a target.
	*/
	class EM5_API_EXPORT DlkLadderComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::DlkLadderComponent" unique component ID
		static const float YAW_EPSILON;			///< Epsilon to check if yaw orientation can be assumed equal
		static const float ANIMATION_EPSILON;	///< Epsilon to check if animation offset can be assumed equal
		static const glm::vec3 ARM_ORIENTATION;	///< Orientation of ladder arm

		enum AnimationState
		{
			STATE_NONE,
			STATE_ALIGN_LADDER,
			STATE_DEPLOY_LADDER,
			STATE_FOLD_LADDER,
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		explicit DlkLadderComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DlkLadderComponent();

		/**
		*  @brief
		*    Move the ladder arm of the DLK to the given position; used to rescue people
		*
		*  @param[in] position
		*    The position the arm should move to
		*/
		void alignLadder(const glm::vec3& position);

		/**
		*  @brief
		*    Move the ladder arm of the DLK up in the air, in direction of position; used extinguish fire
		*
		*  @param[in] position
		*    The position as a reference point
		*/
		void deployLadder(const glm::vec3& position);

		/**
		*  @brief
		*    Move the ladder arm of the DLK back to starting position
		*/
		void foldLadder();

		/**
		*  @brief
		*    Return if the ladder animation is running or not
		*/
		bool isAnimationPlaying() const;

		// Getter and setter
		inline float getVerticalRescueOffset() const;
		inline void setVerticalRescueOffset(float verticalRescueOffset);

		inline float getVerticalDeployOffset() const;
		inline void setVerticalDeployOffset(float verticalDeployOffset);

		inline float getLadderMaxRotateSpeed() const;
		inline void setLadderMaxRotateSpeed(float ladderMaxRotateSpeed);

		inline float getLadderMinRotateSpeed() const;
		inline void setLadderMinRotateSpeed(float ladderMinRotateSpeed);

		inline float getLadderUpSpeed() const;
		inline void setLadderUpSpeed(float ladderUpSpeed);

		inline float getLadderExtendSpeed() const;
		inline void setLadderExtendSpeed(float ladderExtendSpeed);

		inline float getMaxAngle() const;
		inline void setMaxAngle(float maxAngle);

		inline float getMinLength() const;
		inline void setMinLength(float minLength);

		inline float getMaxLength() const;
		inline void setMaxLength(float maxLength);

		inline float getSynchronLength() const;
		inline void setSynchronLength(float synchronLength);

		inline float getTimeUltimatum() const;
		inline void setTimeUltimatum(float timeUltimatum);

		inline const qsf::AssetProxy& getAnimationLadderBaseUp() const;
		inline void setAnimationLadderBaseUp(const qsf::AssetProxy& skeleton);

		inline const qsf::AssetProxy& getAnimationLadderExtend() const;
		inline void setAnimationLadderExtend(const qsf::AssetProxy& skeleton);

		inline const AnimationState getLastAnimationState() const;
		inline const glm::vec3& getTargetPosition() const;

		qsf::Entity* getStageEntity() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool getComponentPointers();
		void calculateAngleAndOffsets();
		bool isLadderNotAligned();
		float isAnimationSynchron(float extendOffset) const;
		void updateAnimation(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		glm::vec3		mPostition;					///< The positing given as parameter (needed for multiplay)
		glm::vec3		mTargetPosition;			///< Target for ladder alignment
		qsf::Time		mTimePassed;				///< Time we used for animation of arm alignment, used to stop animation if it takes to long
		AnimationState	mLastAnimationState;

		// Temporary component pointers
		qsf::TransformComponent*				mLadderBaseTransformComponent;
		qsf::game::BoneToBoneLinkComponent*		mStageBoneToBoneLinkComponent;
		qsf::MeshAnimationComponent*			mLadderUpAnimation;
		qsf::MeshAnimationComponent*			mLadderExtendAnimation;
		qsf::MeshAnimationChannel*				mBaseUpAnimationChannel;
		qsf::MeshAnimationChannel*				mExtendAnimationChannel;
		qsf::WeakPtr<qsf::Entity>				mStateEntity;

		// Temporary variables
		float mRotateYawAngle;
		float mLadderUpAnimationOffset;
		float mExtendAnimationOffset;

		// Job and Audio proxies
		qsf::JobProxy mAnimationJobProxy;		///< Align ladder job proxy, for animation of Ladder movement
		AudioProxy	  mAudioProxy;

		// Exported to camp
		float mVerticalRescueOffset;
		float mVerticalDeployOffset;
		float mLadderMaxRotateSpeed;
		float mLadderMinRotateSpeed;
		float mLadderUpSpeed;
		float mLadderExtendSpeed;
		float mMaxAngle;
		float mMinLength;
		float mMaxLength;
		float mSynchronLength;
		float mTimeUltimatum;
		qsf::AssetProxy mAnimationLadderBaseUp;
		qsf::AssetProxy mAnimationLadderExtend;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/vehicle/parts/DLKLadderComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::DlkLadderComponent)

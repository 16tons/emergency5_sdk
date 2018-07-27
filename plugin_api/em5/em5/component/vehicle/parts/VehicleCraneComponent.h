// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"

#include <qsf/job/JobProxy.h>
#include <qsf/component/Component.h>
#include <qsf/logic/gamelogic/GameLogic.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class TransformComponent;
	class LinkComponent;
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
	*    EMERGENCY 5 vehicle crane component class
	*/
	class EM5_API_EXPORT VehicleCraneComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;			///< "em5::VehicleCraneComponent" unique component ID
		static const float YAW_EPSILON;				///< Epsilon to check if yaw orientation can be assumed equal
		static const float ANIMATION_EPSILON;		///< Epsilon to check if animation offset can be assumed equal
		static const float HOOK_DISTANCE_EPSILON;	///< Epsilon to check if hook movement distance can be assumed equal

		enum PivotMovement
		{
			PAUSE = 0,
			YAW_MOVEMENT,
			PITCH_MOVEMENT,
			EXTEND_MOVEMENT,
			YAW_AND_PITCH_MOVEMENT,
			PITCH_AND_EXTEND_MOVEMENT,
			YAW_AND_EXTEND_MOVEMENT,
			COCKPIT_MOVEMENT,
			PITCH_AND_COCKPIT_MOVEMENT
		};

		enum CraneType
		{
			TOW_CAR,
			BOAT_TRANSPORTER,
			HEAVY_CRANE
		};

		enum AnimationState
		{
			STATE_NONE,
			STATE_ALIGN_ARM,
			STATE_LOAD_CARGO_ARM,
			STATE_HOOK_MOVE,
			STATE_HOOK_MOVE_BACK,
			STATE_FOLD_ARM,
			STATE_STOP_ALL_ANIMATION
		};

		enum SequenceType
		{
			SEQUENCE_NONE,
			SEQUENCE_PICKUP_BY_TOWCAR_ALIGN_ARM,
			SEQUENCE_PICKUP_BY_TOWCAR_LOAD_CARGO_ARM,
			SEQUENCE_LIFT_BY_CRANE_ALIGN_ARM,
			SEQUENCE_LIFT_BY_CRANE_FOLD_ARM,
			SEQUENCE_UNLOAD_BOAT_ALIGN_ARM,
			SEQUENCE_UNLOAD_BOAT_FOLD_ARM,
			SEQUENCE_LOAD_BOAT_ALIGN_ARM,
			SEQUENCE_LOAD_BOAT_LOAD_CARGO_ARM
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
		inline explicit VehicleCraneComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~VehicleCraneComponent();

		/**
		*  @brief
		*    Check if target position is in range
		*
		*  @return
		*    "true" if a correction is necessary, "false" if target position is okay
		*/
		bool checkTargetPosition(const glm::vec3& targetPosition, glm::vec3& correctionOffset);

		/**
		*  @brief
		*    Move the arm of the crane to the given position
		*
		*  @param[in] position
		*    The position the arm should move to
		*/
		void alignArm(const glm::vec3& position);

		/**
		*  @brief
		*    Fold the arm without cargo
		*/
		void foldArm();

		/**
		*  @brief
		*    Load the cargo back to the vehicle
		*/
		void loadCargoArm();

		/**
		*  @brief
		*    Load the cargo back to the vehicle
		*
		*  @param[in] targetPosition
		*    The position of the target
		*  @param[in] distancePercentage
		*    0: position on the crane, 1: position on the target
		*/
		void hookMove(const glm::vec3& targetPosition, float distancePercentage);

		/**
		*  @brief
		*    Move back to starting position
		*
		*  @param[in] distancePercentage
		*    0: position on the crane, 1: old position
		*/
		void hookMoveBack(float distancePercentage = 0.0f);

		/**
		*  @brief
		*    Return if the animation is running or not
		*/
		bool isAnimationPlaying() const;

		inline SequenceType getSequenceType() const;

		/**
		*  @brief
		*    Movement will be in sequence defined the given vector
		*/
		void setSequenceType(SequenceType type);

		/**
		*  @brief
		*    Delete sequence and move synchronously
		*/
		void deleteSequence();

		/**
		*  @brief
		*    Stop all animations
		*/
		void stopAllAnimation();

		/**
		*  @brief
		*    Hide oder show the strap from the stage
		*/
		void setVisibleStrap(bool visible);

		/**
		*  @brief
		*    Un/link target to/from hook
		*
		*  @param[in] targetEntity
		*    Entity we want to link to the hook
		*/
		void linkTargetToHook(qsf::Entity& targetEntity);
		void unlinkTargetFromHook();

		/**
		*  @brief
		*    Get hook entity if there is one, or null pointer if not
		*/
		qsf::Entity* getHookEntity() const;

		/**
		*  @brief
		*    Get position in world space and add a local offset
		*/
		glm::vec3 worldSpacePositionWithLocalOffset(const qsf::TransformComponent& transformComponent, const glm::vec3& localoffset);

		/**
		*  @brief
		*    Get crane entity ID
		*/
		inline uint64 getCraneEntityId() const;
		void setCraneEntityId(uint64 craneEntityId);

		/**
		*  @brief
		*    Getter and setter for CAMP
		*/
		inline float getMinLength() const;
		inline void setMinLength(float minLength);

		inline float getMaxLength() const;
		inline void setMaxLength(float maxLength);

		inline float getMinAngle() const;
		inline void setMinAngle(float minAngle);

		inline float getMaxAngle() const;
		inline void setMaxAngle(float maxAngle);

		inline float getCraneHeight() const;
		inline void setCraneHeight(float craneHeight);

		inline float getArmRotateSpeed() const;
		inline void setArmRotateSpeed(float armRotateSpeed);

		inline float getExtendSpeed() const;
		inline void setExtendSpeed(float extendSpeed);

		inline float getLowToHighSpeed() const;
		inline void setLowToHighSpeed(float lowToHighSpeed);

		inline float getHookSpeed() const;
		inline void setHookSpeed(float hookSpeed);

		inline float getCockpitSpeed() const;
		inline void setCockpitSpeed(float cockpitSpeed);

		inline float getTimeUltimatum() const;
		inline void setTimeUltimatum(float timeUltimatum);

		inline CraneType getCraneType() const;
		inline void setCraneType(CraneType craneType);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(AnimationCraneExtend,    const qsf::AssetProxy&, mAnimationCraneExtend)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(AnimationCraneLowToHigh, const qsf::AssetProxy&, mAnimationCraneLowToHigh)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(AnimationCraneCockpit,   const qsf::AssetProxy&, mAnimationCraneCockpit)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(AnimationCraneHook,      const qsf::AssetProxy&, mAnimationCraneHook)

		inline AnimationState getLastAnimationState() const;
		inline const glm::vec3& getTargetPosition() const;
		inline bool isStrapVisible() const;
		inline float getHookMoveDistancePercentage() const;
		inline uint64 getEntityIdLinkedToHook() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startAnimation();
		void clearAnimation();
		void startHookAnimation();

		bool getComponentPointers();
		bool isNotAligned();
		void calculateAngleAndOffsets();
		void alignStage();
		bool getHookDummy();
		bool isHookMoving();

		void updateAnimation(const qsf::JobArguments& jobArguments);
		void updateHookAnimation(const qsf::JobArguments& jobArguments);

		qsf::AssetProxy getAnimationToUseForCraneExtend() const;
		qsf::AssetProxy getAnimationToUseForCraneLowToHigh() const;
		qsf::AssetProxy getAnimationToUseForCraneCockpit() const;
		qsf::AssetProxy getAnimationToUseForCraneHook() const;

		qsf::Entity* getFirstChildEntity(qsf::Entity* entity) const;
		qsf::Entity* getFirstChildEntityWithTag(qsf::Entity* entity, const std::string& tag) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64							mCraneEntityId;				///< Crane entity ID
		glm::vec3						mTargetPosition;			///< Target for arm alignment
		float							mDistancePercentage;		///< Percentage to target
		qsf::Time						mTimePassed;				///< Time we used for animation of arm alignment, used to stop animation if it takes to long
		qsf::Time						mPauseTimePassed;			///< Pause time calculation for animation
		qsf::Time						mPauseTime;					///< length of the pauses
		std::vector<PivotMovement>				mSequence;			///< Sequence for animation
		std::vector<PivotMovement>::size_type	mSequenceState;		///< State inside sequence
		SequenceType							mSequenceType;		///< The type of the sequence
		AnimationState							mLastAnimationState;
		bool									mStrapVisible;
		uint64									mEntityIdLinkedToHook;

		// Temporary used component pointers, use after getComponentPointers was called
		qsf::Entity*							mCraneEntity;
		qsf::Entity*							mJibEntity;
		qsf::Entity*							mCockpitEntity;
		qsf::Entity*							mHookDummy;
		qsf::TransformComponent*				mCraneTransformComponent;
		qsf::LinkComponent*						mCraneLinkComponent;
		qsf::MeshAnimationComponent*			mCraneMeshAnimationComponent;
		qsf::MeshAnimationComponent*			mJibMeshAnimationComponent;
		qsf::MeshAnimationComponent*			mCockpitMeshAnimationComponent;
		qsf::game::BoneToBoneLinkComponent*		mStageBoneToBoneLinkComponent;	///< Used as hookBoneToBoneLink if we have a heavy crane

		qsf::MeshAnimationChannel*				mExtendAnimationChannel;
		qsf::MeshAnimationChannel*				mLowToHighAnimationChannel;
		qsf::MeshAnimationChannel*				mCockpitAnimationChannel;

		// Temporarily used variables, use after getComponentPointers was called
		float			mRotateYawAngle;
		float			mExtendAnimationOffset;
		float			mLowToHighAnimationOffset;
		float			mCockpitAnimationOffset;
		float			mDistance;
		glm::vec3		mHookMoveTargetPosition;
		glm::vec3		mHookPosition;
		qsf::Time		mCockpitWaitTime;		///< Wait time for the cockpit after pitch alignment start in sequence PITCH_AND_COCKPIT_MOVEMENT

		// Job proxies and audio proxies
		qsf::JobProxy	mAnimationJobProxy;		///< Align ladder job proxy, for animation of ladder movement
		qsf::JobProxy	mHookAnimationJobProxy;	///< Hook animation job proxy, for animation of the hook of a heavy crane
		AudioProxy		mAudioProxy;			///< Audio proxy for crane movement
		AudioProxy		mHookAudioProxy;		///< Audio proxy for hook movement
		AudioProxy		mCockpitAudioProxy;		///< Audio proxy for cockpit movement

		// Exported to CAMP
		float mMinLength;
		float mMaxLength;
		float mMinAngle;
		float mMaxAngle;
		float mCraneHeight;
		float mArmRotateSpeed;
		float mExtendSpeed;
		float mLowToHighSpeed;
		float mHookSpeed;
		float mCockpitSpeed;
		float mTimeUltimatum;
		CraneType mCraneType;

		qsf::AssetProxy mAnimationCraneExtend;
		qsf::AssetProxy mAnimationCraneLowToHigh;
		qsf::AssetProxy mAnimationCraneCockpit;
		qsf::AssetProxy mAnimationCraneHook;


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
#include "em5/component/vehicle/parts/VehicleCraneComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::VehicleCraneComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::VehicleCraneComponent::CraneType)

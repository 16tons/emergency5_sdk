// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"

#include <qsf/component/Component.h>
#include <qsf/math/Transform.h>
#include <qsf/reflection/type/CampGlmVec3.h>
#include <qsf/reflection/type/CampQsfTime.h>
#include <qsf/reflection/type/CampGlmQuat.h>
#include <qsf/job/JobProxy.h>
#include <qsf/base/PropertyHelper.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/renderer/animation/MeshAnimationChannel.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>


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
	*    EMERGENCY 5 door component class
	*/
	class EM5_API_EXPORT DoorComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::DoorComponent" unique component ID
		static const uint32 DOOR_OFFSET;	///< "DoorOffset" unique class property ID inside the class
		static const uint32 DOOR_ROTATION;	///< "DoorRotation" unique class property ID inside the class

		/** Door state */
		enum DoorState
		{
			DOOR_CLOSED,	///< Door is closed
			DOOR_CLOSING,	///< Door is currently closing
			DOOR_OPEN,		///< Door is open
			DOOR_OPENING	///< Door is currently opening
		};

		/** Door type */
		enum DoorType
		{
			DOORTYPE_UNDEFINED = 0,		///< Undefined door type
			DOORTYPE_MAIN_ENTRANCE,		///< Building: Main entrance
			DOORTYPE_DRIVER,			///< Vehicle: Driver door
			DOORTYPE_CODRIVER,			///< Vehicle: Co-driver door
			DOORTYPE_PASSENGER,			///< Vehicle: Passenger door
			DOORTYPE_TRUNK,				///< Vehicle: Trunk door
			DOORTYPE_EQUIPMENTLEFT,		///< Vehicle: Left equipment door
			DOORTYPE_EQUIPMENTRIGHT		///< Vehicle: Right equipment door
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
		inline explicit DoorComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~DoorComponent();

		/**
		*  @brief
		*    Return current door state
		*
		*  @return
		*    The door state (open, closed, opening, closing)
		*/
		inline DoorState getDoorState() const;

		/**
		*  @brief
		*    Return percent of the current door state, depends on the animation -- open/close = 1.0f
		*/
		float getDoorStateInPercent() const;

		/**
		*  @brief
		*    Return percent of the current door position -- closed is 0.0f, open is 1.0f
		*/
		float getDoorAnimationPosition() const;

		/**
		*  @brief
		*    Open the door
		*/
		void openDoor();

		/**
		*  @brief
		*    Close the door
		*/
		void closeDoor();

		/**
		*  @brief
		*    Cut the door
		*/
		void cutDoor();

		inline const qsf::AssetProxy& getOpeningAnimation() const;
		inline void setOpeningAnimation(const qsf::AssetProxy& animation);

		inline const qsf::AssetProxy& getClosingAnimation() const;
		inline void setClosingAnimation(const qsf::AssetProxy& animation);

		inline const qsf::AssetProxy& getCuttingAnimation() const;
		inline void setCuttingAnimation(const qsf::AssetProxy& animation);

		// Define getters and setters for CAMP exported properties
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(RotationAxis, const glm::vec3&, mLocalRotationAxis)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(RotationAngle, float, mMaximumRotationAngle)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DoorType, DoorType, mDoorType)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(SlideOffset, const glm::vec3&, mSlideOffset)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(OpeningAnimationLength, qsf::Time, mOpeningAnimationLength)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ClosingAnimationLength, qsf::Time, mClosingAnimationLength)

		inline const glm::vec3& getDoorOffset() const;
		void setDoorOffset(const glm::vec3& value);

		inline const glm::quat& getDoorRotation() const;
		void setDoorRotation(const glm::quat& value);

		//[-------------------------------------------------------]
		//[ Derived data                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the world transform from the given transform
		*/
		void getDoorWorldSpaceTransform(qsf::Transform& result, const qsf::Transform* vehicleWorldTransform = nullptr) const;
		void getLocalToWorldTransform(qsf::Transform& transform) const;

		glm::vec3 getDoorOffsetWorldSpacePosition() const;
		glm::quat getDoorRotationWorldSpaceRotation() const;

		// Get a position in front of the door (at the outside) for the given distance
		void getWorldPositionInFrontOfDoor(float distance, glm::vec3& position, glm::quat& rotation) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onSetDebug(bool debug) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void activateJob();
		void updateJob(const qsf::JobArguments& jobArguments);
		void applyTransformationByAnimation(qsf::Transform& transform) const;
		bool hasAnimation() const;
		void updateWithAnimation();
		void updateWithoutAnimation(const qsf::JobArguments& jobArguments);
		void updateAudio();
		void startOpeningAnimation(float animationPosition = 0.0f);
		void startClosingAnimation(float animationPosition = 1.0f);
		void updateDoorDebugDraw(bool enabled);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		glm::vec3			mLocalRotationAxis;
		float				mMaximumRotationAngle;
		glm::vec3			mSlideOffset;
		glm::vec3			mDoorOffset;			///< The offset defines the interaction position of the door, local from the real door position
		glm::quat			mDoorRotation;			///< The rotation of the interaction position, looks at the door
		DoorType			mDoorType;
		qsf::Time			mOpeningAnimationLength;	///< Length of opening animation in seconds
		qsf::Time			mClosingAnimationLength;	///< Length of closing animation in seconds
		qsf::AssetProxy		mOpeningAnimation;			///< Animation for the door opening
		qsf::AssetProxy		mClosingAnimation;			///< Animation for the door closing
		qsf::AssetProxy		mCuttingAnimation;			///< Animation for the cutting door

		// Internal
		DoorState			mState;
		float				mAnimationPosition;			///< Current door position -- closed is 0.0f, open is 1.0f
		qsf::Transform		mOriginalLocalTransform;
		qsf::DebugDrawProxy	mDebugDrawProxy;			///< Only for debug
		qsf::JobProxy		mUpdateJobProxy;
		qsf::WeakPtr<qsf::MeshAnimationChannel> mAnimationChannel;
		AudioProxy			mAudioProxy;
		bool				mIsPlayingAudioStarted;


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
#include "em5/component/door/DoorComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::DoorComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::DoorComponent::DoorType)

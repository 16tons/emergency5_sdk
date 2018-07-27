// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/component/PlaySkeletalAnimationComponent.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BoneLookAtNode;
	class LocomotionNode;
}


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
	*    Extends "qsf::PlaySkeletalAnimationComponent" with character related parts (e.g. look at handling)
	*/
	class QSF_API_EXPORT PlayCharacterAnimationComponent : public PlaySkeletalAnimationComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::PlayCharacterAnimationComponent" unique component ID


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
		inline explicit PlayCharacterAnimationComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PlayCharacterAnimationComponent();

		/**
		*  @brief
		*    Get/set CAMP properties
		*/
		inline const std::string& getHeadBone() const;
		void setHeadBone(const std::string& headBone);

		inline uint64 getHeadLookAtEntityId() const;
		void setHeadLookAtEntityId(const uint64 headLookAtEntityId);

		/**
		*  @brief
		*    Let the head look at an entity (so called "HeadLookAt" feature)
		*
		*  @param[in] boneName
		*    The skeleton bone name
		*  @param[in] targetEntity
		*    A target entity we want to look at
		*  @param[in] maximalDistance
		*    Maximal Distance to entity to react to it
		*/
		void addHeadLookAtEntity(const std::string& boneName, Entity& targetEntity, float maximalDistance);

		/**
		*  @brief
		*    Remove the "HeadLookAt" feature
		*
		*  @note
		*    - The head will first smoothly move back to default position
		*/
		void removeHeadLookAtEntity();

		/**
		*  @brief
		*    Check if our "HeadLookAt" feature is active
		*/
		inline bool isHeadLookAtActive() const;

		/**
		*  @brief
		*    Get/set the head movement spring constant, resulting in movement acceleration and speed
		*
		*  @note
		*    - Default value is 100.0f, minimal speed is calculated as sqrt(springConstant) * 0.02
		*/
		inline float getHeadMovementSpringConstant() const;
		void setHeadMovementSpringConstant(float headMovementSpringConstant);

		/**
		*  @brief
		*    Get/set the animation influence of the head to the rotation when we use "HeadLookAt"
		*    - 0 Animation has no influence and is overwritten
		*    - 1 Animation is fully added to the rotation, No jump when "HeadLookAt" is removed
		*    - ]0,1[ Animation is mixed in (We can change the influence over time if we want)
		*
		*  @note
		*    - Remember to set the influence back to 1.0f if you want to remove the head lookat node without a jump in our animation
		*/
		inline float getHeadAnimationInfluence() const;
		void setHeadAnimationInfluence(float headAnimationInfluence);

		/**
		*  @brief
		*    Get/set the head rotation constrain when we use "HeadLookAt"
		*    - The constrain consists of the maximal possible euler angles (yaw, pitch, roll) for rotation
		*
		*  @note
		*    - With animation influence > 0 we can go over this constrains so be careful
		*/
		inline glm::vec3 getHeadRotationConstrain() const;
		void setHeadRotationConstrain(const glm::vec3& headRotationConstrain);

		/**
		*  @brief
		*    Get/set the angle after which we rotate the head to default instead of bending it
		*    - Head yaw rotation in euler angles from default position
		*
		*  @note
		*    - Expected to be higher as the yaw angle of the rotation constrain for good results
		*/
		inline float getHeadMovingToDefaultAngle() const;
		void setHeadMovingToDefaultAngle(float headMovingToDefaultAngle);

		/**
		*  @brief
		*    Add a locomotion node
		*
		*  @param[in] leftFootBoneName
		*    The skeleton bone name of the left foot, it needs to be the child of the knee bone and grand child of the hip bone
		*  @param[in] rightFootBoneName
		*    The skeleton bone name of the right foot
		*  @param[in] minimalHeight
		*    Minimal height in object space for the foot bone
		*  @param[in] maximalHeight
		*    Minimal height in object space for the foot bone
		*
		*  @note
		*    The locomotion node still isn't final and should change and include different features
		*/
		void addLocomotion(const std::string& leftFootBoneName, const std::string& rightFootBoneName, float minimalHeight = 0.0f, float maximalHeight = 0.5f);

		/**
		*  @brief
		*    Remove the locomotion node immediately
		*/
		void removeLocomotion();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;	// Overwritten functions from component to startup and shutdown the component


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::AdvancedAnimationComponent methods ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onPreCalculation(const JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Calculate the target position of the look-at node from target entity
		*/
		void updateLookAtPosition(float timeStep);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		BoneLookAtNode*		mBoneLookAtNode;
		LocomotionNode*		mLocomotionNode;

		WeakPtr<Entity>		mHeadLookAtEntity;
		float				mHeadMaximalDistance;
		float				mHeadMovementSpringConstant;
		float				mHeadAnimationInfluence;
		glm::vec3			mHeadRotationConstrain;
		float				mHeadMovingToDefaultAngle;

		// TODO(tl): We could move the properties to a new class to save memory if we use this component in a game
		// Camp Properties
		std::string			mHeadBone;
		uint64				mHeadLookAtEntityId;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/animation/advanced/component/PlayCharacterAnimationComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PlayCharacterAnimationComponent)

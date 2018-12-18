// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/reflection/object/Object.h>
#include <qsf/base/WeakPtr.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class MovableComponent;
	namespace ai
	{
		class NavigationComponent;
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
	*    Person animation class, handles the move cycle animation for persons
	*/
	class EM5_API_EXPORT PersonAnimationController : public qsf::Object
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum MovementState
		{
			MOVEMENTSTATE_UNDEFINED,	// Can also be custom animation
			MOVEMENTSTATE_IDLE,
			MOVEMENTSTATE_WALK,
			MOVEMENTSTATE_INJURED
		};
		enum PersonType
		{
			PERSONTYPE_OTHER,	// Split out more person types as needed
			PERSONTYPE_DIVER
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit PersonAnimationController(qsf::Entity& callerEntity);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PersonAnimationController();

		virtual void updateAnimation();

		// Reset the PersonAnimationController, so it can choose again what kind of animation should be played. Use this to interrupt animations.
		virtual void resetAnimationState();

		virtual void forceIdleAnimationUpdate();

		// Get the correct name for the injury of the given entity (returns an empty string when not injured etc)
		virtual std::string getInjuredAnimation(qsf::Entity& entity);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		MovementState calculateMovementState();

		bool setInjuredAnimation();

		void setAnimationState();

		void setMovementAnimationParamedics();
		void setMovementAnimationDoctor();
		void setMovementAnimationFireFighter();
		void setMovementAnimationDiver();
		void setMovementAnimationResueDogLeader();
		void setMovementAnimationPolicePerson();
		void setMovementAnimationSekSpecialist();
		void setMovementAnimationPoliceDogLeader();
		void setMovementAnimationArrested();
		void setMovementContaminatedLead();
		void setMovementAnimationUnit();

		bool blendToAnimation(qsf::Entity& entity, const std::string& animationName, float animationBlendTime) const;

		// Check if we care for the movement of this entity or of it's parent
		void checkMovingEntity();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		qsf::Entity&	mEntity;
		MovementState	mMovementState;
		MovementState	mFormerMovementState;
		PersonType		mPersonType;
		bool			mIsSwimming;		// Supported for diver only right now

		qsf::WeakPtr<qsf::Entity>					mMovingEntity;
		qsf::WeakPtr<qsf::MovableComponent>			mMovableComponent;
		qsf::WeakPtr<qsf::ai::NavigationComponent>	mNavigationComponent;
		qsf::WeakPtr<qsf::ActionComponent>			mActionComponent;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::PersonAnimationController)

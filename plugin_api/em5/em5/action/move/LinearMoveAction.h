// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/action/Action.h>

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class GameComponent;
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
	*    EMERGENCY 5 linear move action on XZ plane ignoring all collisions
	*/
	class EM5_API_EXPORT LinearMoveAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::MoveAction" unique action identifier
		static const float TOLERANCE;
		static const float DEFAULT_MAX_SPEED;
		static const float DEFAULT_ACCELERATION;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		LinearMoveAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~LinearMoveAction();

		/**
		*  @brief
		*    Initialization
		*
		*  @param[in] targetPosition
		*    Target position where we want to move
		*  @param[in] maxDistance
		*    Maximal Distance we move, if we are further away we cancel the action
		*  @param[in] minDistance
		*    Minimal Distance we move, if we are to close we ignore this the action, only checked at the start !
		*/
		void init(const glm::vec3& targetPosition, float minDistance = 0.0f, float maxDistance = std::numeric_limits<float>::max());


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Action methods                    ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onAbort() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		float getMovementSpeed(float oldSpeed, float secondsPassed, float distance);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:

		glm::vec3	mTargetPosition;
		float		mMinDistance;
		float		mMaxDistance;
		float       mMaxSpeed;
		float		mAcceleration;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::LinearMoveAction)

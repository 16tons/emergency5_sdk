// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/ai/MovementModes.h"

#include <qsf/logic/gamelogic/GameLogic.h>
#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobArguments;
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
	*    A game logic that lets entity drive to the side of the road when units on emergency operations ("Alarmfahrt") approach
	*/
	class EM5_API_EXPORT FollowMagnetLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::FollowMagnetLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		FollowMagnetLogic();

		void setTargetMagnet(uint64 magnetEntityId, const glm::vec3& relativeDistance = glm::vec3(0, 0, 0));
		uint64 getTargetMagnetEntityId() const;

		void setMovementMode(MovementModes::Modes movementMode);
		MovementModes::Modes getMovementMode() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Job updates
		void updateSimulation(const qsf::JobArguments& jobArguments);
		void onInjured(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32 ACTION_PRIORITY;	///< The priority of the actions generated from this logic


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy		mSimulationJobProxy;		///< Regular simulation job proxy; for updates once a game tick
		qsf::MessageProxy	mOnInjuredProxy;			///< Message proxy if person is injured/killed in action
		qsf::MessageProxy	mOnContaminatedProxy;		///< Message proxy if person is contaminated
		uint64				mTargetMagnetEntityId;		///< Entity to follow
		glm::vec3			mRelativeDistance;			///< relative distance to keep (in local coordinate system of target entity)
		bool				mIsInjured;
		MovementModes::Modes mMovementMode;				///< Movement mode to use for following (defaults to MovementModes::MOVEMENT_MODE_MAGNET)


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::FollowMagnetLogic)

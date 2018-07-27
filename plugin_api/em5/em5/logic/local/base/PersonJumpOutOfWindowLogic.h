// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/gamelogic/GameLogic.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/job/JobProxy.h>


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
	*    Logic for persons jumping out of burning buildings' windows
	*/
	class EM5_API_EXPORT PersonJumpOutOfWindowLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::PersonJumpOutOfWindowLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PersonJumpOutOfWindowLogic();

		/**
		*  @brief
		*    Simulate the jump of the person
		*
		*  @todo
		*    This is just a dummy, please make it better, faster, stronger^^
		*/
		void updateSimulationJump(const qsf::JobArguments& jobArguments);
		void updateSimulationBalcony(const qsf::JobArguments& jobArguments);


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
		void personHitsGround();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy		mSimulationJumpJobProxy;
		qsf::JobProxy		mSimulationBalconyJobProxy;
		qsf::MessageProxy	mMessageProxy;
		glm::vec3			mJump;				///< Vector of the jump speed of this person
		qsf::Time			mJumpWaitTime;		///< Wait time before the jump movement begins (is used to let the jump animation give some time to do part of the animation)


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PersonJumpOutOfWindowLogic)

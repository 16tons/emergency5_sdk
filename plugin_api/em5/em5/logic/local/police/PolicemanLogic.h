// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/logic/local/base/CarryPersonLogic.h"

#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageProxy.h>


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
	*    Policeman logic
	*/
	class EM5_API_EXPORT PolicemanLogic : public CarryPersonLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::PolicemanLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static bool areGangstersAround(qsf::Entity& entity);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		PolicemanLogic(uint32 gameLogicId = GAMELOGIC_TYPE_ID);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Job updates
		void updateSimulation(const qsf::JobArguments& jobArguments);

		bool performAutomatism(uint64 ignoreTarget);
		void abortAutomatism(bool tryToRecover = true);
		bool placePersonAutomatism(uint64 ignoreTarget);
		bool speakToPersonAutomatism();
		void onPersonPlacedInVehicle(const qsf::MessageParameters& parameters);
		qsf::Entity* findIdlingPolicemanAroundVehicle(qsf::Entity& vehicleEntity, uint32 filteredUnitType);
		void onPreCommandExecute(const qsf::MessageParameters& parameters);
		void onPolicemanInjured(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32 ACTION_PRIORITY;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy		mSimulationJobProxy;		///< Regular simulation job proxy; for updates once a game tick
		qsf::MessageProxy	mMessageProxy;				///< for messages
		qsf::MessageProxy	mPostCommandProxy;			///< Message proxy for all commandos
		qsf::MessageProxy	mOnInjuredProxy;			///< Message proxy if police unit is injured/killed in action
		qsf::MessageProxy	mOnContaminatedProxy;		///< Message proxy if police unit is contaminated
		qsf::Time			mTimeWaiting;
		glm::vec3			mAutomatismStartPos;		///< Position of the unit when the automatism kicks in
		bool				mIsAutomatismActive;		///< Should be true when the automatism is active, *not* true when the abort automatism is running
		uint64				mAutomatismTargetVehicle;	///< Target vehicle which the unit tries to reach


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PolicemanLogic)

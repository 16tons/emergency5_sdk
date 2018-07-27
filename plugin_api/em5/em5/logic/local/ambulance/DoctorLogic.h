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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ActionComponent;
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
	*    Doctor logic
	*/
	class EM5_API_EXPORT DoctorLogic : public CarryPersonLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::DoctorLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		DoctorLogic();

		/**
		*   @brief
		*    React to the exit vehicle command
		*
		*   @note
		*    The Doctor has medicase equipment in his hand. He take it from the back of an NEF or from the front door of an RHC.
		*    This is an callback function were we listen to the exit vehicle command to push actions by hand.
		*    The tricky part is to reserved the vehicle correctly, to allow only one doctor interacting with the vehicle at the same time.
		*/
		void takeMedikitFromVehicle(const qsf::MessageParameters& parameters);

		/**
		*   @brief
		*    React to the enter vehicle command
		*
		*   @note
		*    Listen to the enter vehicle command and put the medicase back to the vehicle.
		*/
		void putMedikitInVehicle(const qsf::MessageParameters& parameters);

		void startWaitForEnterVehicle(const qsf::MessageParameters& parameters);
		void abortWaitForEnterVehicle(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Public virtual em5::CarryPersonLogic methods          ]
	//[-------------------------------------------------------]
	public:
		virtual void setCarryPerson(uint64 carryPersonId) override;


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
		static const uint32 ACTION_PRIORITY;	///< Action priority


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Job updates
		void updateSimulation(const qsf::JobArguments& jobArguments);

		void performAutomatism();
		void performTreatmentAutomatism();

		void pushSignalActionAndSetupMessageHandler(uint64 vehicleEntityId, qsf::ActionComponent& actionComponent);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Job proxies for regular update
		qsf::JobProxy	mSimulationJobProxy;		///< Regular simulation job proxy; for updates once a game tick
		qsf::Time		mTimeWaiting;

		// We need this message proxy to get or put away the medikit and have a safe serialization
		qsf::MessageProxy mExitVehicleProxy;		///< Message proxy for post commando messages
		qsf::MessageProxy mEnterVehicleProxy;		///< Message proxy for on commando messages
		qsf::MessageProxy mEnterVehicleWithoutEquipmentProxy;
		qsf::MessageProxy mEnterVehicleWithoutEquipmentAbortProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DoctorLogic)

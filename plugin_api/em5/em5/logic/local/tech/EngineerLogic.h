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
	*    Engineer logic
	*/
	class EM5_API_EXPORT EngineerLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::EngineerLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		EngineerLogic();


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
		/**
		*   @brief
		*    React to the exit vehicle command
		*
		*   @note
		*	 Mostly copy-pasted from DoctorLogic::takeMedikitFromVehicle
		*/
		void takeToolboxFromVehicle(const qsf::MessageParameters& parameters);

		/**
		*   @brief
		*    React to the enter vehicle command
		*
		*   @note
		*	 Mostly copy-pasted from DoctorLogic::putMedikitInVehicle
		*/
		void putToolboxInVehicle(const qsf::MessageParameters& parameters);

		void startWaitForEnterVehicle(const qsf::MessageParameters& parameters);
		void abortWaitForEnterVehicle(const qsf::MessageParameters& parameters);

		void pushSignalActionAndSetupMessageHandler(uint64 vehicleEntityId, qsf::ActionComponent& actionComponent);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// We need this message proxy to get or put away the toolbox and have a safe serialization
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
QSF_CAMP_TYPE_NONCOPYABLE(em5::EngineerLogic)

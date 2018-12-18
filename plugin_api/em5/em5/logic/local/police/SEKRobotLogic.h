// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/message/MessageProxy.h>
#include <qsf/job/JobProxy.h>
#include <qsf/logic/gamelogic/GameLogic.h>


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
	*    SEK robot logic
	*/
	class EM5_API_EXPORT SEKRobotLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::SEKRobotLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		SEKRobotLogic();

		/**
		*  @brief
		*    Get the connected transporter ID
		*
		*  @return
		*    Returns the ID from the transporter to which this SEK robot belongs to
		*/
		uint64 getConnectedTransporterId() const;

		/**
		*  @brief
		*    Set the connected transporter ID
		*
		*  @param[in] transporter
		*    Set the ID from the transporter to which this SEK robot belongs to
		*/
		void setConnectedTransporterId(uint64 transporter);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Job updates
		void updateSimulation(const qsf::JobArguments& jobArguments);

		void onTransporterStartsBurning(const qsf::MessageParameters& parameters);


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
		uint64			  mConnectedTransporterId;	///< The entity ID of the connected transport vehicle
		bool			  mIsIntact;
		qsf::JobProxy	  mSimulationJobProxy;		///< Regular simulation job proxy; for updates once a game tick
		qsf::MessageProxy mMessageProxyConnectedTransporterDestroyed;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::SEKRobotLogic)

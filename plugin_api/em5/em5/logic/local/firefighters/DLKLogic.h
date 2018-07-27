// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/logic/gamelogic/GameLogic.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class BuildingComponent;
}
namespace qsf
{
	class Entity;
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
	*    DLK logic ("Drehleiter Kran Einsatzfahrzeug")
	*/
	class EM5_API_EXPORT DLKLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::DLKLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get place of action building
		*
		*  @param[in] entity
		*    The entity to check
		*
		*  @return
		*    Null pointer if the entity is not a building or a person inside a building
		*
		*  @note
		*    - If it's a person try to get the building component of the container
		*    - If it's a building return the building component
		*    - If it's a industrial plant return the building component
		*/
		static BuildingComponent* getPlaceOfActionBuilding(qsf::Entity& entity);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DLKLogic();

		/**
		*  @brief
		*    Check if the DLK is deployed and extinguishing fire
		*/
		bool isDeployed() const;

		/**
		*  @brief
		*    Next time the DLK moves it needs to undeploy
		*/
		void undeployBeforeMovingNext();


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
		void performAutomatism();
		void rescuePersonAutomatism();
		void updateSimulation(const qsf::JobArguments& jobArguments);
		void undeploy(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Time	mTimeWaiting;	///< Time we wait for automatism
		bool		mDeployed;

		// Job proxies
		qsf::JobProxy		mSimulationJobProxy;	///< Regular simulation job proxy; for updates once a game tick

		// Message proxy for all possible Commands
		qsf::MessageProxy	mUndeployMessageProxyMoveCommand;
		qsf::MessageProxy	mUndeployMessageProxyReturnToBaseCommand;
		qsf::MessageProxy	mUndeployMessageProxyDLKSafePersonCommand;
		qsf::MessageProxy	mUndeployMessageProxyDLKExtinguishCommand;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DLKLogic)

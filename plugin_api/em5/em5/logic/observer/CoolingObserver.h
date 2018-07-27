// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/observer/Observer.h"

#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Objective;
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
	*    EMERGENCY 5 logic observer cooling observer
	*
	*  @note
	*    - This observer is listening to the colling sources changed message
	*    - If the cooling sources changed it checked if enough cooling sources exist it starts the simulation
	*    - After the simulation has reached the goal it updates the objective
	*/
	class EM5_API_EXPORT CoolingObserver : public Observer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::CoolingObserver" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CoolingObserver();

		/**
		*  @brief
		*    Connect this observer to an objective to be informed when an buried entity gets free (unhidden)
		*/
		void initialize(Objective& objective, uint32 minCoolingSources, float neededCoolingEnergy);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    React tho change of the cooling sources
		*/
		void onCoolingEnergySourcesChange(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    Check if the conditions are met and update the simulation
		*/
		void updateSimulation(const qsf::JobArguments& jobArguments);

		/**
		*  @brief
		*    Check if the conditions are met and what proxy must be active
		*/
		void checkCooling();

		/**
		*  @brief
		*    Shortcut to get the current number of plague cloud cooling sources
		*/
		uint32 getCoolingSourceCount();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Constant data
		uint32						mObjectiveId;
		uint32						mMinCoolingSources;
		float						mNeededCoolingEnergy;
		qsf::MessageConfiguration	mCoolingMessage;

		// Internal data
		float				mLeftCoolingEnergy;
		qsf::MessageProxy	mCoolingMessageProxy;
		qsf::JobProxy		mCoolingJobProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::CoolingObserver)

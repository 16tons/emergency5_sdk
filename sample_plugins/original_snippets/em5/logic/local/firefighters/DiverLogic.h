// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/logic/local/base/CarryPersonLogic.h"

#include <qsf_ai/base/UnsignedFloat.h>

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
	*    Diver logic
	*/
	class EM5_API_EXPORT DiverLogic : public CarryPersonLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::DiverLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		DiverLogic();

		bool isSwimming() const;
		bool isCarryingPerson() const;
		float getSwimmingOffset() const;

		void giveCarryOffsetToCarriedPerson();

		void setWalkSpeed(qsf::UnsignedFloat speed);
		void setSwimSpeed(qsf::UnsignedFloat speed);


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
		// Job update
		void updateSimulation(const qsf::JobArguments& jobArguments);

		void updateMovementSpeed();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy	   mSimulationJobProxy;		///< Regular simulation job proxy; for updates once a game tick
		bool			   mIsSwimming;
		float			   mSwimmingOffset;			///< Interpolation offset between standing on land (0.0f) and swimming (1.0f)
		qsf::UnsignedFloat mWalkSpeed;
		qsf::UnsignedFloat mSwimSpeed;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DiverLogic)

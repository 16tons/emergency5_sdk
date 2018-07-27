// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/observer/Observer.h"


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
	*    EMERGENCY 5 logic observer vehicle was damaged
	*
	*  @note
	*    - This observer listens to the vehicle damaged message
	*    - If a vehicle was damaged at least in damage state 2 or destroyed, the observer increases the objective
	*/
	class EM5_API_EXPORT VehicleDamagedGeneralObserver : public Observer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::VehicleDamagedGeneralObserver" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		VehicleDamagedGeneralObserver();

		void setFilterCivilists(bool allowForCivilists);
		void setFilterUnits(bool allowForUnits);

		/**
		*  @brief
		*    Connect this observer to an objective to be informed when a person dies
		*
		*  @param[in] objective
		*    The objective instance to connect to
		*/
		void connectToObjective(Objective& objective);


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
		void onVehicleDamaged(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			  mFilterCivilists;
		bool			  mFilterUnits;
		uint32			  mObjectiveId;
		qsf::MessageProxy mMessageProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::VehicleDamagedGeneralObserver)

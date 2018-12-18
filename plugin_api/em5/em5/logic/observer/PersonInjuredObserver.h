// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/observer/Observer.h"

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
	*    EMERGENCY 5 logic observer person died
	*
	*  @note
	*    - This observers listens to the person injured message
	*    - If the person is injured the observer increases the objective number and dies
	*/
	class EM5_API_EXPORT PersonInjuredObserver : public Observer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::PersonInjuredObserver" game logic type ID

		enum ObjectiveOperation
		{
			OBJECTIVE_OPERATION_INCREASE_NEEDED_NUMBER,
			OBJECTIVE_OPERATION_DECREASE_NEEDED_NUMBER,
			OBJECTIVE_OPERATION_INCREASE_CURRENT_NUMBER,
			OBJECTIVE_OPERATION_DECREASE_CURRENT_NUMBER,
		};

	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PersonInjuredObserver();

		/**
		*  @brief
		*    Connect this observer to an objective to be informed when a person dies
		*
		*  @param[in] objective
		*    The objective instance to connect to
		*  @param[in] operationOnObjective
		*    What to do with the connected objective when the person dies
		*/
		void connectToObjective(Objective& objective, ObjectiveOperation operationOnObjective = OBJECTIVE_OPERATION_INCREASE_CURRENT_NUMBER);


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
		void onPersonInjured(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32				mObjectiveId;
		qsf::MessageProxy	mMessageProxy;
		uint32				mObjectiveOperation;	//< By intent not of the enum type so it can be serialized easily

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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PersonInjuredObserver)

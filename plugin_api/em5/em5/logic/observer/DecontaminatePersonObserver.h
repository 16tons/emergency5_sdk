// Copyright (C) 2012-2017 Promotion Software GmbH


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
	*    EMERGENCY 5 logic observer person was decontaminated
	*
	*  @note
	*    - This observer listens to the decontaminated person message and the person injured message
	*    - If the person was decontaminated, the observer increases the objectives current number and dies
	*    - If the person was injured, it creates the treat person objective, increases needed number and a treat
	*      person observer and connects the observer to the objective.
	*/
	class EM5_API_EXPORT DecontaminatePersonObserver : public Observer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::DecontaminatePersonObserver" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DecontaminatePersonObserver();

		/**
		*  @brief
		*    Connect this observer to an objective to be informed when an infected person was healed
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
		void onPersonInjured(const qsf::MessageParameters& parameters);
		void onPersonDecontaminated(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32				mObjectiveId;			///< ID if the objective
		qsf::MessageProxy	mDecontaminationProxy;	///< Registered at person decontaminated message
		qsf::MessageProxy	mInjuredProxy;			///< Registered at person injured message
		bool				mHasDecontaminated;		///< Person got decontaminated in this observer


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DecontaminatePersonObserver)

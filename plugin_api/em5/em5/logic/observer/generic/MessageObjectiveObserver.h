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
	*    EMERGENCY 5 generic message observer
	*
	*  @note
	*    - This observer is listening to an arbitrary message
	*    - It is a game logic instance inside the entity affected by the message
	*/
	class EM5_API_EXPORT MessageObjectiveObserver : public Observer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::MessageObjectiveObserver" game logic type ID

		enum class Reaction
		{
			NONE = 0,
			OBJECTIVE_DEC_CURRENT,
			OBJECTIVE_INC_CURRENT,
			OBJECTIVE_DEC_NEEDED,
			OBJECTIVE_INC_NEEDED
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MessageObjectiveObserver();

		/**
		*  @brief
		*    Initialize this observer with a message to listen to and an objective to manipulate
		*
		*  @param[in] messageConfiguration
		*    The message to listen to
		*  @param[in] objective
		*    The objective instance to connect to
		*  @param[in] reaction
		*    Defines what to do with the objective when the message triggers
		*  @param[in] removeAfterTrigger
		*    If "true", this observer will get removed after its message triggered
		*/
		void initWithObjective(const qsf::MessageConfiguration& messageConfiguration, Objective& objective, Reaction reaction, bool removeAfterTrigger = true);

		void addMessage(const qsf::MessageConfiguration& messageConfiguration);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor for sub-classes
		*/
		explicit MessageObjectiveObserver(uint32 gameLogicTypeId);


	//[-------------------------------------------------------]
	//[ Protected virtual em5::MessageObjectiveObserver methods ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Callback for sub-class implementations: return "false" to ignore this incoming message
		*/
		virtual bool checkIncomingMessage(const qsf::MessageParameters& parameters)  { return true; }


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onMessage(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<qsf::MessageConfiguration> mMessageConfigurations;
		std::vector<qsf::MessageProxy>		   mMessageProxies;

		uint32	 mObjectiveId;
		Reaction mReaction;
		bool	 mRemoveAfterTrigger;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MessageObjectiveObserver)

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
	*    EMERGENCY 5 general observer logic that reacts on a given message configuration
	*
	*  @note
	*    - This observer observes a message
	*    - The reaction to the message is the increase of the current number or the needed number
	*/
	class EM5_API_EXPORT MessageObjectiveGeneralObserver : public Observer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::MessageObjectiveGeneralObserver" game logic type ID

		enum ObserverReactionType
		{
			REACTION_NONE,
			REACTION_DEC_CURRENT_NUMBER,
			REACTION_INC_CURRENT_NUMBER,
			REACTION_DEC_NEEDED_NUMBER,
			REACTION_INC_NEEDED_NUMBER
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MessageObjectiveGeneralObserver();

		/**
		*  @brief
		*    Register the message proxy if the target entity is the containing entity
		*
		*  @param[in] configuration
		*    The message configuration of the observed message
		*/
		void registerMessage(const qsf::MessageConfiguration& configuration);

		/**
		*  @brief
		*    Register the message proxy if the target entities have their ID in the filters
		*
		*  @param[in] configuration
		*    The message configuration of the observed message
		*  @param[in] entityIdFilter
		*    The position of the filter where the entity ID is
		*/
		void registerMessage(const qsf::MessageConfiguration& configuration, uint32 entityIdFilter);

		/**
		*  @brief
		*    Register the message proxy if the target entities have their ID in the parameters
		*
		*  @param[in] configuration
		*    The message configuration of the observed message
		*  @param[in] entityIdParameter
		*    The name of the parameter where the entity ID is
		*/
		void registerMessage(const qsf::MessageConfiguration& configuration, std::string entityIdParameter);

		/**
		*  @brief
		*    Connect this observer to an objective to be informed when an infected person was healed
		*
		*  @param[in] objective
		*    The objective instance to connect to
		*  @param[in] observerReaction
		*    Bit mask parameter that changes the observer reaction
		*/
		void connectToObjective(Objective& objective, ObserverReactionType observerReaction);


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
		*    Just registers the message proxy at the given configuration
		*/
		void registerMessageProxy(const qsf::MessageConfiguration& configuration);

		/**
		*  @brief
		*   Get the entity from the message parameters
		*
		*  @return
		*    The a pointer to the found entity or "nullptr" as error value
		*/
		qsf::Entity* getEntityFromMessage(const qsf::MessageParameters& parameters);

		/**
		*  @brief
		*    The reaction of the observer if the message was send
		*/
		void onMessage(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32						mObjectiveId;		///< ID if the associated objective
		ObserverReactionType		mObserverReaction;	///< Bit mask that tells the observer what to do on the reaction
		qsf::MessageProxy			mMessageProxy;		///< Message proxy
		qsf::MessageConfiguration	mConfiguration;		///< The configuration of the message that is listened to
		uint32						mEntityIdFilter;	///< The position if the entity ID in the filters
		std::string					mEntityIdParameter;	///< The name of the entity ID in the parameters


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MessageObjectiveGeneralObserver)

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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 logic message observer
	*
	*/
	class EM5_API_EXPORT MessageObserver : public Observer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::MessageObserver" game logic type ID

		enum LogicType
		{
			OR_LOGIC = 0,		// ObserverMessage when Message1 OR Message2
			AND_LOGIC			// ObserverMessage when Message1 AND Message2
		};

		enum MessageTypeForAnd
		{
			FALSE_CHECK = 0,	// Only used to check for the expression
			TRUE_REACT,			// Create an ObserverMessage if the expression is true
			TRUE_CHECK,			// Only used to check for the expression
		};

		// bool parameter: true = second message, false = first message
		typedef boost::function<bool (const qsf::MessageParameters&, bool)> CustomFilterCallback;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MessageObserver();

		/**
		*  @brief
		*    An EM5_OBSERVER_MESSAGE with this observer's ID as filter is created when message 1 and/or message 2 is sent
		*
		*  @param[in] message1
		*    Message configuration for message 1
		*  @param[in] message2
		*    Message configuration for message 2
		*  @param[in] logicType
		*    Type of the logic concatenation, can be OR_LOGIC and AND_LOGIC
		*/
		void initialize(const qsf::MessageConfiguration& message1, const qsf::MessageConfiguration& message2, LogicType logicType);

		/**
		*  @brief
		*    An EM5_OBSERVER_MESSAGE with this observer's ID as filter is created when the logic is true
		*
		*  @param[in] message1
		*    Message configuration for message 1
		*  @param[in] message2
		*    Message configuration for message 2
		*  @param[in] logicType
		*    Type of the logic concatenation, can be OR_LOGIC and AND_LOGIC
		*  @param[in] messageType2
		*    How message2 is evaluated when AND is used; we always react to message1!
		*/
		void initialize(const qsf::MessageConfiguration& message1, const qsf::MessageConfiguration& message2, LogicType logicType, MessageTypeForAnd messageType2);

		/**
		*  @brief
		*    Sets a custom message filter callback, which gets called when a message arrives to allow additional filtering if that message should be counted as the to be observed message
		*
		*  @param[in] customFilterCallback
		*    The callback for the custom filtering
		*/
		void setCustomMessageFilter(const CustomFilterCallback& customFilterCallback);


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
		void onMessage1(const qsf::MessageParameters& parameters);
		void onMessage2(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		LogicType				mLogicType;
		qsf::MessageProxy		mMessageProxy1;
		qsf::MessageProxy		mMessageProxy2;
		bool					mActivatedMessage1;
		bool					mActivatedMessage2;
		MessageTypeForAnd		mMessageType2;
		CustomFilterCallback	mCustomMessageFilter;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MessageObserver)

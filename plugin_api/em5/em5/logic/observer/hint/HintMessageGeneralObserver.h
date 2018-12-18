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
	*    EMERGENCY 5 logic hint observer for messages
	*
	*  @note
	*    - 5 cases are passible to check for:
	*    - case 0: Just callback now
	*    - case 1: Just observe a message and callback when a message is created
	*    - case 2: After a timeframe check if a message was created and callback
	*    - case 3: After a timeframe check if a message was not created and callback
	*    - case 4: Adds a message that adds a delay to the timer
	*
	*    After the callback is called the observer is removed
	*/
	class EM5_API_EXPORT HintMessageGeneralObserver : public Observer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::HintMessageGeneralObserver" game logic type ID

		// bool parameter: true = delay message, false = first message
		typedef boost::function<bool (const qsf::MessageParameters&, bool)> CustomFilterCallback;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		HintMessageGeneralObserver();

		/**
		*  @brief
		*    This constructor should be used for case 0
		*/
		void initialize();

		/**
		*  @brief
		*    This constructor should be used for case 1
		*
		*  @param[in] messageId
		*    Id of the message we want to observe
		*/
		void initialize(const qsf::MessageConfiguration& messageConfiguration);

		/**
		*  @brief
		*    This constructor should be used for case 2 and 3
		*
		*  @param[in] messageConfiguration
		*    Configuration of the message we want to observe in a timeframe
		*
		*  @param[in] timeFrame
		*    Set the timeframe after which it is checked if a message was created or not
		*
		*  @param[in] messageCreated
		*    Set if this observer should react if a message was created or not (true for case 2 and false for case 3)
		*/
		void initialize(const qsf::MessageConfiguration& messageConfiguration, const qsf::Time& timeFrame, const bool messageCreated);

		/**
		*  @brief
		*    This constructor should be used for case 4
		*
		*  @param[in] delayMessageConfiguration
		*    Adds a message that delays the observer
		*
		*  @param[in] messageConfiguration
		*    Configuration of the message we want to observe in a timeframe
		*
		*  @param[in] timeFrame
		*    Set the timeframe after which it is checked if a message was created or not
		*
		*  @param[in] messageCreated
		*    Set if this observer should react if a message was created or not (true for case 2 and false for case 3)
		*/
		void initialize( const qsf::MessageConfiguration& delayMessageConfiguration, const qsf::MessageConfiguration& messageConfiguration,
			const qsf::Time& timeFrame, const bool messageCreated);

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
		void onDelayMessage(const qsf::MessageParameters& parameters);
		void onMessage(const qsf::MessageParameters& parameters);
		void onTimer(const qsf::MessageParameters& parameters);

		void serializeMessageProxy(qsf::BinarySerializer& serializer, qsf::MessageProxy& messageProxy, const qsf::detail::MessageCallbackBinding& function);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool						mMessageCreated;		///< On what case to react after the timeFrame
		bool						mMessageFound;			///< Was a message found after the timeFrame

		qsf::Time					mTimeFrame;				///< Timeframe where we check for the message

		qsf::MessageProxy			mDelayProxy;			///< Message that adds a delay to this observer
		qsf::MessageProxy			mMessageProxy;			///< Message that this observer listens to
		qsf::MessageProxy			mTimerProxy;			///< Message for the Timer

		CustomFilterCallback		mCustomMessageFilter;	///< Custom filter callback


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::HintMessageGeneralObserver)

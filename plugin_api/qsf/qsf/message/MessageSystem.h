// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"
#include "qsf/message/MessageProxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class MessageConfiguration;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Message system class
	*/
	class QSF_API_EXPORT MessageSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MessageProxy;	// May register and unregister itself


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline MessageSystem();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MessageSystem();

		/**
		*  @brief
		*    Emit a message, i.e. call all listeners registered for this message
		*
		*  @param[in] message
		*    The message to emit, consisting of a message identifier and optional filters
		*/
		void emitMessage(const MessageConfiguration& message);

		/**
		*  @brief
		*    Emit a message, i.e. call all listeners registered for this message
		*
		*  @param[in] message
		*    The message to emit, consisting of a message identifier and optional filters
		*  @param[in] parameters
		*    Message parameters structure to pass additional information or to get responses from the message listeners
		*/
		void emitMessage(const MessageConfiguration& message, MessageParameters& parameters);

		/**
		*  @brief
		*    Emit a message and evaluate the response
		*
		*  @param[in] message
		*    The message to emit, consisting of a message identifier and optional filters
		*  @param[out] response
		*    The variable to write the response to; does not get touched in case there is not response
		*
		*  @return
		*    "true" if there was a response at all, "false" if not
		*/
		template<typename T>
		bool emitMessageWithResponse(const MessageConfiguration& message, T& response);

		/**
		*  @brief
		*    Emit a message and evaluate the response
		*
		*  @param[in] message
		*    The message to emit, consisting of a message identifier and optional filters
		*  @param[out] response
		*    The variable to write the response to; does not get touched in case there is not response
		*  @param[in] parameters
		*    Message parameters structure to pass additional information or to get responses from the message listeners
		*
		*  @return
		*    "true" if there was a response at all, "false" if not
		*/
		template<typename T>
		bool emitMessageWithResponse(const MessageConfiguration& message, T& response, MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Register a message listener proxy
		*
		*  @param[in] message
		*    The message configuration to register the proxy at, consisting of a message identifier and optional filters
		*  @param[in] proxy
		*    The message listener proxy to unregister
		*  @param[in] function
		*    The function be invoked if the according message gets emitted
		*
		*  @remarks
		*    Avoid using this method directly, instead call the proxy's registerAt() method
		*/
		void registerListener(const MessageConfiguration& message, MessageProxy& proxy, const detail::MessageCallbackBinding& function);

		/**
		*  @brief
		*    Unregister a message listener proxy
		*
		*  @param[in] proxy
		*    The message listener proxy to unregister
		*
		*  @remarks
		*    Avoid using this method directly, instead call the proxy's unregister() method
		*/
		void unregisterListener(MessageProxy& proxy);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MessageManager* mRootManager;	///< Root message manager, can be a null pointer in case the message system is not started yet


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/message/MessageSystem-inl.h"

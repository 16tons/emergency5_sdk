// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/Proxy.h"
#include "qsf/message/MessageParameters.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class MessageManager;
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
	*    Message listener proxy class
	*
	*  @note
	*    - Best practice: Declare a message listener proxy directly as a part of the class you want to get called as a message
	*       listener (-> composition), avoid dynamic allocation (new MessageProxy) and never define a function-local message
	*       listener proxy. The reason is the listener's lifetime being connected to the proxy's lifetime: When the proxy is
	*       destroyed, the proxy is automatically unregistered.
	*       This is done mainly for security reasons, to avoid calling of a method of an object that was already destroyed.
	*/
	class QSF_API_EXPORT MessageProxy : public Proxy
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MessageManager;	///< Message manager may change private data


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline MessageProxy();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~MessageProxy();

		/**
		*  @brief
		*    Return message manager or null pointer if proxy is invalid, do not destroy the instance
		*/
		inline MessageManager* getMessageManager() const;

		/**
		*  @brief
		*    Returns false if proxy is not registered or has become invalid
		*/
		inline bool isValid() const;

		/**
		*  @brief
		*    Return listener's ID or "qsf::getUninitialized<uint32>()" if proxy is invalid
		*/
		inline uint32 getId() const;

		/**
		*  @brief
		*    Register for a message; throws an exception on fatal error
		*
		*  @param[in] message
		*    Configuration of the message to register at
		*  @param[in] function
		*    A function binding to the method to be called when the message is emitted
		*
		*  @return
		*    "true" if registration was successful, "false" if it failed
		*
		*  @note
		*    - When called multiple times, the previous registration will be cleared first (as you'd call unregister() manually before each registerAt() call)
		*/
		bool registerAt(const MessageConfiguration& message, const boost::function<detail::MessageCallbackSignature>& function);

		/**
		*  @brief
		*    Unregister for the current message
		*
		*  @return
		*    "true" if unregistration was successful, "false" if proxy was not registered at all
		*/
		inline bool unregister();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline void invalidate();
		bool unregisterInternal();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MessageManager* mMessageManager;	///< The message manager instance this proxy is assigned to, can be a null pointer, do not destroy the instance
		uint32			mListenerId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/message/MessageProxy-inl.h"

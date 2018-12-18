// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/message/MessageParameters.h"
#include "qsf/base/manager/BufferedManager.h"
#include "qsf/base/idgenerator/ConsecutiveIdGenerator.h"

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class MessageProxy;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	namespace detail
	{
		/** Internal representation of message listener registration data */
		struct MessageListener
		{
			bool					mValid;
			MessageProxy*			mMessageProxy;		///< Message proxy, always valid, do not destroy the instance
			MessageCallbackBinding	mFunctionBinding;
		};

		typedef BufferedManager<uint32, detail::MessageListener, boost::container::flat_map<uint32, detail::MessageListener>> MessageManagerBase;
	}


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Message manager class
	*
	*  @remarks
	*    Each message manager instance represents a single message or an already filtered message and
	*    stores a list of all listeners registered to this (filtered) message.
	*    Message managers form a tree hierarchy with managers for filtered messages being children of
	*    the manager representing the respective unfiltered message.
	*/
	class QSF_API_EXPORT MessageManager : private detail::MessageManagerBase, public MessageResponseReceiver
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		MessageManager();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Private method because only job system may destroy job manager instances
		*/
		virtual ~MessageManager();

		/**
		*  @brief
		*    Register a listener
		*/
		bool registerListener(MessageProxy& proxy, const detail::MessageCallbackBinding& function);

		/**
		*  @brief
		*    Register a listener
		*/
		bool registerListenerRecursive(MessageProxy& proxy, const detail::MessageCallbackBinding& function, const MessageConfiguration& message, uint32 filterLevel = 0);

		/**
		*  @brief
		*    Unregister a listener; throws an exception on fatal error
		*/
		bool unregisterListener(MessageProxy& proxy);

		/**
		*  @brief
		*    Unregister all listeners
		*/
		void unregisterAllListeners();

		/**
		*  @brief
		*    Emit a message to all listeners
		*/
		void emitMessage(MessageParameters& parameters);

		/**
		*  @brief
		*    Emit a message to all listeners
		*/
		bool emitMessageRecursive(MessageParameters& parameters, const MessageConfiguration& message, uint32 filterLevel = 0);


	//[-------------------------------------------------------]
	//[ Public qsf::MessageResponseReceiver methods           ]
	//[-------------------------------------------------------]
	public:
		virtual void onMessageResponseReceived(const MessageParameters& parameters, const camp::Value& response) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::BufferedManager methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual void onElementRemoval(const ElementKey& key, Element& element) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ConsecutiveIdGenerator<uint32> mIdGenerator;

		typedef boost::container::flat_map<uint64, MessageManager*> MessageManagerMap;
		MessageManagerMap mChildMap;

		bool mResponseReceived;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

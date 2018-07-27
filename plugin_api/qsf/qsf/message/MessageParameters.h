// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/message/MessageConfiguration.h"
#include "qsf/message/MessageResponseReceiver.h"
#include "qsf/base/PropertyDictionary.h"

#include <boost/function.hpp>


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
	*    Message parameters class
	*/
	class MessageParameters
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MessageSystem;				// May directly write private members
	friend class MessageResponseReceiver;	// May directly write private members


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline MessageParameters();

		/**
		*  @brief
		*    Set a parameter value
		*
		*  @param[in] identifier
		*    Identifier or name of the parameter to set
		*  @param[in] value
		*    The value to set; must be of a type connected to the CAMP reflection system
		*/
		template<typename T>
		void setParameter(const StringHash& identifier, const T& value);

		/**
		*  @brief
		*    Get a parameter value and write it to the given variable
		*
		*  @param[in] identifier
		*    Identifier or name of the parameter to return
		*  @param[in] value
		*    Variable reference to write the parameter value to; must be of a type connected to the CAMP reflection system
		*
		*  @return
		*    "true" on success, "false" if the parameter does not exist or could not be converted to the needed type
		*/
		template<typename T>
		bool getParameter(const StringHash& identifier, T& value) const;

		/**
		*  @brief
		*    Return a parameter value
		*
		*  @param[in] identifier
		*    Identifier or name of the parameter to return
		*
		*  @return
		*    The parameter value
		*
		*  @note
		*    - Throws an exception in case the identifier could not be found
		*/
		template<typename T>
		T getParameterSafe(const StringHash& identifier) const;

		/**
		*  @brief
		*    Set a response for the message
		*/
		template<typename T>
		void respond(const T& response) const;

		/**
		*  @brief
		*    Try to get the response converted to the given variable's type
		*/
		template<typename T>
		bool tryGetResponse(T& response) const;

		/**
		*  @brief
		*    Returns if a response was given
		*/
		inline bool hasResponse() const;

		/**
		*  @brief
		*    Return the message configuration
		*/
		const MessageConfiguration& getConfiguration() const;

		/**
		*  @brief
		*    Return the number of filters in message configuration
		*
		*  @return
		*    The number of filters; the message ID itself counts as filter, too, so this is always >= 1
		*
		*  @notes
		*    - This is just a shortcut, you can also ask the message configuration instead
		*/
		inline uint32 getNumberOfFilters() const;

		/**
		*  @brief
		*    Return a filter of message configuration by its index
		*
		*  @return
		*    The filter value, or "getUninitialized<uint64>()" if it does not exist
		*
		*  @notes
		*    - Index 0 always refers to the message ID, the actual filters begin at index 1
		*    - This is just a shortcut, you can also ask the message configuration instead
		*/
		template<typename T = uint64>
		T getFilter(uint32 index) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PropertyDictionary mParameters;				///< Generic parameters map
		const MessageConfiguration* mConfiguration;	///< Message configuration, may be a null pointer

		// Response
		MessageResponseReceiver* mResponseReceiver;	///< Message response receiver
		mutable camp::Value mResponse;				///< Response to the message


	};


	//[-------------------------------------------------------]
	//[ Definitions                                           ]
	//[-------------------------------------------------------]
	namespace detail
	{
		typedef void MessageCallbackSignature(const MessageParameters&);
		typedef boost::function<MessageCallbackSignature> MessageCallbackBinding;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/message/MessageParameters-inl.h"

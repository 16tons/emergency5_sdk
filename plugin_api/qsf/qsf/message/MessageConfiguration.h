// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class StringHash;
	class BinarySerializer;
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
	*    Message configuration class
	*
	*  @note
	*    - Only supports up to four filters
	*    - For performance reasons, a plain C-array of fixed size is used for the filters
	*/
	class QSF_API_EXPORT MessageConfiguration
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] messageId
		*    The message identifier, which is the message's hashed name
		*/
		inline MessageConfiguration(const StringHash& messageId);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] messageId
		*    The message identifier, which is the message's hashed name
		*  @param[in] filter1
		*    First filter value (index 1); message listeners can filter incoming messages by automatically ignoring other values
		*/
		inline MessageConfiguration(const StringHash& messageId, uint64 filter1);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] messageId
		*    The message identifier, which is the message's hashed name
		*  @param[in] filter1
		*    First filter value (index 1); message listeners can filter incoming messages by automatically ignoring other values
		*  @param[in] filter2
		*    Second filter value (index 2)
		*/
		inline MessageConfiguration(const StringHash& messageId, uint64 filter1, uint64 filter2);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] messageId
		*    The message identifier, which is the message's hashed name
		*  @param[in] filter1
		*    First filter value (index 1); message listeners can filter incoming messages by automatically ignoring other values
		*  @param[in] filter2
		*    Second filter value (index 2)
		*  @param[in] filter3
		*    Third filter value (index 3)
		*/
		inline MessageConfiguration(const StringHash& messageId, uint64 filter1, uint64 filter2, uint64 filter3);

		/**
		*  @brief
		*    Copy constructor
		*/
		inline MessageConfiguration(const MessageConfiguration& messageConfiguration);

		/**
		*  @brief
		*    Return the number of filters
		*
		*  @return
		*    The number of filters; the message ID itself counts as filter, too, so this is always >= 1
		*/
		inline uint32 getNumberOfFilters() const;

		/**
		*  @brief
		*    Return a filter by its index
		*
		*  @return
		*    The filter value, or "getUninitialized<uint64>()" if it does not exist
		*
		*  @remarks
		*    Index 0 always refers to the message ID, the actual filters begin at index 1
		*/
		template<typename T = uint64>
		T getFilter(uint32 index) const;

		/**
		*  @brief
		*    Compare to another message configuration
		*
		*  @return
		*    "true" if both are equal, "false" otherwise
		*/
		inline bool operator==(const MessageConfiguration& other) const;

		/**
		*  @brief
		*    Serialization of the message configuration
		*/
		void serialize(BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32 mFilterCount;	///< Number of filters, between 0 and 4
		uint64 mFilters[4];		///< The filter values


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/message/MessageConfiguration-inl.h"

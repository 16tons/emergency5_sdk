// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BasicTypeSerialization.h"


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
	*    Consecutive ID generator template class
	*/
	template <typename TYPE>
	class ConsecutiveIdGenerator
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] minimumId
		*    The lowest allowed ID; this one will be returned in first generate() call
		*/
		ConsecutiveIdGenerator(TYPE minimumId = 1);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] minimumId
		*    The lowest allowed ID; this one will be returned in first generate() call
		*  @param[in] maximumId
		*    The highest allowed ID
		*/
		ConsecutiveIdGenerator(TYPE minimumId, TYPE maximumId);

		/**
		*  @brief
		*    Return a new ID
		*/
		TYPE generate();

		/**
		*  @brief
		*    Serialize or deserialize the ID generator state using a binary serializer
		*
		*  @param[in] serializer
		*    The serializer, which can be either in read or write mode
		*/
		void serialize(BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		TYPE mNextId;		///< ID that will be returned in next generate()
		TYPE mMaximumId;	///< The highest valid ID that is possible


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/idgenerator/ConsecutiveIdGenerator-inl.h"

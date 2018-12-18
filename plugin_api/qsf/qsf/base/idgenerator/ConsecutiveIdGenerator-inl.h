// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"

#include <limits>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	template <typename TYPE>
	ConsecutiveIdGenerator<TYPE>::ConsecutiveIdGenerator(TYPE minimumId) :
		mNextId(minimumId),
		mMaximumId(std::numeric_limits<TYPE>::max())
	{
		// Nothing here
	}

	template <typename TYPE>
	ConsecutiveIdGenerator<TYPE>::ConsecutiveIdGenerator(TYPE minimumId, TYPE maximumId) :
		mNextId(minimumId),
		mMaximumId(maximumId)
	{
		// Nothing here
	}

	template <typename TYPE>
	TYPE ConsecutiveIdGenerator<TYPE>::generate()
	{
		QSF_CHECK(mNextId < mMaximumId, "Cannot generate another ID because the maximum was reached!", QSF_REACT_THROW);
		return mNextId++;
	}

	template <typename TYPE>
	void ConsecutiveIdGenerator<TYPE>::serialize(BinarySerializer& serializer)
	{
		serializer & mNextId;

		// Maximum ID is not serialized by intend; it should not change during run-time at all
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

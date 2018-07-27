// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MessageConfiguration::MessageConfiguration(const StringHash& messageId) :
		mFilterCount(1)
	{
		if (qsf::isInitialized(messageId))
		{
			mFilters[0] = messageId.getHash();
		}
		else
		{
			qsf::setUninitialized<uint64>(mFilters[0]);
			mFilterCount = 0;
		}
	}

	inline MessageConfiguration::MessageConfiguration(const StringHash& messageId, uint64 filter) :
		mFilterCount(2)
	{
		mFilters[0] = messageId.getHash();
		mFilters[1] = filter;
	}

	inline MessageConfiguration::MessageConfiguration(const StringHash& messageId, uint64 filter1, uint64 filter2) :
		mFilterCount(3)
	{
		mFilters[0] = messageId.getHash();
		mFilters[1] = filter1;
		mFilters[2] = filter2;
	}

	inline MessageConfiguration::MessageConfiguration(const StringHash& messageId, uint64 filter1, uint64 filter2, uint64 filter3) :
		mFilterCount(4)
	{
		mFilters[0] = messageId.getHash();
		mFilters[1] = filter1;
		mFilters[2] = filter2;
		mFilters[3] = filter3;
	}

	inline MessageConfiguration::MessageConfiguration(const MessageConfiguration& messageConfiguration) :
		mFilterCount(messageConfiguration.mFilterCount)
	{
		mFilters[0] = messageConfiguration.mFilters[0];
		mFilters[1] = messageConfiguration.mFilters[1];
		mFilters[2] = messageConfiguration.mFilters[2];
		mFilters[3] = messageConfiguration.mFilters[3];
	}

	inline uint32 MessageConfiguration::getNumberOfFilters() const
	{
		return mFilterCount;
	}

	template<typename T>
	T MessageConfiguration::getFilter(uint32 index) const
	{
		QSF_CHECK(index < mFilterCount, "Invalid index for MessageConfiguration::getFilter()", QSF_REACT_THROW);
		return static_cast<T>(mFilters[index]);
	}

	inline bool MessageConfiguration::operator==(const MessageConfiguration& other) const
	{
		if (mFilterCount != other.mFilterCount)
			return false;

		for (uint32 i = 0; i < mFilterCount; ++i)
		{
			if (mFilters[i] != other.mFilters[i])
				return false;
		}
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

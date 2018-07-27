// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline const std::vector<uint64>& ValueHistory::getValues() const
		{
			return mValues;
		}

		inline const boost::container::flat_map<uint16, uint64>& ValueHistory::getPerSecondValues() const
		{
			return mPerSecondData;
		}

		inline uint64 ValueHistory::getLowestValue() const
		{
			return mLowestValue;
		}

		inline uint64 ValueHistory::getHighestValue() const
		{
			return mHighestValue;
		}

		inline const std::unordered_map<uint8, ValueHistory>& ComponentValueHistory::getList() const
		{
			return mComponentValueHistory;
		}

		inline size_t ComponentValueHistory::getValueCount() const
		{
			return mValueCount;
		}

		inline const std::unordered_map<uint8, ValueHistory>& MultiValueHistory::getList() const
		{
			return mMultiValueHistory;
		}

		inline size_t MultiValueHistory::getValueCount() const
		{
			return mValueCount;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/network/multiplayer/datacache/statistics/MapCacheStatistics.h"
#include "em5/network/multiplayer/datacache/DataCacheItemFactory.h"

#include <qsf/application/ApplicationContext.h>
#include <qsf/base/GetUninitialized.h>
#include <qsf/Qsf.h>

#include <boost/nowide/fstream.hpp>


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
		ValueHistory:: ValueHistory(size_t historyLimit) :
			mHistoryLimit(historyLimit),
			mLowestValue(qsf::getUninitialized<uint64>(0)),
			mHighestValue(0),
			mCurrentValueCount(0),
			mCurrentSecond(1)
		{
			// Reserver space for the history limit if limit is set
			if (historyLimit > 0)
			{
				mValues.reserve(historyLimit);
			}
		}

		void ValueHistory::addValue(const uint64 value, uint16 timePassedInMilliseconds)
		{
			if (mHistoryLimit > 0 && mHistoryLimit == mValues.size())
			{
				mValues.erase(mValues.begin());
			}

			mValues.emplace_back(value);

			if (0 == mCurrentValueCount)
			{
				mPerSecondData[mCurrentSecond] = value;
			}
			else
			{
				mPerSecondData[mCurrentSecond] += value;
			}

			mCurrentValueCount += timePassedInMilliseconds;

			if (mCurrentValueCount >= 1000)
			{
				mCurrentValueCount = 0;
				++mCurrentSecond;
			}

			if (value > mHighestValue)
			{
				mHighestValue = value;
			}

			if (value < mLowestValue)
			{
				mLowestValue = value;
			}
		}

		ComponentValueHistory::ComponentValueHistory(const size_t historyLimit) :
			mValueCount(0),
			mHistoryLimit(historyLimit)
		{
			// Initialize for each tracked component the value history entry
			uint8 maxSize = static_cast<uint8>(DataCacheItemFactory::TrackedComponentType::_NumTypes);
			for (uint8 index = 0; index < maxSize; ++index)
			{
				mComponentValueHistory.emplace(std::piecewise_construct, std::forward_as_tuple(index), std::forward_as_tuple(historyLimit));
			}
		}

		void ComponentValueHistory::addValue(uint8 index, uint64 value, uint16 timePassedInMilliseconds)
		{
			// We use index = 0 as counter
			if (index == 0)
			{
				if (mHistoryLimit == 0 || mHistoryLimit > mValueCount)
				{
					mValueCount++;
				}
			}

			auto iterator = mComponentValueHistory.find(index);
			if (iterator != mComponentValueHistory.end())
			{
				iterator->second.addValue(value, timePassedInMilliseconds);
			}
		}

		void ComponentValueHistory::saveToFile() const
		{
			std::string savePath(qsf::Qsf::instance()->getApplicationContext().getApplicationDataUserDirectory());
			savePath += "/mapcache_values.csv";
			boost::nowide::ofstream file(savePath, std::ios::trunc);
			if (file)
			{
				file<<"index";
				for (uint8 index = 0; index < static_cast<uint8>(DataCacheItemFactory::TrackedComponentType::_NumTypes); ++index)
				{
					file<<","<<DataCacheItemFactory::componentIdToString(index);
				}
				file<<"\n";

				size_t maxCount = getValueCount();
				auto& valueList = getList();
				for (uint16 count = 0; count < maxCount; ++count)
				{
					file<<count+1;
					for (uint8 index = 0; index < static_cast<uint8>(DataCacheItemFactory::TrackedComponentType::_NumTypes); ++index)
					{
						auto findIterator = valueList.find(index);
						if (findIterator != valueList.end())
						{
							const std::vector<uint64>& history = findIterator->second.getValues();
							file<<","<<history[count];
						}
						else
						{
							file<<",0";
						}
					}
					file<<"\n";
				}
				file<<"\n";

				file<<"index";
				for (uint8 index = 0; index < static_cast<uint8>(DataCacheItemFactory::TrackedComponentType::_NumTypes); ++index)
				{
					file<<","<<DataCacheItemFactory::componentIdToString(index);
				}
				file<<"\n";
				size_t maxSeconds = getList().begin()->second.getPerSecondValues().size();

				for (size_t seconds = 0; seconds < maxSeconds; ++seconds)
				{
					uint16 curSecond = static_cast<uint16>(seconds+1);
					file<<curSecond;

					for (uint8 index = 0; index < static_cast<uint8>(DataCacheItemFactory::TrackedComponentType::_NumTypes); ++index)
					{
						auto findIterator = valueList.find(index);
						if (findIterator != valueList.end())
						{
							const boost::container::flat_map<uint16, uint64>& history = findIterator->second.getPerSecondValues();
							uint64 value = 0;
							auto findIterator = history.find(curSecond);
							if (findIterator != history.end())
							{
								value = findIterator->second;
							}
							file<<","<<value;
						}
						else
						{
							file<<",0";
						}
					}
					file<<"\n";
				}
				file<<"\n";
			}
		}

		MultiValueHistory::MultiValueHistory(const size_t historyLimit) :
			mValueCount(0),
			mHistoryLimit(historyLimit)
		{

		}

		void MultiValueHistory::addEntryType(const std::string& name)
		{
			if (mEntryNameList.size() < 255)
			{
				uint8 index = static_cast<uint8>(mEntryNameList.size());
				mEntryNameList.emplace_back(name);

				mMultiValueHistory.emplace(std::piecewise_construct, std::forward_as_tuple(index), std::forward_as_tuple(mHistoryLimit));
			}
		}

		void MultiValueHistory::addValue(uint8 index, uint64 value, uint16 timePassedInMilliseconds)
		{
			if (mEntryNameList.empty() || index >= mEntryNameList.size())
			{
				return;
			}

			// We use index = 0 as counter
			if (index == 0)
			{
				if (mHistoryLimit == 0 || mHistoryLimit > mValueCount)
				{
					mValueCount++;
				}
			}

			auto iterator = mMultiValueHistory.find(index);
			if (iterator != mMultiValueHistory.end())
			{
				iterator->second.addValue(value, timePassedInMilliseconds);
			}
		}

		void MultiValueHistory::saveToFile() const
		{
			std::string savePath(qsf::Qsf::instance()->getApplicationContext().getApplicationDataUserDirectory());
			savePath += "/multivalue_values.csv";
			boost::nowide::ofstream file(savePath, std::ios::trunc);
			if (file)
			{
				file<<"index";
				for (const std::string& name : mEntryNameList)
				{
					file<<","<<name;
				}
				file<<"\n";

				auto& valueList = getList();

				for (uint16 count = 0; count < getValueCount(); ++count)
				{
					file<<count;
					for (uint8 index = 0; index < mEntryNameList.size(); ++index)
					{
						auto findIterator = valueList.find(index);
						if (findIterator != valueList.end())
						{
							const std::vector<uint64>& history = findIterator->second.getValues();
							file<<","<<history[count];
						}
						else
						{
							file<<",0";
						}
					}
					file<<"\n";
				}
				file << "\n";

				file << "index";
				for (const std::string& name : mEntryNameList)
				{
					file << "," << name;
				}
				file << "\n";

				size_t maxSeconds = getList().begin()->second.getPerSecondValues().size();

				for (size_t seconds = 0; seconds < maxSeconds; ++seconds)
				{
					uint16 curSecond = static_cast<uint16>(seconds+1);
					file << curSecond;
					for (uint8 index = 0; index < mEntryNameList.size(); ++index)
					{
						auto valueIterator = valueList.find(index);
						if (valueIterator != valueList.end())
						{
							const boost::container::flat_map<uint16, uint64>& history = valueIterator->second.getPerSecondValues();
							uint64 value = 0;
							auto historyIterator = history.find(curSecond);
							if (historyIterator != history.end())
							{
								value = historyIterator->second;
							}
							file << "," << value;
						}
						else
						{
							file << ",0";
						}
					}
					file << "\n";
				}
				file << "\n";
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5

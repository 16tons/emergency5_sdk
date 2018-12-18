// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <boost/noncopyable.hpp>

#include <vector>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    EMERGENCY 5 multiplayer value history
		*/
		class ValueHistory : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] historyLimit
			*    The maximum limit of values to be stored, a value of zero (0) indicates unlimited storage (use this with caution)
			*/
			ValueHistory(size_t historyLimit);

			/**
			*  @brief
			*    Adds a value to the history. When the maximum limit is reached the "oldest" value (first element in the list) gets removed
			*
			*  @param[in] value
			*    The value to be inserted to the history
			*  @param[in] timePassedInMilliseconds
			*    The time passed since last value
			*/
			void addValue(uint64 value, uint16 timePassedInMilliseconds);

			/**
			*  @brief
			*    Returns the stored values
			*/
			inline const std::vector<uint64>& getValues() const;

			/**
			*  @brief
			*    Returns the stored per second values
			*/
			inline const boost::container::flat_map<uint16, uint64>& getPerSecondValues() const;

			/**
			*  @brief
			*    Returns the lowest value which was added to the history
			*/
			inline uint64 getLowestValue() const;

			/**
			*  @brief
			*    Returns the highest value which was added to the history
			*/
			inline uint64 getHighestValue() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<uint64>							mValues;		///< Stores the history values
			size_t										mHistoryLimit;	///< Holds the history limit
			uint64										mLowestValue;	///< The lowest value which was added to the history
			uint64										mHighestValue;	///< The highest value which was added to the history
			boost::container::flat_map<uint16, uint64>	mPerSecondData; ///< Stores the history values per second
			uint16										mCurrentValueCount;
			uint16										mCurrentSecond;

		};


		/**
		*  @brief
		*    EMERGENCY 5 multiplayer per component value history
		*/
		class ComponentValueHistory : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] historyLimit
			*    The maximum limit of values to be stored, a value of zero (0) indicates unlimited storage (use this with caution). Defaults to 40 entries per component
			*/
			ComponentValueHistory(const size_t historyLimit = 40);

			/**
			*  @brief
			*    Adds a value to the history of the given component. Nothing is added when the component id is unknown
			*
			*  @param[in] componentId
			*    The id of the component for which the value should be added
			*  @param[in] value
			*    The value to be inserted to the history
			*  @param[in] timePassedInMilliseconds
			*    The time passed since last value
			*/
			void addValue(uint8 componentId, uint64 value, uint16 timePassedInMilliseconds);

			/**
			*  @brief
			*    Saves the per component history as csv to a file named "mapcache_values.csv". The location is the application user data directory
			*/
			void saveToFile() const;

			/**
			*  @brief
			*    Returns the list of the stored per component value history
			*/
			inline const std::unordered_map<uint8, ValueHistory>& getList() const;

			/**
			*  @brief
			*    Returns how many entries each value history entry has (This value is only correct for all entries, when always for all components a value was added)
			*/
			inline size_t getValueCount() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::unordered_map<uint8, ValueHistory>	mComponentValueHistory;	///< Stores the per component history values
			size_t									mValueCount;			///< Holds how many entries each history entry has
			size_t									mHistoryLimit;			///< Holds the history limit


		};


		/**
		*  @brief
		*    EMERGENCY 5 multiplayer per component value history
		*/
		class MultiValueHistory : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] historyLimit
			*    The maximum limit of values to be stored, a value of zero (0) indicates unlimited storage (use this with caution). Defaults to 40 entries per component
			*/
			MultiValueHistory(const size_t historyLimit = 40);

			void addEntryType(const std::string& name);

			/**
			*  @brief
			*    Adds a value to the history of the given component. Nothing is added when the component id is unknown
			*
			*  @param[in] componentId
			*    The id of the component for which the value should be added
			*  @param[in] value
			*    The value to be inserted to the history
			*  @param[in] timePassedInMilliseconds
			*    The time passed since last value
			*/
			void addValue(uint8 componentId, uint64 value, uint16 timePassedInMilliseconds);

			/**
			*  @brief
			*    Saves the per component history as csv to a file named "mapcache_values.csv". The location is the application user data directory
			*/
			void saveToFile() const;

			/**
			*  @brief
			*    Returns the list of the stored per component value history
			*/
			inline const std::unordered_map<uint8, ValueHistory>& getList() const;

			/**
			*  @brief
			*    Returns how many entries each value history entry has (This value is only correct for all entries, when always for all components a value was added)
			*/
			inline size_t getValueCount() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::unordered_map<uint8, ValueHistory>	mMultiValueHistory;		///< Stores the per entry history values
			std::vector<std::string>				mEntryNameList;
			size_t									mValueCount;			///< Holds how many entries each history entry has
			size_t									mHistoryLimit;			//< Holds the history limit


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // multiplayer
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/network/multiplayer/datacache/statistics/MapCacheStatistics-inl.h"

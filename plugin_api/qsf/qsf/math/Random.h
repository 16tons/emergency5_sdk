// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#ifdef _MSC_VER
	#include <xutility>
#endif
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>

#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>


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
	*    Static random number generation helper class
	*
	*  @todo
	*    - TODO(fw): This is just for ease-of-use of the QSF AI / Boost random number generation.
	*      It's not very efficient this way, and for the game, we might want to have better control over random number generation.
	*/
	class QSF_API_EXPORT Random
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns a random floating point value in the range [min...max], that is *including* min and max as possible values
		*/
		static float getRandomFloat(float minimum, float maximum);

		/**
		*  @brief
		*    Returns a random signed integer in the range [min...max], that is *including* min and max as possible values
		*/
		static int32 getRandomInt(int32 minimum, int32 maximum);

		/**
		*  @brief
		*    Returns a random unsigned integer in the range [min...max], that is *including* min and max as possible values
		*/
		static uint32 getRandomUint(uint32 minimum, uint32 maximum);

		/**
		*  @brief
		*    Returns a random size_t value in the range [min...max], that is *including* min and max as possible values
		*/
		static size_t getRandomSize(size_t minimum, size_t maximum);

		inline static bool getRandomChance(float chance);

		//[-------------------------------------------------------]
		//[ Any of                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get any of container
		*
		*  @note
		*    - Get any of will only work, if the container has random access iterators
		*/
		template <typename RandomAccessIterator>
		static RandomAccessIterator getAnyOf(RandomAccessIterator begin, RandomAccessIterator end);

		/**
		*  @brief
		*    Get any of std::vector container
		*
		*  @note
		*    - Throws an exception for an empty container, so check this first
		*/
		template <typename T>
		static const T& getAnyOf(const std::vector<T>& container);

		/**
		*  @brief
		*    Get any of std::vector container
		*/
		template <typename T>
		static const T& getAnyOf(const std::vector<T>& container, const T& fallbackIfEmpty);

		/**
		*  @brief
		*    Get any of std::set container
		*
		*  @note
		*    - Throws an exception for an empty container, so check this first
		*/
		template <typename T>
		static const T& getAnyOf(const std::set<T>& container);
		template <typename T>
		static const T& getAnyOf(const std::unordered_set<T>& container);
		template <typename T>
		static const T& getAnyOf(const boost::container::flat_set<T>& container);

		/**
		*  @brief
		*    Get any of std::set container
		*/
		template <typename T>
		static const T& getAnyOf(const std::set<T>& container, const T& fallbackIfEmpty);
		template <typename T>
		static const T& getAnyOf(const std::unordered_set<T>& container, const T& fallbackIfEmpty);
		template <typename T>
		static const T& getAnyOf(const boost::container::flat_set<T>& container, const T& fallbackIfEmpty);

		/**
		*  @brief
		*    Get any of std::map container
		*
		*  @note
		*    - Throws an exception for an empty container, so check this first
		*/
		template <typename T, typename S>
		static const typename std::map<T, S>::const_iterator getAnyOf(const std::map<T, S>& container);
		template <typename T, typename S>
		static const typename std::unordered_map<T, S>::const_iterator getAnyOf(const std::unordered_map<T, S>& container);
		template <typename T, typename S>
		static const typename boost::container::flat_map<T, S>::const_iterator getAnyOf(const boost::container::flat_map<T, S>& container);

		/**
		*  @brief
		*    Get any of std::map container
		*/
		template <typename T, typename S>
		static const typename std::map<T, S>::const_iterator getAnyOf(const std::map<T, S>& container, const T& fallbackIfEmpty);
		template <typename T, typename S>
		static const typename std::unordered_map<T, S>::const_iterator getAnyOf(const std::unordered_map<T, S>& container, const T& fallbackIfEmpty);
		template <typename T, typename S>
		static const typename boost::container::flat_map<T, S>::const_iterator getAnyOf(const boost::container::flat_map<T, S>& container, const T& fallbackIfEmpty);

		//[-------------------------------------------------------]
		//[ Pick out any of                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get any of std::vector container and erase the instance from the container
		*
		*  @note
		*    - Throws an exception for an empty container, so check this first
		*/
		template <typename T>
		static T getPickOutAnyOf(std::vector<T>& container);

		/**
		*  @brief
		*    Get any of std::set container and erase the instance from the container
		*
		*  @note
		*    - Throws an exception for an empty container, so check this first
		*/
		template <typename T>
		static T getPickOutAnyOf(std::set<T>& container);
		template <typename T>
		static T getPickOutAnyOf(std::unordered_set<T>& container);
		template <typename T>
		static T getPickOutAnyOf(boost::container::flat_set<T>& container);

		/**
		*  @brief
		*    Get any of std::map container and erase the instance from the container
		*
		*  @note
		*    - Throws an exception for an empty container, so check this first
		*/
		template <typename T, typename S>
		static T getPickOutAnyOf(std::map<T, S>& container);
		template <typename T, typename S>
		static T getPickOutAnyOf(std::unordered_map<T, S>& container);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/math/Random-inl.h"

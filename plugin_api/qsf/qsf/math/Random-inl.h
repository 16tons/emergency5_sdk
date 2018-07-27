// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline bool Random::getRandomChance(float chance)
	{
		return (getRandomFloat(0.0f, 1.0f) < chance);
	}

	template <typename RandomAccessIterator>
	RandomAccessIterator Random::getAnyOf(RandomAccessIterator begin, RandomAccessIterator end)
	{
		static_assert(std::is_same<typename std::iterator_traits<RandomAccessIterator>::iterator_category, std::random_access_iterator_tag>::value, "getAnyOf will only work with random access iterators");

		const std::size_t size = std::distance(begin, end);
		if (size == 0)
		{
			return end;
		}
		return begin + getRandomSize(0, size - 1);
	}

	template <typename T>
	const T& Random::getAnyOf(const std::vector<T>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getAnyOf() for an empty std::vector", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);
		return container[index];
	}

	template <typename T>
	const T& Random::getAnyOf(const std::vector<T>& container, const T& fallbackIfEmpty)
	{
		if (container.empty())
			return fallbackIfEmpty;

		const size_t index = getRandomSize(0, container.size() - 1);
		return container[index];
	}

	template <typename T>
	const T& Random::getAnyOf(const std::set<T>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getAnyOf() for an empty std::set", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);

		typename std::set<T>::const_iterator iterator = container.begin();
		std::advance(iterator, index);

		return *iterator;
	}

	template <typename T>
	const T& Random::getAnyOf(const std::unordered_set<T>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getAnyOf() for an empty std::unordered_set", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);

		typename std::unordered_set<T>::const_iterator iterator = container.begin();
		std::advance(iterator, index);

		return *iterator;
	}

	template <typename T>
	const T& Random::getAnyOf(const boost::container::flat_set<T>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getAnyOf() for an empty boost::container::flat_set", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);

		typename boost::container::flat_set<T>::const_iterator iterator = container.begin();
		std::advance(iterator, index);

		return *iterator;
	}

	template <typename T>
	const T& Random::getAnyOf(const std::set<T>& container, const T& fallbackIfEmpty)
	{
		if (container.empty())
			return fallbackIfEmpty;

		const size_t index = getRandomSize(0, container.size() - 1);

		typename std::set<T>::const_iterator iterator = container.begin();
		std::advance(iterator, index);

		return *iterator;
	}

	template <typename T>
	const T& Random::getAnyOf(const std::unordered_set<T>& container, const T& fallbackIfEmpty)
	{
		if (container.empty())
			return fallbackIfEmpty;

		const size_t index = getRandomSize(0, container.size() - 1);

		typename std::unordered_set<T>::const_iterator iterator = container.begin();
		std::advance(iterator, index);

		return *iterator;
	}

	template <typename T>
	const T& Random::getAnyOf(const boost::container::flat_set<T>& container, const T& fallbackIfEmpty)
	{
		if (container.empty())
			return fallbackIfEmpty;

		const size_t index = getRandomSize(0, container.size() - 1);

		typename boost::container::flat_set<T>::const_iterator iterator = container.begin();
		std::advance(iterator, index);

		return *iterator;
	}

	template <typename T, typename S>
	const typename std::map<T, S>::const_iterator Random::getAnyOf(const std::map<T, S>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getAnyOf() for an empty std::map", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);

		typename std::map<T, S>::const_iterator iterator = container.begin();
		std::advance(iterator, index);

		return iterator;
	}

	template <typename T, typename S>
	const typename std::unordered_map<T, S>::const_iterator Random::getAnyOf(const std::unordered_map<T, S>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getAnyOf() for an empty std::unordered_map", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);

		typename std::unordered_map<T, S>::const_iterator iterator = container.begin();
		std::advance(iterator, index);

		return iterator;
	}

	template <typename T, typename S>
	const typename boost::container::flat_map<T, S>::const_iterator Random::getAnyOf(const boost::container::flat_map<T, S>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getAnyOf() for an empty boost::container::flat_map", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);

		typename boost::container::flat_map<T, S>::const_iterator iterator = container.begin();
		std::advance(iterator, index);

		return iterator;
	}

	template <typename T, typename S>
	const typename std::map<T, S>::const_iterator Random::getAnyOf(const std::map<T, S>& container, const T& fallbackIfEmpty)
	{
		if (container.empty())
			return fallbackIfEmpty;

		const size_t index = getRandomSize(0, container.size() - 1);

		typename std::map<T, S>::const_iterator iterator = container.begin();
		std::advance(iterator, index);

		return iterator;
	}

	template <typename T, typename S>
	const typename std::unordered_map<T, S>::const_iterator Random::getAnyOf(const std::unordered_map<T, S>& container, const T& fallbackIfEmpty)
	{
		if (container.empty())
			return fallbackIfEmpty;

		const size_t index = getRandomSize(0, container.size() - 1);

		typename std::unordered_map<T, S>::const_iterator iterator = container.begin();
		std::advance(iterator, index);

		return iterator;
	}

	template <typename T, typename S>
	const typename boost::container::flat_map<T, S>::const_iterator Random::getAnyOf(const boost::container::flat_map<T, S>& container, const T& fallbackIfEmpty)
	{
		if (container.empty())
			return fallbackIfEmpty;

		const size_t index = getRandomSize(0, container.size() - 1);

		typename boost::container::flat_map<T, S>::const_iterator iterator = container.begin();
		std::advance(iterator, index);

		return iterator;
	}

	template <typename T>
	T Random::getPickOutAnyOf(std::vector<T>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getPickOutAnyOf() for an empty std::vector", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);
		const T instance = container[index];

		container.erase(container.begin() + index);

		return instance;
	}

	template <typename T>
	T Random::getPickOutAnyOf(std::set<T>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getPickOutAnyOf() for an empty std::set", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);

		typename std::set<T>::const_iterator iterator = container.begin();
		std::advance(iterator, index);
		T instance = *iterator;

		container.erase(iterator);

		return instance;
	}

	template <typename T>
	T Random::getPickOutAnyOf(std::unordered_set<T>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getPickOutAnyOf() for an empty std::unordered_set", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);

		typename std::unordered_set<T>::const_iterator iterator = container.begin();
		std::advance(iterator, index);
		T instance = *iterator;

		container.erase(iterator);

		return instance;
	}

	template <typename T>
	T Random::getPickOutAnyOf(boost::container::flat_set<T>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getPickOutAnyOf() for an empty boost::container::flat_set", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);

		typename boost::container::flat_set<T>::const_iterator iterator = container.begin();
		std::advance(iterator, index);
		T instance = *iterator;

		container.erase(iterator);

		return instance;
	}

	template <typename T, typename S>
	T Random::getPickOutAnyOf(std::map<T, S>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getPickOutAnyOf() for an empty std::map", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);

		typename std::map<T, S>::const_iterator iterator = container.begin();
		std::advance(iterator, index);
		const T instance = iterator->second;

		container.erase(iterator);

		return instance;
	}

	template <typename T, typename S>
	T Random::getPickOutAnyOf(std::unordered_map<T, S>& container)
	{
		QSF_CHECK(!container.empty(), "Called qsf::Random::getPickOutAnyOf() for an empty std::unordered_map", QSF_REACT_THROW);

		const size_t index = getRandomSize(0, container.size() - 1);

		typename std::unordered_map<T, S>::const_iterator iterator = container.begin();
		std::advance(iterator, index);
		const T instance = iterator->second;

		container.erase(iterator);

		return instance;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

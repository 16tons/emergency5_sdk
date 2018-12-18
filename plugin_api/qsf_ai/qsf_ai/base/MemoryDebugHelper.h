// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/container/flat_map.hpp>

#include <cstddef>
#include <vector>
#include <deque>
#include <map>
#include <iosfwd>


// file Contains helper functionality for debugging memory allocation situations
namespace qsf
{
	namespace memory
	{
		// Calculate the allocated amount of memory for a vector containing a POD type. Doesn't work for nested collections and similar stuff
		template <typename T>
		std::size_t calculateMemoryConsumption(const std::vector<T>& v)
		{
			return sizeof(v) + (v.capacity() * sizeof(T));
		}

		// Calculate the allocated amount of memory for a deque containing a POD type. Doesn't work for nested collections and similar stuff
		template <typename T>
		std::size_t calculateMemoryConsumption(const std::deque<T>& v)
		{
			return sizeof(v) + (v.size() * sizeof(T)); // can't use capacity() here although it would be better
		}

		// Calculate the allocated amount of memory for an auto_ptr containing a POD type. Doesn't work for nested collections and similar stuff
		template <typename T>
		std::size_t calculateMemoryConsumption(const std::auto_ptr<T>& p)
		{
			return sizeof(p) + (p.get() ? sizeof(T) : 0);
		}

		// Calculate the allocated amount of memory for a boost flat_map containing only POD key as well as value types. Doesn't work for nested collections and similar stuff
		template <typename Key, typename Value>
		std::size_t calculateMemoryConsumption(const boost::container::flat_map<Key, Value>& map)
		{
			return sizeof(map) + (map.capacity() * sizeof(std::pair<Key, Value>));
		}

		// Calculate the allocated amount of memory for a boost flat_multimap containing only POD key as well as value types. Doesn't work for nested collections and similar stuff
		template <typename Key, typename Value>
		std::size_t calculateMemoryConsumption(const boost::container::flat_multimap<Key, Value>& map)
		{
			return sizeof(map) + (map.capacity() * sizeof(std::pair<Key, Value>));
		}

		// Estimates the allocated amount of memory for a map containing only POD key as well as value types. Doesn't work for nested collections and similar stuff
		template <typename Key, typename Value>
		std::size_t estimateMemoryConsumption(const std::map<Key, Value>& map)
		{
			return sizeof(map) + (map.size() * (sizeof(std::pair<Key, Value>) + (3 + sizeof(void*)))); // the three machine word is a rough assumption of the overhead of elements in the red/black tree
		}

		// Estimates the allocated amount of memory for a multimap containing only POD key as well as value types. Doesn't work for nested collections and similar stuff
		template <typename Key, typename Value>
		std::size_t estimateMemoryConsumption(const std::multimap<Key, Value>& map)
		{
			return sizeof(map) + (map.size() * (sizeof(std::pair<Key, Value>) + (3 + sizeof(void*)))); // the three machine word is a rough assumption of the overhead of elements in the red/black tree
		}

		// Print a readable plain text representation to the output stream of the number of bytes passed which is typically a result of the sizeof call.
		void printReadableSize(std::size_t bytes, std::ostream& outputStream);
	}
}

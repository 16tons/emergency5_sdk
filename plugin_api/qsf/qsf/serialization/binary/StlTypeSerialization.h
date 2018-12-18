// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BasicTypeSerialization.h"

#include <string>
#include <vector>
#include <list>
#include <queue>
#include <set>
#include <map>
#include <unordered_map>
#include <memory>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace serialization
	{
		// This file contains serialization specializations for STL types for use with the QSF binary serialization

		template<>
		struct serializer<std::string>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, std::string& value)
			{
				uint32 length = 0;
				if (serializer.isReading())
				{
					// Read std::string
					serializer & length;
					value.resize(static_cast<std::size_t>(length));
				}
				else
				{
					// Write std::string
					length = static_cast<uint32>(value.length());
					serializer & length;
				}

				serializer.serializeRawBlock(&value[0], length + 1);
			}
		};

		template<typename T, typename Allocator>
		struct serializer<std::vector<T, Allocator>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, std::vector<T, Allocator>& value)
			{
				uint32 size = 0;
				if (serializer.isReading())
				{
					// Read std::vector<T>
					serializer & size;
					value.resize(static_cast<std::size_t>(size));
				}
				else
				{
					// Write std::vector<T>
					size = static_cast<uint32>(value.size());
					serializer & size;
				}

				for (uint32 i = 0; i < size; ++i)
				{
					serializer & static_cast<T&>(value[i]);
				}
			}
		};

		template<typename Allocator>
		struct serializer<std::vector<bool, Allocator>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, std::vector<bool, Allocator>& value)
			{
				uint32 size = 0;
				if (serializer.isReading())
				{
					// Read std::vector<bool>
					serializer & size;
					value.resize(static_cast<std::size_t>(size));

					for (uint32 i = 0; i < size; ++i)
					{
						value[i] = serializer.read<bool>();
					}
				}
				else
				{
					// Write std::vector<bool>
					size = static_cast<uint32>(value.size());
					serializer & size;

					for (uint32 i = 0; i < size; ++i)
					{
						serializer.writeAs<bool>(value[i]);
					}
				}
			}
		};

		template<typename T, typename Allocator>
		struct serializer<std::list<T, Allocator>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, std::list<T, Allocator>& value)
			{
				uint32 size = 0;
				if (serializer.isReading())
				{
					// Read std::list<T>
					value.clear();
					serializer & size;

					for (uint32 i = 0; i < size; ++i)
					{
						T t;
						serializer & t;
						value.push_back(t);
					}
				}
				else
				{
					// Write std::list<T>
					size = static_cast<uint32>(value.size());
					serializer & size;

					for (typename std::list<T>::iterator iterator = value.begin(); iterator != value.end(); ++iterator)
					{
						serializer & static_cast<T&>(*iterator);
					}
				}
			}
		};

		template<typename T, typename Allocator>
		struct serializer<std::deque<T, Allocator>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, std::deque<T, Allocator>& value)
			{
				uint32 size = 0;
				if (serializer.isReading())
				{
					// Read std::deque<T>
					serializer & size;
					value.resize(static_cast<std::size_t>(size));
				}
				else
				{
					// Write std::deque<T>
					size = static_cast<uint32>(value.size());
					serializer & size;
				}

				for (uint32 i = 0; i < size; ++i)
				{
					serializer & static_cast<T&>(value[i]);
				}
			}
		};

		template<typename T, typename U>
		struct serializer<std::pair<T, U>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, std::pair<T, U>& value)
			{
				serializer & value.first;
				serializer & value.second;
			}
		};

		template<typename T, typename Comparator, typename Allocator>
		struct serializer<std::set<T, Comparator, Allocator>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, std::set<T, Comparator, Allocator>& value)
			{
				uint32 size = static_cast<uint32>(value.size());
				serializer & size;
				if (serializer.isReading())
				{
					// Read std::set<T>
					value.clear();

					for (uint32 i = 0; i < size; ++i)
					{
						T t;
						serializer & t;
						value.insert(t);
					}
				}
				else
				{
					// Write std::set<T>
					for (auto i = value.begin(); i != value.end(); ++i)
					{
						serializer & const_cast<T&>(*i); // we need a const cast here - and we know we are in write mode. No sane write implementation should change the parameter
					}
				}
			}
		};

		template<typename T, typename Comparator, typename Allocator>
		struct serializer<std::multiset<T, Comparator, Allocator>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, std::multiset<T, Comparator, Allocator>& value)
			{
				uint32 size = static_cast<uint32>(value.size());
				serializer & size;
				if (serializer.isReading())
				{
					value.clear();

					for (uint32 i = 0; i < size; ++i)
					{
						T t;
						serializer & t;
						value.insert(t);
					}
				}
				else
				{
					for (typename std::multiset<T>::iterator i = value.begin(); i != value.end(); ++i)
					{
						serializer & const_cast<T&>(*i); // we need a const cast here - and we know we are in write mode. No sane write implementation should change the parameter
					}
				}
			}
		};

		template<typename Key, typename Value, typename Comparator, typename Allocator>
		struct serializer<std::map<Key, Value, Comparator, Allocator>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, std::map<Key, Value, Comparator, Allocator>& data)
			{
				uint32 size = static_cast<uint32>(data.size());
				serializer & size;
				if (serializer.isReading())
				{
					data.clear();

					for (uint32 i = 0; i < size; ++i)
					{
						Key k;
						serializer & k;
						Value v;
						serializer & v;
						data.emplace(k, v);
					}
				}
				else
				{
					for (auto i = data.begin(); i != data.end(); ++i)
					{
						serializer & const_cast<Key&>(i->first); // we need a const cast here - and we know we are in write mode. No sane write implementation should change the parameter
						serializer & i->second;
					}
				}
			}
		};

		template<typename Key, typename Value, typename Hasher, typename Comparator, typename Allocator>
		struct serializer<std::unordered_map<Key, Value, Hasher, Comparator, Allocator>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, std::unordered_map<Key, Value, Hasher, Comparator, Allocator>& data)
			{
				uint32 size = static_cast<uint32>(data.size());
				serializer & size;
				if (serializer.isReading())
				{
					data.clear();

					for (uint32 i = 0; i < size; ++i)
					{
						Key k;
						serializer & k;
						Value v;
						serializer & v;
						data.emplace(k, v);
					}
				}
				else
				{
					for (auto i = data.begin(); i != data.end(); ++i)
					{
						serializer & const_cast<Key&>(i->first); // we need a const cast here - and we know we are in write mode. No sane write implementation should change the parameter
						serializer & i->second;
					}
				}
			}
		};

		template<typename Key, typename Value, typename Comparator, typename Allocator>
		struct serializer<std::multimap<Key, Value, Comparator, Allocator>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, std::multimap<Key, Value, Comparator, Allocator>& data)
			{
				uint32 size = static_cast<uint32>(data.size());
				serializer & size;
				if (serializer.isReading())
				{
					data.clear();

					for (uint32 i = 0; i < size; ++i)
					{
						Key k;
						serializer & k;
						Value v;
						serializer & v;
						data.emplace(k, v);
					}
				}
				else
				{
					for (auto i = data.begin(); i != data.end(); ++i)
					{
						Key& key = const_cast<Key&>(i->first); // we need a const cast here - and we know we are in write mode. No sane write implementation should change the parameter
						serializer & key;
						Value& value = i->second;
						serializer & value;
					}
				}
			}
		};

		template<typename T>
		struct serializer<std::auto_ptr<T>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, std::auto_ptr<T>& value)
			{
				if (serializer.isReading())
				{
					bool isInitialized = false;
					serializer & isInitialized;
					if (isInitialized)
					{
						value.reset(new T); // allocate a default created object
						T& data = *value; // extract a reference to the real value contained
						serializer & data; // read data onto the real value
					}
				}
				else
				{
					bool isInitialized = value.get() != nullptr;
					serializer & isInitialized; // serialize a bool first
					if (isInitialized)
						serializer & *value; // serialize the real data
				}
			}
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // serialization
} // qsf

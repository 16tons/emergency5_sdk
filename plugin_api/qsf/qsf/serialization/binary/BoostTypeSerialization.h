// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BasicTypeSerialization.h"

#include <boost/optional.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/flyweight.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace serialization
	{
		// This file contains serialization specializations for boost types for use with the QSF binary serialization
		template<typename T>
		struct serializer<boost::optional<T>>
		{
			static void serialize(qsf::BinarySerializer& serializer, boost::optional<T>& value)
			{
				if (serializer.isReading())
				{
					bool isInitialized = false;
					serializer & isInitialized;
					if (isInitialized)
					{
						T data; // create temporary object
						serializer & data; // read data onto the temporary object
						value = data; // and assign it to the final instance
					}
				}
				else
				{
					bool isInitialized = value.is_initialized();
					serializer & isInitialized; // serialize a bool first
					if (isInitialized)
						serializer & *value; // serialize the real data
				}
			}
		};

		template<typename T, typename LessPredicate, typename Allocator>
		struct serializer<boost::container::flat_set<T, LessPredicate, Allocator>>
		{
			static void serialize(qsf::BinarySerializer& serializer, boost::container::flat_set<T, LessPredicate, Allocator>& value)
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
					for (auto i = value.begin(); i != value.end(); ++i)
					{
						serializer & const_cast<T&>(*i); // we need a const cast here - and we know we are in write mode. No sane write implementation should change the parameter
					}
				}
			}
		};

		template<typename T, typename LessPredicate, typename Allocator>
		struct serializer<boost::container::flat_multiset<T, LessPredicate, Allocator>>
		{
			static void serialize(qsf::BinarySerializer& serializer, boost::container::flat_multiset<T, LessPredicate, Allocator>& value)
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
					for (auto i = value.begin(); i != value.end(); ++i)
					{
						serializer & const_cast<T&>(*i); // we need a const cast here - and we know we are in write mode. No sane write implementation should change the parameter
					}
				}
			}
		};

		template<typename Key, typename Value, typename LessPredicate, typename Allocator>
		struct serializer<boost::container::flat_map<Key, Value, LessPredicate, Allocator>>
		{
			static void serialize(qsf::BinarySerializer& serializer, boost::container::flat_map<Key, Value, LessPredicate, Allocator>& data)
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

		template<typename Key, typename Value, typename LessPredicate, typename Allocator>
		struct serializer<boost::container::flat_multimap<Key, Value, LessPredicate, Allocator>>
		{
			static void serialize(qsf::BinarySerializer& serializer, boost::container::flat_multimap<Key, Value, LessPredicate, Allocator>& data)
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

		template<typename Block, typename Allocator>
		struct serializer<boost::dynamic_bitset<Block, Allocator>>
		{
			static void serialize(qsf::BinarySerializer& serializer, boost::dynamic_bitset<Block, Allocator>& data)
			{
				uint32 totalBits = static_cast<uint32>(data.size());
				serializer & totalBits;

				if (serializer.isReading())
				{
					data.resize(totalBits);
				}

				const uint32 bytes = (totalBits + 7) / 8;
				uint8 flags;

				for (uint32 byte = 0; byte < bytes; ++byte)
				{
					const uint32 bits = std::min<uint32>(totalBits - byte * 8, 8);

					if (serializer.isReading())
					{
						serializer & flags;

						for (uint32 bit = 0; bit < bits; ++bit)
						{
							data[byte * 8 + bit] = ((flags & (1 << bit)) != 0);
						}
					}
					else
					{
						flags = 0;

						for (uint32 bit = 0; bit < bits; ++bit)
						{
							if (data[byte * 8 + bit])
							{
								flags |= (1 << bit);
							}
						}

						serializer & flags;
					}
				}
			}
		};

		template<typename T, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
		struct serializer<boost::flyweight<T, Arg1, Arg2, Arg3, Arg4, Arg5>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, boost::flyweight<T, Arg1, Arg2, Arg3, Arg4, Arg5>& data)
			{
				serializer.serializeAs<T>(data);
			}
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // serialization
} // qsf

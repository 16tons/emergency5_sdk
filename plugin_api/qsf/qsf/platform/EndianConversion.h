// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace endianconversion
	{


		inline static uint16 reverseByteOrder(uint16 u16)
		{
			return ((u16 << 8) & 0xff00) | ((u16 >> 8) & 0x00ff);
		}

		inline static uint32 reverseByteOrder(uint32 u32)
		{
			return ((u32 << 24) & 0xff000000U) | ((u32 << 8) & 0x00ff0000U) | ((u32 >> 8) & 0x0000ff00U) | ((u32 >> 24) & 0x000000ffU);
		}

		inline static uint64 reverseByteOrder(uint64 u64)
		{
			u64 = ((u64 >>  8) & 0x00ff00ff00ff00ffULL) | ((u64 <<  8) & 0xff00ff00ff00ff00ULL);
			u64 = ((u64 >> 16) & 0x0000ffff0000ffffULL) | ((u64 << 16) & 0xffff0000ffff0000ULL);
			return ((u64 >> 32) & 0x00000000ffffffffULL) | ((u64 << 32) & 0xffffffff00000000ULL);
		}

		template<typename T, size_t N = sizeof(T)>
		struct ByteOrder
		{
			inline static void Reverse(T& value)
			{
#ifdef _MSC_VER
				bad_type_specified
#endif
			}
		};

		template<typename T>
		struct ByteOrder<T, 1>
		{
			inline static void Reverse(T& value)
			{
				// Nothing to do here
			}
		};

		template<typename T>
		struct ByteOrder<T, 2>
		{
			inline static void Reverse(T& value)
			{
				const uint16 result = reverseByteOrder(*reinterpret_cast<uint16*>(&value));
				value = *reinterpret_cast<const T*>(&result);
			}
		};

		template<typename T>
		struct ByteOrder<T, 4>
		{
			inline static void Reverse(T& value)
			{
				const uint32 result = reverseByteOrder(*reinterpret_cast<uint32*>(&value));
				value = *reinterpret_cast<const T*>(&result);
			}
		};

		template<typename T>
		struct ByteOrder<T, 8>
		{
			inline static void Reverse(T& value)
			{
				const uint64 result = reverseByteOrder(*reinterpret_cast<uint64*>(&value));
				value = *reinterpret_cast<const T*>(&result);
			}
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	}
}

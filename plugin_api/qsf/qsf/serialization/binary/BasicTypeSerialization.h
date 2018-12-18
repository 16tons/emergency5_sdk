// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BinarySerializer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace serialization
	{


		//[-------------------------------------------------------]
		//[ Serialization of basic types                          ]
		//[-------------------------------------------------------]
		template<>
		struct serializer<char>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, char& value)
			{
				serializer.serializeRawBlock(&value, 1);
			}
		};

		template<>
		struct serializer<int8>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, int8& value)
			{
				serializer.serializeRawBlock(&value, 1);
			}
		};

		template<>
		struct serializer<uint8>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, uint8& value)
			{
				serializer.serializeRawBlock(&value, 1);
			}
		};

		template<>
		struct serializer<int16>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, int16& value)
			{
				serializer.serializeRawBlock(&value, 2, true);
			}
		};

		template<>
		struct serializer<uint16>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, uint16& value)
			{
				serializer.serializeRawBlock(&value, 2, true);
			}
		};

		template<>
		struct serializer<int32>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, int32& value)
			{
				serializer.serializeRawBlock(&value, 4, true);
			}
		};

		template<>
		struct serializer<uint32>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, uint32& value)
			{
				serializer.serializeRawBlock(&value, 4, true);
			}
		};

		template<>
		struct serializer<int64>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, int64& value)
			{
				serializer.serializeRawBlock(&value, 8, true);
			}
		};

		template<>
		struct serializer<uint64>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, uint64& value)
			{
				serializer.serializeRawBlock(&value, 8, true);
			}
		};

		template<>
		struct serializer<float>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, float& value)
			{
				serializer.serializeRawBlock(&value, 4);
			}
		};

		template<>
		struct serializer<double>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, double& value)
			{
				serializer.serializeRawBlock(&value, 8);
			}
		};

		template<>
		struct serializer <bool>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, bool& value)
			{
				if (serializer.isReading())
				{
					// Read value
					uint8 targetTypeValue;
					serializer.serialize(targetTypeValue);
					value = (targetTypeValue != 0);
				}
				else
				{
					// Write value
					uint8 targetTypeValue = (value ? 1 : 0);
					serializer.serialize(targetTypeValue);
				}
			}
		};


		//[-------------------------------------------------------]
		//[ Tokens of basic types                                 ]
		//[-------------------------------------------------------]
		template<>
		inline uint32 getToken<uint8>()
		{
			return *(uint32*)("__u8");
		}

		template<>
		inline uint32 getToken<int8>()
		{
			return *(uint32*)("__i8");
		}

		template<>
		inline uint32 getToken<uint16>()
		{
			return *(uint32*)("_u16");
		}

		template<>
		inline uint32 getToken<int16>()
		{
			return *(uint32*)("_i16");
		}

		template<>
		inline uint32 getToken<uint32>()
		{
			return *(uint32*)("_u32");
		}

		template<>
		inline uint32 getToken<int32>()
		{
			return *(uint32*)("_i32");
		}

		template<>
		inline uint32 getToken<uint64>()
		{
			return *(uint32*)("_u64");
		}

		template<>
		inline uint32 getToken<int64>()
		{
			return *(uint32*)("_i64");
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // serialization
} // qsf

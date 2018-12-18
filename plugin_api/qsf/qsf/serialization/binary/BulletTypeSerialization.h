// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BasicTypeSerialization.h"

#include <LinearMath/btVector3.h>
#include <LinearMath/btQuaternion.h>
#include <LinearMath/btMatrix3x3.h>
#include <LinearMath/btTransform.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace serialization
	{
		// This file contains serialization specializations for Bullet types for use with the QSF binary serialization

		template<>
		struct serializer<btVector3>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, btVector3& value)
			{
				// SSE based type (__m128), therefor 4 elements
				serializer.serialize(value.m_floats[0]);
				serializer.serialize(value.m_floats[1]);
				serializer.serialize(value.m_floats[2]);
				serializer.serialize(value.m_floats[3]);
			}
		};

		template<>
		struct serializer<btQuaternion>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, btQuaternion& value)
			{
				if (serializer.isReading())
				{
					float xyzw[4];

					serializer.serialize(xyzw[0]);
					serializer.serialize(xyzw[1]);
					serializer.serialize(xyzw[2]);
					serializer.serialize(xyzw[3]);

					value.setX(xyzw[0]);
					value.setY(xyzw[1]);
					value.setZ(xyzw[2]);
					value.setW(xyzw[3]);
				}
				else
				{
					serializer.serialize((float)value.x());
					serializer.serialize((float)value.y());
					serializer.serialize((float)value.z());
					serializer.serialize((float)value.w());
				}
			}
		};

		template<>
		struct serializer<btMatrix3x3>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, btMatrix3x3& value)
			{
				serializer.serialize(value[0]); // btVector3
				serializer.serialize(value[1]); // btVector3
				serializer.serialize(value[2]); // btVector3
			}
		};

		template<>
		struct serializer<btTransform>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, btTransform& value)
			{
				if (serializer.isReading())
				{
					btVector3 origin;
					btMatrix3x3 basis;

					serializer.serialize(origin);
					serializer.serialize(basis);

					value.setOrigin(origin);
					value.setBasis(basis);
				}
				else
				{
					serializer.serialize(value.getOrigin());
					serializer.serialize(value.getBasis());
				}
			}
		};

		template<typename T>
		struct serializer<btAlignedObjectArray<T>>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, btAlignedObjectArray<T>& value)
			{
				uint32 size = 0;
				if (serializer.isReading())
				{
					// Read btAlignedObjectArray<T>
					serializer & size;
					value.resize(static_cast<std::size_t>(size));
				}
				else
				{
					// Write btAlignedObjectArray<T>
					size = static_cast<uint32>(value.size());
					serializer & size;
				}

				for (uint32 i = 0; i < size; ++i)
				{
					serializer & static_cast<T&>(value[i]);
				}
			}
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // serialization
} // qsf

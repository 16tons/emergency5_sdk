// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/serialization/binary/BasicTypeSerialization.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace serialization
	{
		// This file contains serialization specializations for GLM types for use with the QSF binary serialization

		template<>
		struct serializer<glm::vec2>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, glm::vec2& value)
			{
				serializer.serialize(value.x);
				serializer.serialize(value.y);
			}
		};

		template<>
		struct serializer<glm::vec3>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, glm::vec3& value)
			{
				serializer.serialize(value.x);
				serializer.serialize(value.y);
				serializer.serialize(value.z);
			}
		};

		template<>
		struct serializer<glm::vec4>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, glm::vec4& value)
			{
				serializer.serialize(value.x);
				serializer.serialize(value.y);
				serializer.serialize(value.z);
				serializer.serialize(value.w);
			}
		};

		template<>
		struct serializer<glm::mat2>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, glm::mat2& value)
			{
				serializer.serialize(value[0]);
				serializer.serialize(value[1]);
			}
		};

		template<>
		struct serializer<glm::mat3>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, glm::mat3& value)
			{
				serializer.serialize(value[0]);
				serializer.serialize(value[1]);
				serializer.serialize(value[2]);
			}
		};

		template<>
		struct serializer<glm::mat4>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, glm::mat4& value)
			{
				serializer.serialize(value[0]);
				serializer.serialize(value[1]);
				serializer.serialize(value[2]);
				serializer.serialize(value[3]);
			}
		};

		template<>
		struct serializer<glm::quat>
		{
			inline static void serialize(qsf::BinarySerializer& serializer, glm::quat& value)
			{
				serializer.serialize(value.w);
				serializer.serialize(value.x);
				serializer.serialize(value.y);
				serializer.serialize(value.z);
			}
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // serialization
} // qsf

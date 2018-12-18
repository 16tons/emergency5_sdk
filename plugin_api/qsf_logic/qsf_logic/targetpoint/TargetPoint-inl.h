// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BasicTypeSerialization.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>
#include <qsf/serialization/binary/BoostTypeSerialization.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		inline TargetPoint::TargetPoint(const glm::vec3& position, bool dynamicTarget, float distance, float tolerance, const boost::optional<glm::quat>& orientation) :
			mPosition(position),
			mOrientation(orientation),
			mDistance(distance),
			mTolerance(tolerance),
			mDynamic(dynamicTarget)
		{}

		inline TargetPoint::TargetPoint() :
			mDistance(0.0f),
			mTolerance(0.1f),
			mDynamic(false)
		{}


	} // logic


	namespace serialization
	{
		// specialization for the binary serialization
		template <>
		struct serializer<logic::TargetPoint>
		{
			inline static void serialize(BinarySerializer& serializer, logic::TargetPoint& data)
			{
				serializer & data.mPosition;
				serializer & data.mOrientation;
				serializer & data.mDistance;
				serializer & data.mTolerance;
				serializer & data.mDynamic;
			}
		};
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/BitMaskHelper.h"

#include <qsf/math/Math.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>

namespace qsf
{
	namespace ai
	{
		inline Waypoint::Waypoint(const glm::vec3& pos, const glm::vec3& direction, const DynamicPortal&  portal, bool onPrimaryMap, bool movingForwards,
			unsigned int worldElementId, unsigned int nodeId, const boost::optional<TurningConfiguration>& turningConfig) :
			mPosition(pos),
			mDirection(direction),
			mPortal(portal),
			mWorldElementId(worldElementId),
			mNodeId(nodeId),
			mFlags(0),
			mTurningConfiguration(turningConfig)
		{
			// combine flags
			if (onPrimaryMap)
				bitmasking::addTo(mFlags, IS_ON_PRIMARY_MAP);
			if (movingForwards)
				bitmasking::addTo(mFlags, IS_MOVING_FORWARD);
		}

		inline Waypoint::Waypoint() :
		mWorldElementId(getUninitialized(mWorldElementId)),
		mNodeId(getUninitialized(mNodeId)),
		mFlags(IS_ON_PRIMARY_MAP | IS_MOVING_FORWARD)
		{}

		inline bool Waypoint::hasWorldElementId() const
		{
			return isInitialized(mWorldElementId);
		}

		inline bool Waypoint::hasNodeId() const
		{
			return isInitialized(mNodeId);
		}

		inline bool Waypoint::hasOrientation() const
		{
			return mDirection != Math::GLM_VEC3_ZERO;
		}

		inline bool Waypoint::isOnPrimaryMap() const
		{
			return bitmasking::holdsIn(mFlags, IS_ON_PRIMARY_MAP);
		}

		inline bool Waypoint::isMovingForward() const
		{
			return bitmasking::holdsIn(mFlags, IS_MOVING_FORWARD);
		}

		inline bool Waypoint::isEvasionWaypoint() const
		{
			return bitmasking::holdsIn(mFlags, IS_EVASION_WAYPOINT);
		}

		inline bool Waypoint::isLocalRouterResult() const
		{
			return bitmasking::holdsIn(mFlags, IS_CALCULATED_BY_LOCAL_ROUTER);
		}

		inline void Waypoint::makeEvasionWaypoint()
		{
			bitmasking::addTo(mFlags, IS_EVASION_WAYPOINT);
		}

		inline void Waypoint::makeLocalRouterResult()
		{
			bitmasking::addTo(mFlags, IS_CALCULATED_BY_LOCAL_ROUTER);
		}
	}

	namespace serialization
	{
		// specialization for use with the QSF binary serializer
		template <>
		struct serializer<ai::Waypoint>
		{
			inline static void serialize(BinarySerializer& serializer, ai::Waypoint& waypoint)
			{
				serializer & waypoint.mPosition;
				serializer & waypoint.mDirection;
				serializer & waypoint.mPortal;
				serializer & waypoint.mWorldElementId;
				serializer & waypoint.mNodeId;
				serializer & waypoint.mFlags;
			}
		};
	}
}

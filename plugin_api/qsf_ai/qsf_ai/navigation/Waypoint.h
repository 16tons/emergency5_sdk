// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/serialization/QsfBinarySerializationFwd.h"
#include "qsf_ai/navigation/TurningConfiguration.h"
#include "qsf_ai/navigation/DynamicPortal.h"

#include <qsf/base/GetUninitialized.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>

#include <glm/glm.hpp>

#include <boost/optional.hpp>

#include <utility>

namespace qsf
{
	namespace ai
	{
		/** A waypoint contains information about a position that can be approached during navigation.
		* This is the exact coordinates as well as the world element and node indices that can be used
		* to retrieve further information like the terrain or whether the goal area is currently blocked for example.
		*
		* Simple class that is copyable via the default mechanisms.
		*/
		class QSF_AI_API_EXPORT Waypoint
		{
		public:
			// Some flags specific to a waypoint. These can be combined into a bitmask of these options
			enum Flags
			{
				IS_ON_PRIMARY_MAP = 1 << 0, // whether the waypoint is on the primary or the secondary navigation map of the entity
				IS_MOVING_FORWARD = 1 << 1, // whether the entity is moving forward or backward when approaching this waypoint. We only support these two extremes currently
				IS_EVASION_WAYPOINT = 1 << 2, // whether the waypoint was dynamically added to evade some entity
				IS_CALCULATED_BY_LOCAL_ROUTER = 1 << 3, // whether the waypoint is from a converted em4 router path result
			};

			// friends are necessary for private access for the binary serialization
			friend struct serialization::serializer<Waypoint>;
			friend class std::allocator<Waypoint>;

			Waypoint(const glm::vec3& pos, const glm::vec3& direction, const DynamicPortal& portal, bool onPrimaryMap, bool movingForwards,
				unsigned int worldElementId = getUninitialized<unsigned int>(), unsigned int nodeId = getUninitialized<unsigned int>(),
				const boost::optional<TurningConfiguration>& turningConfig = boost::optional<TurningConfiguration>());

			// Returns whether the optional data fields world element, node id or orientation were set or kept at the uninitialized default.
			//@{
			bool hasWorldElementId() const;
			bool hasNodeId() const;
			bool hasOrientation() const;
			//@}

			// Access to the flags setting
			//@{
			bool isOnPrimaryMap() const;
			bool isMovingForward() const;
			bool isEvasionWaypoint() const;
			bool isLocalRouterResult() const;
			//@}

			// Setters for the flags
			//@{
			void makeEvasionWaypoint();
			void makeLocalRouterResult();
			//@}

			glm::vec3 mPosition;
			glm::vec3 mDirection; // A direction to be facing when reaching this waypoint. ZERO means not used
			DynamicPortal mPortal; // The portal to cross where the exact position lies on. Only needed if the path is adapted dynamically
			unsigned int mWorldElementId; // uninitialized means no information about world elements is contained
			unsigned int mNodeId; // uninitialized means no information about a node is contained
			unsigned int mFlags; // A binary combination of the flags enum values signaling which specific options are valid for this waypoint
			boost::optional<TurningConfiguration> mTurningConfiguration; // Configuration for the curve if the entity takes a turn to move here, not set if the entity is moving in a straight line

		private:
			Waypoint(); // hidden constructor only used for the serialization
		};
	}
}

#include "qsf_ai/navigation/Waypoint-inl.h"

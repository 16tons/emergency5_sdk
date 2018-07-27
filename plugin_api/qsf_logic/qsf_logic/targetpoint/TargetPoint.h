// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/Export.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <boost/optional.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    A target point is a spatial configuration that allows an interaction between two entities.
		*    Since a target point is relative to the interacting as well as the interacted on entity it can be dynamic and is often invalidated as soon as one of the entities moves.
		*    Simple class that is copyable via the default mechanisms and public members because it is only a tiny data collection.
		*/
		class QSF_LOGIC_API_EXPORT TargetPoint
		{


		public:
			// Default constructor is necessary for serialization.
			// Initializes a move towards 0,0,0 with distance 0 and tolerance 0.1 as static target.
			TargetPoint();

			// Standard constructor providing all values.
			TargetPoint(const glm::vec3& position, bool dynamicTarget, float distance, float tolerance, const boost::optional<glm::quat>& orientation = boost::optional<glm::quat>());

			// Position in global coordinates.
			glm::vec3 mPosition;

			// An optional orientation to arrive at in the target position.
			// This changes its semantics depending on whether we have a zero or nonzero distance (see below).
			// With a zero distance the orientation is in world coordinates as we have an absolute position.
			// A set distance means we don't know how we approach the target point and the orientation is in a local coordinate frame with the entity in the origin and the
			// target point is on the 'in' axis.
			// For the typical case of facing towards the target point use an orientation pointing towards the 'in' axis.
			boost::optional<glm::quat> mOrientation;

			// An optional maximum distance to keep to the position.
			// A distance of zero means that we should arrive exactly at the position.
			// The entity will try to reach the distance if it starts farther away but it will not move away if it is already closer to the target.
			// Be aware that there is a big conceptual difference whether this value is zero or greater than zero.
			// A zero distance target point is actually one exact location while a distance means there is an area around a point with legal solutions.
			// This changes the semantics of the optional orientation parameter (see above).
			// TODO(vs) Should be unsigned float as negative distance makes no sense
			float mDistance;

			// An additional tolerance to apply when deciding whether the target can be considered reached.
			// A possible ideal distance to keep towards the target is already contained in the position provided.
			// This tolerance is only a delta fallback that should be considered when the position can not be achieved.
			// TODO(vs) Should be unsigned float as negative tolerance makes no sense
			float mTolerance;

			// Flag to signal whether the target should be considered as dynamic target.
			// A target point that is dynamic can be invalidated at any time.
			// It is up to the client systems using target points to decide how often to check.
			bool mDynamic;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_logic/targetpoint/TargetPoint-inl.h"

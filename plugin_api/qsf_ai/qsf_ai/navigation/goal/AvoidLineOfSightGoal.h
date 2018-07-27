// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/goal/NavigationGoal.h"
#include "qsf_ai/base/UnsignedFloat.h"

#include <qsf/serialization/binary/GlmTypeSerialization.h>

#include <glm/glm.hpp>

namespace qsf
{
	class Entity;

	namespace ai
	{
		/**
		* Navigation goal to avoid a line of sight towards a given entity and a target point.
		* The range data is read from the target point.
		* TODO(vs) nonfunctional dummy because we don't have the algorithm for negative goal conditions set up
		*/
		class QSF_AI_API_EXPORT AvoidLineOfSightGoal : public NavigationGoal
		{
		public:
			friend class NavigationGoalFactory; // for using the private default constructor

			// The collision flags are used to determine which kind of obstacles block the line of sight.
			// See the bullet collision flags in qsf::BulletCollisionComponent for details.
			AvoidLineOfSightGoal(const Entity& target, uint32 targetPoint, short traceLineCollisionFlags);

			// navigation goal interface implementation
			//@{
			virtual process::State evaluateState(const Entity& entity) override;
			virtual bool checkForChangedGoalSituation(const Entity& entity) override;
			virtual bool isTargetMovingCloser() const override;
			virtual UnsignedFloat estimateDistanceToTarget(const glm::vec3& fromPos) const override;
			virtual std::auto_ptr<NavigationGoal> clone() const override;
			//@}
		protected:
			virtual void serializeSpecificData(BinarySerializer& serializer) override;

		private:
			AvoidLineOfSightGoal(); // hidden so it can be only used by friends

			uint64 mTargetObjectId;
			uint32 mTargetPointId;
			UnsignedFloat mMaxRange;
			short mTraceLineCollisionFlags;
			glm::vec3 mAvoidedPosition; // the position to avoid having line of sight to
		};
	}
}

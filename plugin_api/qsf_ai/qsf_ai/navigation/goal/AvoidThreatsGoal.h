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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		* Navigation goal to reach an area where the entity doesn't collide with threatening objects anymore.
		* Special purpose use for collisions that are no physical obstacles like a proxy object for the fire influence.
		* TODO(vs) nonfunctional dummy because we don't have the algorithm for negative goal conditions set up
		*/
		class QSF_AI_API_EXPORT AvoidThreatsGoal : public NavigationGoal
		{
		public:
			friend class NavigationGoalFactory; // for using the private default constructor

			// The collision filter is used to determine which kind of obstacles to avoid.
			// See the bullet collision flags in qsf::BulletCollisionComponent for details.
			AvoidThreatsGoal(UnsignedFloat desiredDistance, short avoidedObjectsFilter);

			// Navigation goal interface implementation
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
			AvoidThreatsGoal(); // hidden so it can be only used by friends

			UnsignedFloat mDesiredDistance;
			short mAvoidedCollisionsFilter;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

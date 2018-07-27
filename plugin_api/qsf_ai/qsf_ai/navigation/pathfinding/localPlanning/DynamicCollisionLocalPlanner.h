// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/SpatialConfiguration.h"

#include <qsf/component/Component.h>
#include <qsf/reflection/CampDefines.h>
#include <qsf/job/JobProxy.h>


namespace qsf
{
	namespace ai
	{
		class NavigationComponent;
		class ERouteFinder;

		/**
		* The dynamic collision local planner provides local planning around registered dynamic obstacles.
		* It is used as a singleton component that is attached to the core entity.
		* It is a thin QSF wrapper around the em4 route finder technology.
		*/
		class DynamicCollisionLocalPlanner : public Component
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			DynamicCollisionLocalPlanner(Prototype* prototype);
			~DynamicCollisionLocalPlanner();

			// pseudo singleton access via the core entity
			static DynamicCollisionLocalPlanner& getInstance();

			// Access the wrapped router object
			//@{
			ERouteFinder& getRouteFinder();
			const ERouteFinder& getRouteFinder() const;
			//@}

			// Creates a request to plan a local path around a collision encountered in distanceToClosestCollision units away that should start in the configuration plannedAdaptationBegin
			void requestPathAdaptation(NavigationComponent& navi, const SpatialConfiguration3D& plannedAdaptationBegin, float distanceToClosestCollision, bool usePathNodesAsTarget = true);


		protected:
			// Overridden protected Component interface
			//@{
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onSetActive(bool active) override;
			//@}


		private:
			bool registerUpdateJob();
			// Main update function that is passed to the job manager
			void updateJob(const JobArguments& jobArguments);

			JobProxy mUpdateJobProxy;

			QSF_CAMP_RTTI();
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::DynamicCollisionLocalPlanner);

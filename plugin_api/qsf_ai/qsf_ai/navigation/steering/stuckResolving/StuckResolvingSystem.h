// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/StandardSystem.h"
#include "qsf_ai/navigation/steering/stuckResolving/StuckResolverComponent.h"


namespace qsf
{
	namespace ai
	{
		class NavigationDebugGroup;

		class QSF_AI_API_EXPORT StuckResolvingSystem : public StandardSystem<StuckResolverComponent, StuckResolvingSystem>
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			// Plain text identifier of the system
			static const char* NAME;
			// Only try to reach the goal via a completely different route if we ended up at least this distance away from the goal
			static const ExportedProperty<UnsignedFloat> MIN_DISTANCE_TO_REPLAN;

			StuckResolvingSystem(Prototype* prototype);

			// Public template interface requirement of qsf::ai::StandardSystem
			//@{
			bool onStartup() override;
			effort::Indicator updateComponent(StuckResolverComponent& resolver, const JobArguments& arguments);
			void createDebugOutput(const StuckResolverComponent& resolver) const;
			void updateGlobals(const JobArguments&);
			//@}

			UnsignedFloat getMinDistanceToReplan() const;
			void setMinDistanceToReplan(UnsignedFloat distance);

		private:
			// Actually handle a stuck situation between two entities
			effort::Indicator onStuckSituationDetected(StuckResolverComponent& stuck, stuckResolve::Analysis analysis, Time now);

			// Called when we found the entity to be not in a stuck situation
			effort::Indicator onNoStuckSituationDetected(StuckResolverComponent& resolver, stuckResolve::Analysis analysis, Time now);

			// check whether there is a cyclic stuck situation involving the two entities passed.
			// Writes the ids of all entities that were detected to be stuck in the cycle to the reference parameter
			bool checkForCyclicStuckSituation(NavigationComponent& naviA, NavigationComponent& naviB, std::set<NavigationComponent*>& waitingEntities) const;

			// Just store the cyclic stuck participants
			void onCyclicStuckSituationDetected(std::set<NavigationComponent*>& stuckEntities);

			// Debug helper to draw a circle around an entity
			void debugDrawEntityPosition(const StuckResolverComponent& resolver, stuckResolve::Stage stage) const;


			// Private data
			// Optional pointer to the navigation debug group
			NavigationDebugGroup* mDebugGroup;

			// If we detect stuck situations, we actually detect stuck entities out of their regular order.
			// We store the addresses here to avoid overhead in calculating it twice
			// for technical reasons we collect the navigation and not the stuck resolving components here
			// This is volatile data only valid through the current update and only may save some performance. Therefore it is not serialized
			std::set<NavigationComponent*> mCyclicStuckParticipants;

			UnsignedFloat mMinDistanceToReplan;
			QSF_CAMP_RTTI();
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::StuckResolvingSystem);

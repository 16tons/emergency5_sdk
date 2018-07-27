// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/StandardSystem.h"
#include "qsf_ai/worldModel/trafficControl/DynamicNavigationBlockerComponent.h"

#include <qsf/reflection/CampDefines.h>

namespace qsf
{
	namespace ai
	{
		/**
		* A system component that updates all the dynamic navigation blocker components.
		* Like all system components it is attached to the core entity and there should be only one instance in the simulation.
		*/
		class QSF_AI_API_EXPORT TrafficControlSystem : public StandardSystem<DynamicNavigationBlockerComponent, TrafficControlSystem>
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			// Plain text identifier
			static const char* NAME;

			TrafficControlSystem(Prototype* prototype);

			// Required ai::StandardSystem template interface implementation
			//@{
			effort::Indicator updateComponent(DynamicNavigationBlockerComponent& dnbc, const JobArguments& arguments) const;
			void createDebugOutput(const DynamicNavigationBlockerComponent& dnbc) const;
			void updateGlobals(const JobArguments&) const;
			//@}

			QSF_CAMP_RTTI();
		};


		// inline implementation
		inline TrafficControlSystem::TrafficControlSystem(Prototype* prototype) :
			StandardSystem<DynamicNavigationBlockerComponent, TrafficControlSystem>(prototype, NAME)
		{}

		inline void TrafficControlSystem::updateGlobals(const JobArguments&) const
		{}

		inline void TrafficControlSystem::createDebugOutput(const DynamicNavigationBlockerComponent&) const
		{}
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::TrafficControlSystem);

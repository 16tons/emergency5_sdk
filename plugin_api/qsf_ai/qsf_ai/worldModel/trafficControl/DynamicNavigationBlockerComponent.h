// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/worldModel/WorldElementState.h"

#include <qsf/component/Component.h>
#include <qsf/reflection/CampDefines.h>
#include <qsf/logic/EffortIndicator.h>
#include <qsf/time/Time.h>

namespace qsf
{
	namespace ai
	{
		/**
		* Abstract super class for components that influence the state of AI navigation maps at runtime.
		*/
		class QSF_AI_API_EXPORT DynamicNavigationBlockerComponent : public Component
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			DynamicNavigationBlockerComponent(Prototype* prototyp);

			// Main function that dynamically updates the navigation map state.
			// Returns the effort needed to update the managed state.
			virtual effort::Indicator update(const Time& now) = 0;

			// This tries to access a NavigationElementConnector component attached to the same entity and set the state through its interface
			void setElementState(const worldElement::State& state) const;

		private:
			QSF_CAMP_RTTI();
		};


		// inline implementation
		inline DynamicNavigationBlockerComponent::DynamicNavigationBlockerComponent(Prototype* prototype) :
		Component(prototype)
		{}
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::DynamicNavigationBlockerComponent);

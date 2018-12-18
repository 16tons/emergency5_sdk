// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/worldModel/trafficControl/DynamicNavigationBlockerComponent.h"
#include "qsf_ai/worldModel/trafficControl/TrafficLightPhaseSchedule.h"

#include <qsf/reflection/CampDefines.h>
#include <qsf/reflection/CampClass.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>
#include <qsf/logic/EffortIndicator.h>

#include <boost/optional.hpp>

#include <string>

namespace qsf
{
	class Time;

	namespace ai
	{
		class TrafficLightComponent;

		/**
		* A traffic light timer component manages a group of traffic light components.
		* It uses a TrafficLightPhaseSchedule to manage which signals are shown at what traffic lights at each point in time.
		* It is typically an individual, abstract entity added to each crossing or in general area that has synchronized traffic lights.
		* The schedule is read from a asset whose global asset ID needs to be set before the component is started.
		*/
		class QSF_AI_API_EXPORT TrafficLightTimerComponent : public DynamicNavigationBlockerComponent
		{
		public:
			// Unique CAMP IDs and default values for exported properties.
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			static const unsigned int ASSET_PROPERTY_ID;
			static const std::string DEFAULT_SCHEDULE_ASSET;
			//@}

			TrafficLightTimerComponent(Prototype* prototype);

			// overridden public component interface
			virtual void serialize(BinarySerializer& serializer) override;

			// Apply the current schedule state for the current time.
			virtual effort::Indicator update(const Time& now) override;

			// Access to the asset property.
			//@{
			const AssetProxy& getAsset() const;
			void setAsset(const AssetProxy& asset);
			//@}

		protected:
			// Component interface override
			virtual void onSetActive(bool active) override;
			virtual bool onStartup() override;
			virtual void onSetSimulating(bool simulating) override;
		private:
			void collectAllManagedComponents();

			// The schedule to be executed
			TrafficLightPhaseSchedule mSchedule;
			// Optional pointer to the current phase.
			// May be null before the first update or with an empty schedule for example.
			const TrafficLightPhaseConfiguration* mCurrentPhase;

			// Asset containing the serialized schedule.
			AssetProxy mScheduleAsset;

			// Ids of directly linked entities - this vector is exported to CAMP and tweaked via the linked mechanics.
			std::vector<uint64> mLinkedTrafficLights;
			// Contains the pointers to all components that are recursively read from the linked entities
			std::vector<TrafficLightComponent*> mManagedComponents;

		public:
			// Public array variable exported to CAMP
			QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(ManagedTrafficLightList, TrafficLightTimerComponent, std::vector<uint64>, uint64);
			ManagedTrafficLightList ManagedTrafficLights;

			QSF_CAMP_RTTI();
		};
	}
}

#include "qsf_ai/worldModel/trafficControl/TrafficLightTimerComponent-inl.h"

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::TrafficLightTimerComponent);
QSF_CAMP_ARRAY_MAPPER_DEFINITION(qsf::ai::TrafficLightTimerComponent::ManagedTrafficLightList);

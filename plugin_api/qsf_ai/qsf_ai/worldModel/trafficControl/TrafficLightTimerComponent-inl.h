// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/reflection/CampDefines.h"

#include <camp/arraymapper.hpp>

namespace qsf
{
	namespace ai
	{
		inline TrafficLightTimerComponent::TrafficLightTimerComponent(Prototype* prototype) :
			DynamicNavigationBlockerComponent(prototype),
			mCurrentPhase(nullptr),
			ManagedTrafficLights(*this, mLinkedTrafficLights)
		{}

		// Implementation of internal class that is needed for the export to CAMP
		inline const AssetProxy& TrafficLightTimerComponent::getAsset() const
		{
			return mScheduleAsset;
		}

		inline std::size_t TrafficLightTimerComponent::ManagedTrafficLightList::size() const
		{
			return getVariable().size();
		}

		inline const TrafficLightTimerComponent::ManagedTrafficLightList::ElementType& TrafficLightTimerComponent::ManagedTrafficLightList::get(std::size_t index) const
		{
			return getVariable()[index];
		}

		inline void TrafficLightTimerComponent::ManagedTrafficLightList::set(std::size_t index, const TrafficLightTimerComponent::ManagedTrafficLightList::ElementType& value)
		{
			getVariable()[index] = value;
			TrafficLightTimerComponent& owner = getOwner();
			if (owner.isRunning())
				getOwner().collectAllManagedComponents();
		}

		inline void TrafficLightTimerComponent::ManagedTrafficLightList::insert(std::size_t before, const TrafficLightTimerComponent::ManagedTrafficLightList::ElementType& value)
		{
			getVariable().insert(getVariable().begin() + before, value);
			TrafficLightTimerComponent& owner = getOwner();
			if (owner.isRunning())
				owner.collectAllManagedComponents();
		}

		inline void TrafficLightTimerComponent::ManagedTrafficLightList::remove(std::size_t index)
		{
			getVariable().erase(getVariable().begin() + index);
			TrafficLightTimerComponent& owner = getOwner();
			if (owner.isRunning())
				owner.collectAllManagedComponents();
		}
	}
}

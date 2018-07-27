// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/building/BuildingLightTimeControlComponent.h"
#include "em5/component/building/BuildingComponent.h"

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BuildingLightTimeControlComponent::COMPONENT_ID = qsf::StringHash("em5::BuildingLightTimeControlComponent");


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	bool BuildingLightTimeControlComponent::onStartup()
	{
		if (LightTimeControlComponent::onStartup())
		{
			// Setup on/off times
			mWindowLightOnTime = getTimeFromTimeRange(mWindowLightEarliestOnTime, mWindowLightLatestOnTime);
			mWindowLightOffTime = getTimeFromTimeRange(mWindowLightEarliestOffTime, mWindowLightLatestOffTime);

			return true;
		}
		return false;
	}

	void BuildingLightTimeControlComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Serialize base class
		LightTimeControlComponent::serialize(serializer);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::LightTimeControlComponent methods ]
	//[-------------------------------------------------------]
	void BuildingLightTimeControlComponent::onUpdateLightState(const qsf::Time& currentTimeOfDay, bool forceSet)
	{
		// Call the base implementation
		LightTimeControlComponent::onUpdateLightState(currentTimeOfDay, forceSet);

		// Check current time of day against the interval when the window lights should be switched on
		const bool windowLightsShouldBeOn = (isActive() && isTimeInOnInterval(currentTimeOfDay, mWindowLightOnTime, mWindowLightOffTime));
		if (forceSet || mWindowLightIsOnStatus != windowLightsShouldBeOn)
		{
			// Check if we should switch the lights off
			mWindowLightIsOnStatus = windowLightsShouldBeOn;
			enableEmissiveMap(mWindowEmissiveMaterial, windowLightsShouldBeOn);
		}
	}

	void BuildingLightTimeControlComponent::enableChildLights(bool enable)
	{
		// No call to base because here we have a complete different implementation/usecase
		// Toggle linked lights on/off via the building component
		BuildingComponent* buildingComponent = getEntity().getComponent<BuildingComponent>();
		if (nullptr != buildingComponent)
		{
			buildingComponent->setLightsActive(enable, true);
		}
	}

	void BuildingLightTimeControlComponent::switchOffAllLights()
	{
		LightTimeControlComponent::switchOffAllLights();
		mWindowLightIsOnStatus = false;
		enableEmissiveMap(mWindowEmissiveMaterial, false);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

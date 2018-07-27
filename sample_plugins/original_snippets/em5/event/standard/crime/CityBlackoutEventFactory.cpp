// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/crime/CityBlackoutEventFactory.h"
#include "em5/event/standard/crime/CityBlackoutEvent.h"
#include "em5/freeplay/factory/FreeplayEventTriggerInfo.h"

#include <qsf_game/environment/time/TimeOfDayComponent.h>
#include <qsf_game/environment/time/TimeOfDayHelper.h>

#include <qsf/log/LogSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Anonymous namespace                                   ]
//[-------------------------------------------------------]
namespace
{
	const std::string DEFAULT_BASELAYER("FP_04_STROMAUSFALL");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CityBlackoutEventFactory::CityBlackoutEventFactory()
	{
		// Nothing here
	}

	CityBlackoutEventFactory::~CityBlackoutEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* CityBlackoutEventFactory::tryTriggerEvent()
	{
		// Make sure event cannot trigger twice
		if (isEventAlreadyRunning())
			return nullptr;

		if (!getTriggerInfo().mTriggeredByDebug)
		{
			// Check time of day
			if (mMinTriggerTimeOfDay != mMaxTriggerTimeOfDay)	// For the mini-event, this should be false
			{
				qsf::game::TimeOfDayComponent* timeOfDayComponent = QSF_MAINMAP.getCoreEntity().getComponent<qsf::game::TimeOfDayComponent>();
				if (nullptr != timeOfDayComponent)
				{
					qsf::Time currentTime = timeOfDayComponent->getCurrentTimeOfDay();
					if (!qsf::game::TimeOfDayHelper::isInBetweenDayTime(currentTime, mMinTriggerTimeOfDay, mMaxTriggerTimeOfDay))
					{
						setTriggerFailReason("City blackout event can only trigger between time of day " + std::to_string(mMinTriggerTimeOfDay.getHours()) + " and " + std::to_string(mMaxTriggerTimeOfDay.getHours()));
						return nullptr;
					}
				}
			}
		}

		// If base layer name is not set, set it to default
		if (mBaseLayer.empty())
		{
			mBaseLayer = DEFAULT_BASELAYER;
		}

		// Accidents are edited in layers. Find a layer to use.
		qsf::Layer* eventLayer = findCandidate();
		if (nullptr == eventLayer)
		{
			setTriggerFailReason("Could not find event layer " + mEventLayer);
			return nullptr;
		}

		// Log some debug information
		QSF_LOG_PRINTS(DEBUG, "Triggering city blackout at location: " << eventLayer->getName());

		// Create the event instance
		CityBlackoutEvent& freeplayEvent = createAndInitializeEvent<CityBlackoutEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize
		freeplayEvent.init(*eventLayer);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Layer* CityBlackoutEventFactory::findCandidate() const
	{
		// Use standard editing for event location layers
		return getCandidateEventLayer(mEventLayer, mBaseLayer, boost::bind(&CityBlackoutEventFactory::checkCandidate, this, _1));
	}

	bool CityBlackoutEventFactory::checkCandidate(const qsf::Layer& layer) const
	{
		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

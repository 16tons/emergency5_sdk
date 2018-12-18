// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/crime/EvacuateTrainStationEventFactory.h"
#include "em5/event/standard/crime/EvacuateTrainStationEvent.h"

#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/log/LogSystem.h>
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
	const std::string DEFAULT_BASELAYER("MINI04_MUN_BAHNHOFEVAKUIERUNG");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EvacuateTrainStationEventFactory::EvacuateTrainStationEventFactory()
	{
		// Nothing here
	}

	EvacuateTrainStationEventFactory::~EvacuateTrainStationEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* EvacuateTrainStationEventFactory::tryTriggerEvent()
	{
		// If base layer name is not set, set it to default
		if (mEventLayer.empty())
		{
			mEventLayer = DEFAULT_BASELAYER;
		}

		// Accidents are edited in layers. Find a layer to use.
		qsf::Layer* eventLayer = findCandidate();
		if (nullptr == eventLayer)
		{
			setTriggerFailReason("Could not find event layer " + mEventLayer);
			return nullptr;
		}

		// Log some debug information
		QSF_LOG_PRINTS(DEBUG, "Triggering train station evacuation at location: " << eventLayer->getName());

		// Create the event instance
		EvacuateTrainStationEvent& freeplayEvent = createAndInitializeEvent<EvacuateTrainStationEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize
		freeplayEvent.init(*eventLayer);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Layer* EvacuateTrainStationEventFactory::findCandidate() const
	{
		// There is only one instance in the Munich map, not more. Access the layer by name
		// If an event layer is explicitly set, we have to use this one
		if (!mEventLayer.empty())
		{
			qsf::Layer* eventLayer = QSF_MAINMAP.getLayerManager().getLayerByName(qsf::StringHash(mEventLayer));
			QSF_ASSERT(nullptr != eventLayer, getCampClass().name() << ": Can't find event layer \"" << mEventLayer << "\"", return nullptr);
			return eventLayer;
		}

		return nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

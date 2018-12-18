// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/TrafficAccident01EventFactory.h"
#include "em5/event/standard/destruction/TrafficAccident01Event.h"
#include "em5/event/EventHelper.h"

#include <qsf/QsfHelper.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>
#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>


//[-------------------------------------------------------]
//[ Anonymous namespace                                   ]
//[-------------------------------------------------------]
namespace
{
	const std::string LAYER_CAR_ACCIDENT("car_accident");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TrafficAccident01EventFactory::TrafficAccident01EventFactory()
	{
		// Nothing here
	}

	TrafficAccident01EventFactory::~TrafficAccident01EventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* TrafficAccident01EventFactory::tryTriggerEvent()
	{
		// If base layer name is not set, set it to "car_accident"
		if (mBaseLayer.empty())
		{
			mBaseLayer = LAYER_CAR_ACCIDENT;
		}

		// Accidents are edited in layers. Find a layer to use.
		qsf::Layer* accidentLayer = TrafficAccident01EventFactory::findCandidate();
		if (nullptr == accidentLayer)
		{
			setTriggerFailReason("Could not find event layer " + mEventLayer);
			return nullptr;
		}

		// Create the event instance
		TrafficAccident01Event& freeplayEvent = createAndInitializeEvent<TrafficAccident01Event>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize
		freeplayEvent.init(accidentLayer);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool TrafficAccident01EventFactory::checkCandidate(const qsf::Layer& layer) const
	{
		if (layer.isActive())
			return false;

		// Check if objects to spawn are collision free
		if (!TrafficAccident01Event::canSpawnEntitiesCollisionFree(layer, QSF_MAINMAP))
			return false;

		// Players must not see layer activation
		if (EventHelper::isLayerVisible(layer))
			return false;

		return true;
	}

	qsf::Layer* TrafficAccident01EventFactory::findCandidate() const
	{
		// Use standard editing for event location layers
		return getCandidateEventLayer(mEventLayer, mBaseLayer, boost::bind(&TrafficAccident01EventFactory::checkCandidate, this, _1));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

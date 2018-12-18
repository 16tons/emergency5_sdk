// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/TrainAccidentEventFactory.h"
#include "em5/event/standard/destruction/TrainAccidentEvent.h"

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
	const std::string DEFAULT_BASELAYER("FP_03_TRAINACCIDENT");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TrainAccidentEventFactory::TrainAccidentEventFactory()
	{
		// Nothing here
	}

	TrainAccidentEventFactory::~TrainAccidentEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* TrainAccidentEventFactory::tryTriggerEvent()
	{
		// If base layer name is not set, set it to "car_accident"
		if (mBaseLayer.empty())
		{
			mBaseLayer = DEFAULT_BASELAYER;
		}

		// Accidents are edited in layers. Find a layer to use.
		qsf::Layer* accidentLayer = TrainAccidentEventFactory::findCandidate();
		if (nullptr == accidentLayer)
		{
			setTriggerFailReason("Could not find event layer " + mEventLayer);
			return nullptr;
		}

		// Log some debug information
		QSF_LOG_PRINTS(DEBUG, "Triggering train accident at location: " << accidentLayer->getName());

		// Create the event instance
		TrainAccidentEvent& freeplayEvent = createAndInitializeEvent<TrainAccidentEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize
		freeplayEvent.init(accidentLayer);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Layer* TrainAccidentEventFactory::findCandidate() const
	{
		// Use standard editing for event location layers
		return getCandidateEventLayer(mEventLayer, mBaseLayer, boost::bind(&TrainAccidentEventFactory::checkCandidate, this, _1));
	}

	bool TrainAccidentEventFactory::checkCandidate(const qsf::Layer& layer) const
	{
		const std::vector<qsf::Layer*>& subLayers = layer.getLayers();

		if (subLayers.size() > 2)
			return false;

		// Search for layers with "_erase" and "_add" naming
		qsf::StringHash eraseLayerName = qsf::StringHash(mBaseLayer + "_erase");
		qsf::StringHash addLayerName = qsf::StringHash(mBaseLayer + "_add");

		qsf::Layer* eraseLayer = nullptr;
		qsf::Layer* addLayer = nullptr;

		for (size_t i = 0; i < subLayers.size(); ++i)
		{
			if (subLayers[i]->getNameHash() == eraseLayerName)
			{
				eraseLayer = subLayers[i];
			}
			else if (subLayers[i]->getNameHash() == addLayerName)
			{
				addLayer = subLayers[i];
			}

			if (eraseLayer != nullptr && addLayer != nullptr)
			{
				break;
			}
		}

		if (!eraseLayer || !addLayer)
			return false;

		if (addLayer->isActive())
			return false;

		// TODO(mk) Keep code here, we have to support this functions
		// Check if objects to spawn are collision free
		//if (!TrainAccidentEvent::canSpawnEntitiesCollisionFree(layer, QSF_MAINMAP))
		//	return false;

		// Players must not see layer activation
		//if (EventHelper::isLayerVisible(layer))
		//	return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

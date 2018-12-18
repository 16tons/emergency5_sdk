// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/DemonstrationEventFactory.h"
#include "em5/event/standard/gangster/DemonstrationEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/map/MapHelper.h"

#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf/map/layer/Layer.h>
#include <qsf/map/layer/LayerManager.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DemonstrationEventFactory::DemonstrationEventFactory() :
		mNumberOfCenters(1)
	{
		// Nothing here
	}

	DemonstrationEventFactory::~DemonstrationEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* DemonstrationEventFactory::tryTriggerEvent()
	{
		// If base layer name is not set, set it to "DEMONSTRATION"
		if (mBaseLayer.empty())
		{
			mBaseLayer = "DEMONSTRATION";
		}

		// Demonstrations are edited in layers, find a layer to use
		qsf::Layer* eventLayer = findCandidate();
		if (nullptr == eventLayer)
		{
			setTriggerFailReason("Could not find event layer " + mEventLayer);
			return nullptr;
		}

		// Create the event instance
		DemonstrationEvent& freeplayEvent = createAndInitializeEvent<DemonstrationEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize
		freeplayEvent.init(eventLayer);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool DemonstrationEventFactory::checkCandidate(const qsf::Layer& layer) const
	{
		if (layer.isActive())
			return false;

		// Check if objects to spawn are collision free
		if (!DemonstrationEvent::canSpawnEntitiesCollisionFree(layer, QSF_MAINMAP))
			return false;

		// Players must not see layer activation
		if (EventHelper::isLayerVisible(layer))
			return false;

		// Check number of centers
		uint32 centerFound = 0;
		qsf::ComponentMapQuery componentMapQuery(layer.getInternalBufferMap());
		for (const qsf::game::EventTagComponent* eventTagComponent : componentMapQuery.getAllInstances<qsf::game::EventTagComponent>())
		{
			if (eventTagComponent->hasTag(qsf::StringHash("DEMONSTRATION_CENTER"))
				|| eventTagComponent->hasTag(qsf::StringHash("DEMONSTRATION_HOTEL_CENTER")))
			{
				++centerFound;
			}
		}
		if (centerFound != mNumberOfCenters)
			return false;

		return true;
	}

	qsf::Layer* DemonstrationEventFactory::findCandidate() const
	{
		// Use standard editing for event location layers
		return getCandidateEventLayer(mEventLayer, mBaseLayer, boost::bind(&DemonstrationEventFactory::checkCandidate, this, _1));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

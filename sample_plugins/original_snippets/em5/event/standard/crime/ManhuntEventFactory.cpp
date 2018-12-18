// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/crime/ManhuntEventFactory.h"
#include "em5/event/standard/crime/ManhuntEvent.h"
#include "em5/component/building/BuildingComponent.h"

#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/component/event/EventTagManagerComponent.h>

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
	const std::string DEFAULT_BASELAYER("FP_05_FAHNDUNG");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ManhuntEventFactory::ManhuntEventFactory()
	{
		// Nothing here
	}

	ManhuntEventFactory::~ManhuntEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* ManhuntEventFactory::tryTriggerEvent()
	{
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
		QSF_LOG_PRINTS(DEBUG, "Triggering manhunt at location: " << eventLayer->getName());

		// Create the event instance
		ManhuntEvent& freeplayEvent = createAndInitializeEvent<ManhuntEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize
		freeplayEvent.init(*eventLayer);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Layer* ManhuntEventFactory::findCandidate() const
	{
		// Use standard editing for event location layers
		return getCandidateEventLayer(mEventLayer, mBaseLayer, boost::bind(&ManhuntEventFactory::checkCandidate, this, _1));
	}

	bool ManhuntEventFactory::checkCandidate(const qsf::Layer& layer) const
	{
		// Don't start the same event location twice
		if (layer.isActive())
			return false;

		const std::string TAG_HOUSE_REAL = ManhuntEvent::getHouseEventagFromEventLayerName(layer.getName());

		const std::vector<qsf::game::EventTagComponent*>& housesComponents = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(TAG_HOUSE_REAL), QSF_MAINMAP);
		for (qsf::game::EventTagComponent* houseComponent : housesComponents)
		{
			if (houseComponent->getEntity().getComponent<BuildingComponent>())
			{
				// Accepted
				return true;
			}
		}

		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

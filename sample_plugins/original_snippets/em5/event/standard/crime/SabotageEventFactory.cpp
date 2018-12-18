// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/crime/SabotageEventFactory.h"
#include "em5/event/standard/crime/SabotageEvent.h"

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
	const std::string DEFAULT_BASELAYER("FP_06_GROSSVERANSTALTUNG");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SabotageEventFactory::SabotageEventFactory()
	{
		// Nothing here
	}

	SabotageEventFactory::~SabotageEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* SabotageEventFactory::tryTriggerEvent()
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
		QSF_LOG_PRINTS(DEBUG, "Triggering sabotage at location: " << eventLayer->getName());

		// Create the event instance
		SabotageEvent& freeplayEvent = createAndInitializeEvent<SabotageEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize
		freeplayEvent.init(*eventLayer);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Layer* SabotageEventFactory::findCandidate() const
	{
		// Use standard editing for event location layers
		return getCandidateEventLayer(mEventLayer, mBaseLayer, boost::bind(&SabotageEventFactory::checkCandidate, this, _1));
	}

	bool SabotageEventFactory::checkCandidate(const qsf::Layer& layer) const
	{
		const std::vector<qsf::Layer*>& subLayers = layer.getLayers();
		for (size_t i = 0; i < subLayers.size(); ++i)
		{
			if (subLayers[i]->getName().find("_erase") != std::string::npos)
			{
				// Don't start the same event location twice (check if _erase layer is inactive)
				if (!subLayers[i]->isActive())
					return false;
			}
		}

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

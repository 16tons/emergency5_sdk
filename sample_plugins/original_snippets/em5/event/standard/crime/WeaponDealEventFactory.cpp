// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/crime/WeaponDealEventFactory.h"
#include "em5/event/standard/crime/WeaponDealEvent.h"
#include "em5/freeplay/factory/FreeplayEventTriggerInfo.h"
#include "em5/game/player/Player.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/units/UnitPool.h"

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
	const std::string DEFAULT_BASELAYER("FP_02_DROGENHANDEL");
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	WeaponDealEventFactory::WeaponDealEventFactory()
	{
		// Nothing here
	}

	WeaponDealEventFactory::~WeaponDealEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* WeaponDealEventFactory::tryTriggerEvent()
	{
		if (!getTriggerInfo().mTriggeredByDebug)
		{
			// We require the local player (host in multiplay) to have the police dog available
			if (!PlayerManager::getLocalPlayerSafe().getUnitPool().hasUnit("PHF"))
			{
				setTriggerFailReason("Police dog not available");
				return nullptr;
			}
		}

		// If base layer name is not set, set it to default
		if (mBaseLayer.empty())
		{
			mBaseLayer = DEFAULT_BASELAYER;
		}

		// Weapon deal event locations are edited in layers. Find a layer to use.
		qsf::Layer* eventLayer = findCandidate();
		if (nullptr == eventLayer)
		{
			setTriggerFailReason("Could not find event layer " + mEventLayer);
			return nullptr;
		}

		// Log some debug information
		QSF_LOG_PRINTS(DEBUG, "Triggering weapon deal at location: " << eventLayer->getName());

		// Create the event instance
		WeaponDealEvent& freeplayEvent = createAndInitializeEvent<WeaponDealEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize
		freeplayEvent.init(*eventLayer);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Layer* WeaponDealEventFactory::findCandidate() const
	{
		// Use standard editing for event location layers
		return getCandidateEventLayer(mEventLayer, mBaseLayer, boost::bind(&WeaponDealEventFactory::checkCandidate, this, _1));
	}

	bool WeaponDealEventFactory::checkCandidate(const qsf::Layer& layer) const
	{
		// Don't start the same event location twice
		if (layer.isActive())
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

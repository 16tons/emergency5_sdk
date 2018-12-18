// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/DefectEboxEventFactory.h"
#include "em5/event/standard/destruction/DefectEboxEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/units/UnitPool.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	// Event tags
	const std::string EVENT_TAG_EBOX_EVENT = "EBOX_EVENT";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DefectEboxEventFactory::DefectEboxEventFactory()
	{
		// Nothing here
	}

	DefectEboxEventFactory::~DefectEboxEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* DefectEboxEventFactory::tryTriggerEvent()
	{
		// Check if the needed units to finish the event are available (SEK drohne and rescue dog)
		if (!checkIfNeededUnitsAreAvailable())
			return nullptr;

		// Find spawn point
		qsf::Entity* eboxEntity = findCandidate();
		if (nullptr == eboxEntity)
			return nullptr;

		// Create the event instance
		DefectEboxEvent& freeplayEvent = createAndInitializeEvent<DefectEboxEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setEboxEntity(*eboxEntity);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	qsf::Entity* DefectEboxEventFactory::findCandidate() const
	{
		std::string eventTag = getEventTag();
		if (eventTag.empty())
		{
			eventTag = EVENT_TAG_EBOX_EVENT;
		}

		// Get a listing of all eboxes
		const auto& tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(eventTag), QSF_MAINMAP);
		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator(tagComponentsInMap);

		while (qsf::Entity* entity = randomEnumerator.getNextEntity())
		{
			// Check if the ebox is appropriate
			if (checkCandidate(*entity))
			{
				return entity;
			}
		}

		// None found
		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool DefectEboxEventFactory::checkCandidate(qsf::Entity& entity) const
	{
		EntityHelper entityHelper(entity);
		if (entityHelper.isBurned() || entityHelper.isBurning())
			return false;

		// Check if entity is a valid event target
		return entityHelper.isEntityValidEventTarget();
	}

	bool DefectEboxEventFactory::checkIfNeededUnitsAreAvailable() const
	{
		Player* localPlayer = PlayerManager::getLocalPlayer();
		if (nullptr == localPlayer)
			return false;

		// Check if engineer is available
		return (localPlayer->getUnitPool().getNumberMaxAvailableByName(CommandableComponent::UNITTAG_TECH_ENGINEER) > 0);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

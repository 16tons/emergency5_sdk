// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/HostagesEventFactory.h"
#include "em5/event/standard/gangster/HostagesEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/units/UnitPool.h"
#include "em5/map/EntityHelper.h"
#include "em5/specs/LogicSpecsGroup.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{

	namespace detail
	{
		const std::string EVENT_TAG_BUILDING("HOSTAGE");
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HostagesEventFactory::HostagesEventFactory() :
		mGangsterGiveUpChance(-1.0f)
	{
		// Nothing here
	}

	HostagesEventFactory::~HostagesEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* HostagesEventFactory::tryTriggerEvent()
	{
		const Player* localPlayer = PlayerManager::getLocalPlayer();
		if (nullptr == localPlayer)
			return nullptr;

		// Check if the key units for this event are available
		const uint32 numberOfSEKUnits = localPlayer->getUnitPool().getNumberMaxAvailableByName(CommandableComponent::UNITTAG_POLICE_SEK);
		if (numberOfSEKUnits < 3)
		{
			return nullptr;
		}

		// Find potential building where hostages can be taken
		qsf::Entity* candidate = findCandidate();
		if (nullptr == candidate)
			return nullptr;

		// Create the event instance
		HostagesEvent& freeplayEvent = createAndInitializeEvent<HostagesEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Random chance for the gangster to give up
		float giveUpChance = mGangsterGiveUpChance;
		if (giveUpChance < 0.0f || giveUpChance > 1.0f)
		{
			giveUpChance = LogicSpecsGroup::getInstanceSafe().getPoliceHostageTakerGiveUpChance();
		}
		const bool hostageTakerGiveUp = qsf::Random::getRandomChance(giveUpChance);

		// Initialize the event
		//  -> Layer to show is named just like the event tag
		freeplayEvent.init(*candidate, hostageTakerGiveUp, mLayerName);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* HostagesEventFactory::findCandidate()
	{
		std::string eventTag = getEventTag();
		if (eventTag.empty())
		{
			eventTag = detail::EVENT_TAG_BUILDING;
		}

		// Search for tagged entities
		std::vector<qsf::game::EventTagComponent*> tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsStartsWithTag(eventTag, QSF_MAINMAP);
		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator(constructionpolicy::SWAP, tagComponentsInMap);

		while (qsf::game::EventTagComponent* eventTagComponent = randomEnumerator.getNext())
		{
			// Check if the candidate is appropriate
			BuildingComponent* buildingComponent = eventTagComponent->getEntity().getComponent<BuildingComponent>();
			if (checkBuildingCandidate(*buildingComponent))
			{
				mLayerName = eventTagComponent->getTagStartsWithTag(eventTag);
				return &buildingComponent->getEntity();
			}
		}

		// None found
		return nullptr;
	}

	bool HostagesEventFactory::checkBuildingCandidate(em5::BuildingComponent& buildingComponent) const
	{
		const EntityHelper entityHelper(buildingComponent.getEntity());

		// Check if entity has an event ID component already
		if (entityHelper.checkForEventIdComponent())
			return false;

		// Building not damaged or destroyed
		if (!buildingComponent.isIntact())
			return false;

		// Check if building has all destruction stages
		if (!buildingComponent.getIntactMesh().isValid()
			|| !buildingComponent.getDamagedMesh().isValid()
			|| !buildingComponent.getDestroyedMesh().isValid())
			return false;

		// Building not burning
		if (entityHelper.isBurning())
			return false;

		// Check if there is a person inside the building
		if (!entityHelper.isContainerEmpty())
			return false;

		if (!entityHelper.isInEventBoundaries())
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

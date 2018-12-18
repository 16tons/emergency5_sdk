// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/destruction/BuildingNearlyCollapseEventFactory.h"
#include "em5/event/standard/destruction/BuildingNearlyCollapseEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	namespace detail
	{
		const std::string EVENT_TAG_BUILDING("BUILDING_NEARLY_COLLAPSE");
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BuildingNearlyCollapseEventFactory::BuildingNearlyCollapseEventFactory() :
		mShowTutorialHints(false),
		mUseAmbientLayer(false)
	{
		// Nothing here
	}

	BuildingNearlyCollapseEventFactory::~BuildingNearlyCollapseEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* BuildingNearlyCollapseEventFactory::tryTriggerEvent()
	{
		// Find a entity
		qsf::Entity* targetEntity = findCandidate();
		if (!targetEntity)
			return nullptr;

		// Create the event instance
		BuildingNearlyCollapseEvent& freeplayEvent = createAndInitializeEvent<BuildingNearlyCollapseEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.init(*targetEntity, mShowTutorialHints);
		if (mUseAmbientLayer && !mLayerName.empty())
		{
			freeplayEvent.setAmbientLayerName(mLayerName);
		}

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* BuildingNearlyCollapseEventFactory::findCandidate()
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
			qsf::Entity& entity = eventTagComponent->getEntity();
			BuildingComponent* buildingComponent = entity.getComponent<BuildingComponent>();

			// Check if the candidate is appropriate
			if (checkHouseCandidate(*buildingComponent))
			{
				if (mUseAmbientLayer)
				{
					mLayerName = eventTagComponent->getTagStartsWithTag(eventTag);
				}
				return &entity;
			}
		}

		return nullptr;
	}

	bool BuildingNearlyCollapseEventFactory::checkHouseCandidate(em5::BuildingComponent& buildingComponent) const
	{
		qsf::Entity& entityBuilding = buildingComponent.getEntity();
		const EntityHelper entityHelper(entityBuilding);

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

		// Get entities linked to the house
		qsf::LinkComponent* buildingLinkComponent = entityBuilding.getComponent<qsf::LinkComponent>();
		if (buildingLinkComponent == nullptr)
			return false;

		// Candidate is valid
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

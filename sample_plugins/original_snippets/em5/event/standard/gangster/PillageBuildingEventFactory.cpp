// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/PillageBuildingEventFactory.h"
#include "em5/event/standard/gangster/PillageBuildingEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{

	namespace detail
	{
		const std::string EVENT_TAG_PILLAGE_BUILDING = "PILLAGE_BUILDING";
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PillageBuildingEventFactory::PillageBuildingEventFactory() :
		mUseAmbientLayer(false)
	{
		// Nothing here
	}

	PillageBuildingEventFactory::~PillageBuildingEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* PillageBuildingEventFactory::tryTriggerEvent()
	{
		// Find potential pickpocket and victim
		qsf::Entity* candidate = findCandidate();

		if (candidate == nullptr)
			return nullptr;

		// Create the event instance
		PillageBuildingEvent& freeplayEvent = createAndInitializeEvent<PillageBuildingEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		freeplayEvent.init(*candidate);
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
	qsf::Entity* PillageBuildingEventFactory::findCandidate()
	{
		std::string eventTag = getEventTag();
		if (eventTag.empty())
		{
			eventTag = detail::EVENT_TAG_PILLAGE_BUILDING;
		}

		// Search for tagged entities
		std::vector<qsf::game::EventTagComponent*> tagComponentsInMap = qsf::game::EventTagManagerComponent::getEventTagComponentsStartsWithTag(eventTag, QSF_MAINMAP);
		ComponentsRandomEnumerator<qsf::game::EventTagComponent> randomEnumerator(constructionpolicy::SWAP, tagComponentsInMap);

		while (qsf::game::EventTagComponent* eventTagComponent = randomEnumerator.getNext())
		{
			// Check if the candidate is appropriate
			BuildingComponent* buildingComponent = eventTagComponent->getEntity().getComponent<BuildingComponent>();
			if (nullptr != buildingComponent && checkHouseCandidate(buildingComponent->getEntity()))
			{
				if (mUseAmbientLayer)
				{
					mLayerName = eventTagComponent->getTagStartsWithTag(eventTag);
				}
				return &buildingComponent->getEntity();
			}
		}

		// None found
		return nullptr;
	}

	bool PillageBuildingEventFactory::checkHouseCandidate(qsf::Entity& entity) const
	{
		EntityHelper entityHelper(entity);
		if (!entityHelper.isBuildingInValidState())
			return false;

		if (!entityHelper.isInEventBoundaries())
			return false;

		if (entityHelper.checkForInitializedEventIdComponent(nullptr))
			return false;

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

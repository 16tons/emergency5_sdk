// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/RowdyEventFactory.h"
#include "em5/event/standard/gangster/RowdyEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf/QsfHelper.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RowdyEventFactory::RowdyEventFactory() :
		mPeopleToAttack(0),
		mDebugTarget(false)
	{
		// Nothing here
	}

	RowdyEventFactory::~RowdyEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* RowdyEventFactory::tryTriggerEvent()
	{
		// Find potential spawn building
		qsf::Entity* candidate = findCandidate();
		if (nullptr == candidate)
			return nullptr;

		// Create the event instance
		RowdyEvent& freeplayEvent = createAndInitializeEvent<RowdyEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setBuilding(*candidate);
		freeplayEvent.setPeopleToAttack(mPeopleToAttack);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* RowdyEventFactory::findCandidate() const
	{
		// Try to find a bar
		{
			ComponentsRandomEnumerator<BuildingComponent> randomEnumerator;
			while (BuildingComponent* buildingComponent = randomEnumerator.getNext())
			{
				qsf::Entity& entity = buildingComponent->getEntity();
				if (checkCandidate(entity, mPrimaryBuildingTag))
					return &entity;
			}
		}

		// Failing that, try to find a shop
		{
			ComponentsRandomEnumerator<BuildingComponent> randomEnumerator;
			while (BuildingComponent* buildingComponent = randomEnumerator.getNext())
			{
				qsf::Entity& entity = buildingComponent->getEntity();
				if (checkCandidate(entity, mSecondaryBuildingTag))
					return &entity;
			}
		}

		QSF_WARN("Could not find a bar or a shop for rowdy event!", QSF_REACT_NONE);
		return nullptr;
	}

	bool RowdyEventFactory::checkCandidate(qsf::Entity& candidate, const std::string& tag) const
	{
		EntityHelper entityHelper(candidate);
		if (entityHelper.isBuildingDamaged())
			return false;

		BuildingComponent& buildingComponent = candidate.getComponentSafe<BuildingComponent>();
		if (!buildingComponent.hasDoor())
			return false;

		if (mDebugTarget)
			return true;

		qsf::game::EventTagComponent* eventTagComponent = candidate.getComponent<qsf::game::EventTagComponent>();
		if (nullptr == eventTagComponent)
			return false;
		if (!eventTagComponent->hasTag(tag))
			return false;

		// Building must be within gangster event area
		if (!entityHelper.isInGangsterEventArea())
			return false;

		// Accepted
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

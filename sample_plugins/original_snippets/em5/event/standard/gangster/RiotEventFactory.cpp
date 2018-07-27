// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/RiotEventFactory.h"
#include "em5/event/standard/gangster/RiotEvent.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RiotEventFactory::RiotEventFactory() :
		mPeopleToAttack(0),
		mMinStartGangsters(0),
		mMaxStartGangsters(0),
		mDebugTarget(false)
	{
		// Nothing here
	}

	RiotEventFactory::~RiotEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* RiotEventFactory::tryTriggerEvent()
	{
		// Find potential spawn building
		qsf::Entity* candidate = findCandidate();

		if (nullptr == candidate)
			return nullptr;

		// Create the event instance
		RiotEvent& freeplayEvent = createAndInitializeEvent<RiotEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setBuilding(*candidate);
		freeplayEvent.setPeopleToAttack(mPeopleToAttack);
		freeplayEvent.setStartGangsters(qsf::Random::getRandomUint(mMinStartGangsters, mMaxStartGangsters));

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* RiotEventFactory::findCandidate() const
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

		QSF_WARN("Could not find a bar or a shop for riot event!", QSF_REACT_NONE);
		return nullptr;
	}

	bool RiotEventFactory::checkCandidate(qsf::Entity& candidate, const std::string& tag) const
	{
		BuildingComponent& buildingComponent = candidate.getComponentSafe<BuildingComponent>();

		EntityHelper entityHelper(candidate);

		if (entityHelper.isBuildingDamaged())
			return false;

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

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/fire/TreeAndBushFireEventFactory.h"
#include "em5/event/standard/fire/SimpleFireEvent.h"
#include "em5/event/standard/fire/helper/FireEventCandidateSearchHelper.h"
#include "em5/base/ComponentsRandomEnumerator.h"
#include "em5/component/vegetation/TreeComponent.h"
#include "em5/component/vegetation/HedgeComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Locally used helper functions                         ]
	//[-------------------------------------------------------]
	namespace detail
	{
		bool checkCandidateForTreeAndBushFire(qsf::Entity& entity)
		{
			const EntityHelper entityHelper(entity);

			// Check if entity is a valid event target
			if (!entityHelper.isEntityValidEventTarget())
				return false;

			// Check if entity is not damaged or destroyed
			if (entityHelper.isEntityDamaged())
				return false;

			// Check if entity is burning or burned
			if (entityHelper.isBurning() || entityHelper.isBurned())
				return false;

			// Check if entity can be set on fire
			if (!entityHelper.canBeSetOnFireNow())
				return false;

			// Accepted
			return true;
		}
	}


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	qsf::Entity* TreeAndBushFireEventFactory::findTreeCandidate(const std::string& eventTag, const std::string& difficultyTags)
	{
		// If there are tags set
		if (difficultyTags.empty())
		{
			ComponentsRandomEnumerator<TreeComponent> randomEnumerator;
			while (TreeComponent* treeComponent = randomEnumerator.getNext())
			{
				qsf::Entity& entity = treeComponent->getEntity();
				if (!eventTag.empty())
				{
					qsf::game::EventTagComponent* eventTagComponent = entity.getComponent<qsf::game::EventTagComponent>();
					if (nullptr == eventTagComponent || !eventTagComponent->hasTag(eventTag))
						continue;
				}

				// Check if the tree is appropriate
				if (detail::checkCandidateForTreeAndBushFire(entity))
				{
					return &entity;
				}
			}
		}
		else
		{
			// Start the helper
			FireEventCandidateSearchHelper fireEventCandidateSearchHelper;
			fireEventCandidateSearchHelper.setDifficultyTagsByString(difficultyTags);

			ComponentsRandomEnumerator<TreeComponent> randomEnumerator;
			while (TreeComponent* treeComponent = randomEnumerator.getNext())
			{
				qsf::Entity& entity = treeComponent->getEntity();

				// Check difficulty tag
				if (!fireEventCandidateSearchHelper.hasDifficultyTag(entity))
					continue;

				// Check if the tree is appropriate
				if (detail::checkCandidateForTreeAndBushFire(entity))
				{
					return &entity;
				}
			}
		}

		// None found
		return nullptr;
	}

	qsf::Entity* TreeAndBushFireEventFactory::findHedgeCandidate(const std::string& eventTag, const std::string& difficultyTags)
	{
		// If there are tags set
		if (difficultyTags.empty())
		{
			ComponentsRandomEnumerator<HedgeComponent> randomEnumerator(20);		// Fixed number of tries
			while (HedgeComponent* hedgeComponent = randomEnumerator.getNext())
			{
				qsf::Entity& entity = hedgeComponent->getEntity();

				// Check if the hedge is appropriate
				if (detail::checkCandidateForTreeAndBushFire(entity))
				{
					return &entity;
				}
			}
		}
		else
		{
			// Start the helper
			FireEventCandidateSearchHelper fireEventCandidateSearchHelper;
			fireEventCandidateSearchHelper.setDifficultyTagsByString(difficultyTags);

			ComponentsRandomEnumerator<HedgeComponent> randomEnumerator;
			while (HedgeComponent* hedgeComponent = randomEnumerator.getNext())
			{
				qsf::Entity& entity = hedgeComponent->getEntity();

				// Check difficulty tag
				if (!fireEventCandidateSearchHelper.hasDifficultyTag(entity))
					continue;

				// Check if the hedge is appropriate
				if (detail::checkCandidateForTreeAndBushFire(entity))
				{
					return &entity;
				}
			}
		}

		// None found
		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TreeAndBushFireEventFactory::TreeAndBushFireEventFactory()
	{
		// Nothing here
	}

	TreeAndBushFireEventFactory::~TreeAndBushFireEventFactory()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEventFactory methods      ]
	//[-------------------------------------------------------]
	FreeplayEvent* TreeAndBushFireEventFactory::tryTriggerEvent()
	{
		// Find a candidate entity
		qsf::Entity* targetEntity = findCandidate();
		if (nullptr == targetEntity)
		{
			setTriggerFailReason("Could not find tree entity by event tag " + getDifficultyTags());
			return nullptr;
		}

		// Create the event instance
		SimpleFireEvent& freeplayEvent = createAndInitializeEvent<SimpleFireEvent>();
		deserializeRandomFreeplayVariant(freeplayEvent);

		// Initialize the event
		freeplayEvent.setTargetEntity(*targetEntity);

		// Done
		return &freeplayEvent;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	qsf::Entity* TreeAndBushFireEventFactory::findCandidate() const
	{
		if (mEventTag == "CHRISTMAS_TREE")
			return findTreeCandidate(mEventTag, mDifficultyTags);

		// This switch case is to get the right target type
		switch (qsf::Random::getRandomUint(0, 1))
		{
			// Tree
			case 0:
			{
				// Find tree candidate
				qsf::Entity* entity = findTreeCandidate(mEventTag, mDifficultyTags);
				if (nullptr != entity)
					return entity;

				// If no tree is found, find hedge
				return findHedgeCandidate(mEventTag, mDifficultyTags);
			}

			// Hedge
			case 1:
			{
				// Find hedge candidate
				qsf::Entity* entity = findHedgeCandidate(mEventTag, mDifficultyTags);
				if (nullptr != entity)
					return entity;

				// If no hedge is found, find tree
				return findTreeCandidate(mEventTag, mDifficultyTags);
			}

			default:
				QSF_WARN("Got a wrong number to switch on", QSF_REACT_NONE);
				return nullptr;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

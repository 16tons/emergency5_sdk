// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/GangsterBrawlerUnarmed.h"
#include "em5/logic/local/gangsters/GangsterGaper.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/component/event/EventTagComponent.h>

#include <qsf/job/JobArguments.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterBrawlerUnarmed::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterBrawlerUnarmed");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterBrawlerUnarmed::GangsterBrawlerUnarmed() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID),
		mCurrentTargetId(qsf::getUninitialized<uint64>()),
		mAggroRange(12.0f),
		mDeAggroRange(15.0f),
		mHitCoolDown(qsf::Time::ZERO),
		mHitCoolDownTime(qsf::Time::fromSeconds(30.0f))
	{
		mEquippedWeaponTypes.set(weapon::FIST);

		// This gangster type always show his weapon
		mAlwaysShowWeapon = true;
	}

	void GangsterBrawlerUnarmed::setAggroRange(float range)
	{
		mAggroRange = range;
	}

	qsf::Entity* GangsterBrawlerUnarmed::getNextTarget() const
	{
		qsf::Entity* gangster = getEntity();
		QSF_CHECK(nullptr != gangster, "Gangster does not exist!", return nullptr);

		qsf::TransformComponent* tc = gangster->getComponent<qsf::TransformComponent>();
		QSF_CHECK(nullptr != tc, "Gangster does not have a transform component!", return nullptr);

		// Create a component map query to search for suitable targets
		qsf::ComponentMapQuery componentMapQuery(getMap());

		// Entity and priority
		std::vector<PersonComponent*> personCompsInRange;
		componentMapQuery.getInstancesInCircle<PersonComponent>(mTransformComponent->getPosition(), mAggroRange, personCompsInRange);

		for (const PersonComponent* personComponent : personCompsInRange)
		{
			qsf::Entity& targetEntity = personComponent->getEntity();

			// Don't attack yourself
			if (personComponent->getEntityId() == getEntityId())
				continue;

/*
			// TODO(co) Tag is never used? Can someone review this?
			std::string tag;
			if (targetEntity.getComponent<qsf::game::EventTagComponent>())
			{
				tag = targetEntity.getComponent<qsf::game::EventTagComponent>()->getEventTagsAsString();
			}
*/

			// Do the usual tests (no injuries, deaths, no children, not part of an event, ...)
			/*if (!EntityHelper(targetEntity).isValidGangsterEventTarget(*gangster, false))
				continue;*/

			if (EntityHelper(targetEntity).isPersonInjured())
				continue;

			qsf::GameLogicComponent* gameLogic = targetEntity.getComponent<qsf::GameLogicComponent>();
			if (gameLogic == nullptr)
			{
				continue;
			}

			if (gameLogic->getGameLogic<GangsterBrawlerUnarmed>() == nullptr && gameLogic->getGameLogic<GangsterGaper>() == nullptr)
			{
				continue;
			}

			if (gameLogic->getGameLogic<GangsterBrawlerUnarmed>() != nullptr && (gameLogic->getGameLogic<GangsterBrawlerUnarmed>()->isCurrentlyFighting() || gameLogic->getGameLogic<GangsterBrawlerUnarmed>()->hasTarget()))
			{
				continue;
			}

			if (gameLogic->getGameLogic<GangsterGaper>() != nullptr && ( gameLogic->getGameLogic<GangsterGaper>()->hasTarget()))
			{
				continue;
			}

			return &personComponent->getEntity();
		}

		// Nothing found
		return nullptr;
	}

	void GangsterBrawlerUnarmed::setDeAggroRange(float val)
	{
		mDeAggroRange = val;
	}

	void GangsterBrawlerUnarmed::setHitCoolDownTime(qsf::Time val)
	{
		mHitCoolDownTime = val;
	}

	bool GangsterBrawlerUnarmed::hasTarget() const
	{
		return qsf::isInitialized(mCurrentTargetId);
	}

	void GangsterBrawlerUnarmed::setTarget(uint64 targetId)
	{
		mCurrentTargetId = targetId;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	void GangsterBrawlerUnarmed::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		QSF_CHECK(nullptr != getEntity(), "No entity exists! This must not happen!", return);

		qsf::ActionComponent* actionComponent = getEntity()->getOrCreateComponent<qsf::ActionComponent>();

		if (!actionComponent->getPlan().isEmpty())
		{
			// Gangster got work to do, let him finish that
			return;
		}

		qsf::Entity* currentTarget = nullptr;

		if (qsf::isUninitialized(mCurrentTargetId))
		{
			// Gangster needs a new target!
			currentTarget = getNextTarget();
		}
		else
		{
			// Stick with the current target as it is still valid
			currentTarget = getMap().getEntityById(mCurrentTargetId);
		}
		if (currentTarget != nullptr && qsf::game::DistanceHelper::get2dDistance(*getEntity(), *currentTarget) > mDeAggroRange)
		{
			currentTarget = getNextTarget();
		}

		if (mHitCoolDown > qsf::Time::ZERO)
		{
			mHitCoolDown -= jobArguments.getTimePassed();
			currentTarget = nullptr;
		}

		if (nullptr == currentTarget)
		{
			// No target in range. Become a Guard passage
			//changeLogic(qsf::StringHash("em5::GangsterGuardPassageLogic"));
		}
		else
		{
			// The gangster has a registered target

			mCurrentTargetId = currentTarget->getId();

			EntityHelper targetEntityHelper(*currentTarget);
			bool isValid = true;
			if (targetEntityHelper.isPersonInjured())
			{
				mHitCoolDown = mHitCoolDownTime;
				actionComponent->clearPlan();
				AnimationHelper gangsterAnimationHelper(*getEntity());
				gangsterAnimationHelper.playAnimation("em5/skeleton/default/male_cheer", true);
				isValid = false;
			}

			if (isValid && qsf::game::DistanceHelper::getDistance(*getEntity(), *currentTarget) < mDeAggroRange)
			{
				// And the target is neither injured, nor dead, nor burning, ...

				qsf::GameLogicComponent* gameLogic = currentTarget->getComponent<qsf::GameLogicComponent>();
				if (gameLogic->getGameLogic<GangsterBrawlerUnarmed>() != nullptr)
				{
					gameLogic->getGameLogic<GangsterBrawlerUnarmed>()->setTarget(getEntity()->getId());
				}
				else if (gameLogic->getGameLogic<GangsterGaper>() != nullptr)
				{
					gameLogic->getGameLogic<GangsterGaper>()->setTarget(getEntity()->getId());
				}

				// Start a fight!
				attack(*currentTarget);
			}
			else
			{
				// Reset target -> search new one
				mCurrentTargetId = qsf::getUninitialized<uint64>();
				return;
			}
		}
	}

	void GangsterBrawlerUnarmed::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		// Nothing here (Everything is handled in the onSimulationUpdate() )
	}

	void GangsterBrawlerUnarmed::onNoUnitsSpotted()
	{
		// Nothing here (Everything is handled in the onSimulationUpdate() )
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

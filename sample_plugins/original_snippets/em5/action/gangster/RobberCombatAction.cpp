// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/gangster/RobberCombatAction.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/component/base/TransformComponent.h>
#ifndef ENDUSER
	#include <qsf/debug/request/TextDebugDrawRequest.h>
#endif
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier RobberCombatAction::ACTION_ID = "em5::RobberCombatAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RobberCombatAction::RobberCombatAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mHitCounter(0)
	{
		// Nothing here
	}

	RobberCombatAction::~RobberCombatAction()
	{
		// Nothing here
	}

	void RobberCombatAction::init(const qsf::Entity& entity, bool)
	{
		mTargetEntityId = entity.getId();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void RobberCombatAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serialize(mHitCounter);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool RobberCombatAction::onStartup()
	{
		// Get the caller and target entity
		const qsf::Entity& callerEntity = getEntity();
		const qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getPullObjectDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		return true;
	}

	qsf::action::Result RobberCombatAction::updateAction(const qsf::Clock&)
	{
		// Get the entity of the caller
		qsf::Entity& callerEntity = getEntity();

		// Get the target entity instance
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "The EM5 robber combat action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		PersonComponent* attackerPersonComponent = callerEntity.getComponent<PersonComponent>();
		QSF_CHECK(nullptr != attackerPersonComponent, "The EM5 robber combat action was unable to retrieve the target entity's person component", return qsf::action::RESULT_DONE);

		PersonComponent* targetPersonComponent = targetEntity->getComponent<PersonComponent>();
		QSF_CHECK(nullptr != targetPersonComponent, "The EM5 robber combat action was unable to retrieve the target entity's person component", return qsf::action::RESULT_DONE);

		HealthComponent* healthComponentAttacker = callerEntity.getComponent<HealthComponent>();
		QSF_CHECK(nullptr != healthComponentAttacker, "The EM5 robber combat action was unable to retrieve the target entity's health component", return qsf::action::RESULT_DONE);

		HealthComponent* healthComponentTarget = targetEntity->getComponent<HealthComponent>();
		QSF_CHECK(nullptr != healthComponentTarget, "The EM5 robber combat action was unable to retrieve the target entity's health component", return qsf::action::RESULT_DONE);

		QSF_CHECK(nullptr == EntityHelper(*targetEntity).getContainerEntity(), "The EM5 robber combat action was unable to continue because the target entity is inside a container (vehicle or building)", return qsf::action::RESULT_DONE);

		const float lowerAttackerHealth = 0.05f;
		const float lowerTargetHealth = 0.25f;

		if (mHitCounter % 2 == 0)
		{
			// Person is now involved in a fight and can't turn to other attackers
			targetPersonComponent->setPersonFlags(PersonComponent::FLAG_INVOLVED_IN_FIGHT, true);
			#ifndef ENDUSER
				mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
			#endif
			glm::vec3 textPosition = callerEntity.getComponentSafe<qsf::TransformComponent>().getPosition();
			textPosition.y += 2;

			// TODO(fr) Wait action is just a placeholder, use a animation here!
			#ifndef ENDUSER
				mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("First Hit!", textPosition, qsf::Color4::YELLOW));
			#endif
			getComponent().pushAction<qsf::WaitAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(2.0f));

			// Lower health of target by 10%
			healthComponentTarget->setHealthEnergy(healthComponentTarget->getHealthEnergy() - (healthComponentTarget->getMaximumHealthEnergy() * lowerTargetHealth));
		}
		else
		{
			#ifndef ENDUSER
				mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
			#endif

			// TODO(fr) Wait action is just a placeholder, use a animation here!
			glm::vec3 textPosition = callerEntity.getComponentSafe<qsf::TransformComponent>().getPosition();
			textPosition.y += 2;
			#ifndef ENDUSER
				mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("Second Hit!", textPosition, qsf::Color4::YELLOW));
			#endif
			getComponent().pushAction<qsf::WaitAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(2.0f));

			// Lower health of attacker
			healthComponentAttacker->setHealthEnergy(healthComponentAttacker->getHealthEnergy() - (healthComponentAttacker->getMaximumHealthEnergy() * lowerAttackerHealth));

		}
		++mHitCounter;
		if (mHitCounter == 7)
		{
			healthComponentTarget->setHealthEnergy(0);
			targetPersonComponent->setPersonFlags(PersonComponent::FLAG_INVOLVED_IN_FIGHT, false);
			return qsf::action::RESULT_DONE;
		}

		return qsf::action::RESULT_CONTINUE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

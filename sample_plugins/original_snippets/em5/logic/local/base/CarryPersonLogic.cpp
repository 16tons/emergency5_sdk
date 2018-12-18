// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/fire/PullPersonAction.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/logic/local/base/CarryPersonLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/Map.h>
#include <qsf/renderer/animation/MeshAnimationComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CarryPersonLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::CarryPersonLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CarryPersonLogic::CarryPersonLogic(uint32 gameLogicId) :
		qsf::GameLogic(gameLogicId),
		mCarryPersonId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	uint64 CarryPersonLogic::getCarryPerson() const
	{
		return mCarryPersonId;
	}

	void CarryPersonLogic::setCarryPerson(uint64 person)
	{
		if (qsf::isInitialized(mCarryPersonId))
		{
			mMessageProxyInjured.unregister();
			mMessageProxyUnitInjured.unregister();
		}

		mCarryPersonId = person;

		if (qsf::isInitialized(mCarryPersonId))
		{
			// Reset the animation of the carried entity
			qsf::Entity* carryEntity = getMap().getEntityById(mCarryPersonId);
			if (nullptr != carryEntity)
			{
				PersonComponent* personComponent = carryEntity->getComponent<PersonComponent>();
				if (nullptr != personComponent)
				{
					personComponent->clearAnimation();
				}
			}

			// Register to catch if our guided person was injured while being carried
			mMessageProxyInjured.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSINJURED_PERSON, mCarryPersonId), boost::bind(&CarryPersonLogic::onPersonInjured, this, _1));

			// Register to catch if our unit person was injured while pulling something
			mMessageProxyUnitInjured.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSINJURED_PERSON, getEntityId()), boost::bind(&CarryPersonLogic::onUnitPersonInjured, this, _1));
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool CarryPersonLogic::onStartup()
	{
		// Call base implementation
		return qsf::GameLogic::onStartup();
	}

	void CarryPersonLogic::onShutdown()
	{
		mMessageProxyInjured.unregister();
		mMessageProxyUnitInjured.unregister();

		// Call base implementation
		qsf::GameLogic::onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CarryPersonLogic::onPersonInjured(const qsf::MessageParameters& parameters)
	{
		// Injuries while we carry it. Fireman should switch to pulling
		if (getEntity() != nullptr && UnitHelper(*getEntity()).isFireFighterPerson())
		{
			AnimationHelper(*getEntity()).clearAnimation();
			qsf::MeshAnimationComponent* meshAnimationComponent = getEntity()->getOrCreateComponent<qsf::MeshAnimationComponent>();
			if (meshAnimationComponent != nullptr)
			{
				AnimationHelper::AnimationMaskData animationMaskData;
				const qsf::AssetProxy assetProxy(AnimationHelper(*getEntity()).getIdleAnimation(animationMaskData));
				meshAnimationComponent->playAnimation(assetProxy, true);
			}

			qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

			// We have to clear the plan - can't just continue walking as pulling is backwards while leading is moving forwards.
			actionComponent.clearPlan();

			actionComponent.pushAction<PullPersonAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(mCarryPersonId, PullPersonAction::START, true);
			actionComponent.pushAction<qsf::WaitAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(qsf::Time::fromSeconds(1.f));	// give target person some time to fall to the floor
		}
	}

	void CarryPersonLogic::onUnitPersonInjured(const qsf::MessageParameters& parameters)
	{
		// Fireman gets injured while carrying a person -> unlink them
		if (getEntity() != nullptr)
		{
			qsf::Entity* carriedEntity = getMap().getEntityById(mCarryPersonId);
			if (nullptr != carriedEntity)
			{
				AnimationHelper(*carriedEntity).clearAnimation();
				EntityHelper(*carriedEntity).unlinkFromParent();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

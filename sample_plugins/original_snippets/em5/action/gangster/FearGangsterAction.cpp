// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/gangster/FearGangsterAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/logic/action/TurnToAction.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Random.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier FearGangsterAction::ACTION_ID = "em5::FearGangsterAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FearGangsterAction::FearGangsterAction() :
		Action(ACTION_ID),
		mGangsterEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	FearGangsterAction::~FearGangsterAction()
	{
		// Nothing here
	}

	void FearGangsterAction::init(const qsf::Entity& gangsterEntity)
	{
		mGangsterEntityId = gangsterEntity.getId();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void FearGangsterAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mGangsterEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool FearGangsterAction::onStartup()
	{
		// Check if our gangster is still around
		qsf::Entity& entity = getEntity();
		qsf::Entity* gangsterEntity = getMap().getEntityById(mGangsterEntityId);
		if (nullptr == gangsterEntity)
			return false;

		// Check distance to gangster
		const float currentDistance = qsf::game::DistanceHelper::getDistance(entity, *gangsterEntity);
		if (currentDistance > ActionRangeSpecsGroup::getInstanceSafe().getGangsterFearDistance())
			return false;

		// I was a gangster once!
		if (EntityHelper(entity).isArrested())
			return false;

		// Push TurnToAction
		qsf::ActionComponent* actionComponent = entity.getComponent<qsf::ActionComponent>();
		if (nullptr == actionComponent)
			return false;

		actionComponent->pushAction<qsf::TurnToAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(gangsterEntity->getId());

		return true;
	}

	void FearGangsterAction::onShutdown()
	{
		AnimationHelper::AnimationMaskData animationMaskData;
		AnimationHelper animationHelper(getEntity());
		animationHelper.playAnimation(animationHelper.getIdleAnimation(animationMaskData), true, false, qsf::Time::fromSeconds(1.0f));

		Action::onShutdown();
	}

	qsf::action::Result FearGangsterAction::updateAction(const qsf::Clock&)
	{
		qsf::Entity& entity = getEntity();
		qsf::Entity* gangsterEntity = getMap().getEntityById(mGangsterEntityId);
		if (nullptr == gangsterEntity)
			return qsf::action::RESULT_DONE;

		EntityHelper gangsterEntityHelper(*gangsterEntity);
		if (gangsterEntityHelper.isArrested() || gangsterEntityHelper.isPersonInjured())
			return qsf::action::RESULT_DONE;

		const float currentDistance = qsf::game::DistanceHelper::getDistance(entity, *gangsterEntity);
		if (currentDistance > ActionRangeSpecsGroup::getInstanceSafe().getGangsterFearDistance())
			return qsf::action::RESULT_DONE;

		EntityHelper entityHelper(entity);
		if (entityHelper.isGangsterPerson())
			return qsf::action::RESULT_DONE;

		qsf::ActionComponent& actionComponent = getComponent();

		// Push play animation
		switch (qsf::Random::getRandomUint(0, 1))
		{
			case 0:
				actionComponent.pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(em5::AnimationHelper(entity).getAnimationDuckDownEnd(), true, false, false, qsf::Time::fromSeconds(0.5f));
				actionComponent.pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(em5::AnimationHelper(entity).getAnimationDuckDownIdle(), true, true, false, qsf::Time::fromSeconds(0.5f), qsf::Time::fromSeconds(4.0f));
				actionComponent.pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(em5::AnimationHelper(entity).getAnimationDuckDownStart(), true, false, false, qsf::Time::fromSeconds(0.5f));
				break;

			case 1:
				actionComponent.pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(em5::AnimationHelper(entity).getAnimationWince(), true, false, false, qsf::Time::fromSeconds(1.0f));
		}

		return qsf::action::RESULT_CONTINUE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

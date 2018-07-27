// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/gangster/DemonstrateAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/component/base/TransformComponent.h>
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
	const qsf::NamedIdentifier DemonstrateAction::ACTION_ID = "em5::DemonstrateAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DemonstrateAction::DemonstrateAction() :
		Action(ACTION_ID),
		mEventId(qsf::getUninitialized<uint64>()),
		mDemonstrationRadius(0.0f)
	{
		// Nothing here
	}

	DemonstrateAction::~DemonstrateAction()
	{
		// Nothing here
	}

	void DemonstrateAction::init(uint64 eventId, const glm::vec3& demonstrationCenter, float demonstrationRadius, const qsf::NamedIdentifier& equipment)
	{
		mEventId = eventId;
		mDemonstrationCenter = demonstrationCenter;
		mDemonstrationRadius = demonstrationRadius;
		mEquipment = equipment;
	}

	uint64 DemonstrateAction::getEventId() const
	{
		return mEventId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DemonstrateAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mEquipment);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool DemonstrateAction::onStartup()
	{
		qsf::Entity& entity = getEntity();
		qsf::ActionComponent* actionComponent = entity.getComponent<qsf::ActionComponent>();
		if (nullptr == actionComponent)
			return false;

		// Test distance from demonstration
		qsf::TransformComponent* transformComponent = entity.getComponent<qsf::TransformComponent>();
		if (nullptr == transformComponent)
			return false;

		float distanceFromCenter = qsf::game::DistanceHelper::getSquaredDistance(transformComponent->getPosition(), mDemonstrationCenter);
		if (distanceFromCenter > mDemonstrationRadius)
			return false;

		const float equipChance = qsf::Random::getRandomFloat(0.0f, 1.0f);

		if (mEquipment.empty())
		{
			mEquipment =
				(equipChance < 0.15f) ? equipment::DEMO_SIGN_1 :	// 15%
				(equipChance < 0.30f) ? equipment::DEMO_SIGN_2 :	// 15%
				(equipChance < 0.45f) ? equipment::DEMO_SIGN_3 :	// 15%
				(equipChance < 0.60f) ? equipment::MOBILEPHONE :	// 15%
				(equipChance < 0.75f) ? equipment::BOTTLE :			// 15%
				(equipChance < 0.80f) ? equipment::GUITAR : "";		// 15%
		}

		if (!mEquipment.empty())
		{
			EntityHelper(entity).showEquipment(mEquipment);
		}

		return true;
	}

	void DemonstrateAction::onShutdown()
	{
		qsf::Entity& entity = getEntity();

		EntityHelper(entity).deleteActiveEquipment();

		AnimationHelper::AnimationMaskData animationMaskData;
		AnimationHelper animationHelper(entity);
		animationHelper.playAnimation(animationHelper.getIdleAnimation(animationMaskData), true, false, qsf::Time::fromSeconds(1.0f));

		Action::onShutdown();
	}

	qsf::action::Result DemonstrateAction::updateAction(const qsf::Clock&)
	{
		qsf::Entity& entity = getEntity();

		qsf::ActionComponent* actionComponent = entity.getComponent<qsf::ActionComponent>();
		if (nullptr == actionComponent)
			return qsf::action::RESULT_DONE;

		AnimationHelper animationHelper(entity);

		if (mEquipment == equipment::DEMO_SIGN_1 || mEquipment == equipment::DEMO_SIGN_2 || mEquipment == equipment::DEMO_SIGN_3)
		{
			std::string animationPath;

			AnimationHelper::AnimationMaskData animationMaskData;
			animationHelper.getIdleAnimation(animationMaskData);

			const float animationChance = qsf::Random::getRandomFloat(0.0f, 1.0f);

			animationPath = (animationChance < 0.40f) ? animationHelper.getAnimationFemaleStandIdleBottle() :
							(animationChance < 0.80f) ? animationMaskData.mMaskAnimation :
														animationHelper.getAnimationWaveObject();

			actionComponent->pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(animationPath, true, true, false, qsf::Time::fromSeconds(2.0f), qsf::Time::fromSeconds(2.0f));	// Extra weiche Übergänge

		}
		else if (mEquipment == equipment::MOBILEPHONE)
		{
			std::string animationPath;

			switch (qsf::Random::getRandomUint(1, 3)) // Choose a random animation
			{
				case 1: animationPath = animationHelper.getAnimationMobilephoneUseTalk(); break;
				case 2: animationPath = animationHelper.getAnimationMobilephoneUsePhotographNear(); break;
				case 3: animationPath = animationHelper.getAnimationMobilephoneUsePhotograph(); break;
			}

			actionComponent->pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(animationPath, true, false, false, qsf::Time::fromSeconds(0.5f));
		}
		else if (mEquipment == equipment::BOTTLE)
		{
			std::string animationPath;

			switch (qsf::Random::getRandomUint(1, 3)) // Choose a random animation
			{
				case 1: animationPath = animationHelper.getAnimationHooliganCheeringWithBottle(); break;
				case 2: animationPath = animationHelper.getAnimationFemalePointingWithBottle(); break;
				case 3: animationPath = animationHelper.getAnimationStandIdleBottle(); break;
			}

			actionComponent->pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(animationPath, true, true, false, qsf::Time::fromSeconds(1.0f), qsf::Time::fromSeconds(3.0f));	// Extra weiche Übergänge
		}
		else if (mEquipment == equipment::GUITAR)
		{
			actionComponent->pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(animationHelper.getAnimationPlayGuitar(), true, true, false, qsf::Time::fromSeconds(0.5f), qsf::Time::fromSeconds(3.0f));
		}
		else
		{
			std::string animationPath;

			switch (qsf::Random::getRandomUint(1, 5)) // Choose a random animation
			{
				case 1: animationPath = animationHelper.getAnimationCheer(); break;
				case 2: animationPath = animationHelper.getAnimationWaveHelpStanding(); break;
				case 3: animationPath = animationHelper.getAnimationTalkExcited(); break;
				case 4: animationPath = animationHelper.getAnimationTalkStanding(); break;
				case 5: animationPath = animationHelper.getAnimationCivilGape3(); break;
			}

			actionComponent->pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(animationPath, true, false, false, qsf::Time::fromSeconds(0.5f));
		}


		return qsf::action::RESULT_CONTINUE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

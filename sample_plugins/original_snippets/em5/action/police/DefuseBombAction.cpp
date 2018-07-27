// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/DefuseBombAction.h"
#include "em5/command/police/DefuseBombCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/objects/BombComponent.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/audio/AudioHelper.h"
#include "em5/audio/AudioProxy.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/renderer/animation/MeshAnimationComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier DefuseBombAction::ACTION_ID = "em5::DefuseBombAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DefuseBombAction::DefuseBombAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT)
	{
		// Nothing here
	}

	DefuseBombAction::~DefuseBombAction()
	{
		abortDefuseBomb();
	}

	void DefuseBombAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntity = targetEntity;
	}

	uint64 DefuseBombAction::getTargetEntityId() const
	{
		return mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DefuseBombAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(fw): Serialization
		//serializer.serialize(mTargetEntity);
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool DefuseBombAction::onStartup()
	{
		return validateUserAndTarget();
	}

	qsf::action::Result DefuseBombAction::updateAction(const qsf::Clock&)
	{
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				if (!validateUserAndTarget())
				{
					return qsf::action::RESULT_DONE;
				}
				startDefuseBomb();

				// Play defuse bomb
				AudioProxy audioProxy;
				AudioHelper::playSEKRoboterDefuseBombe(audioProxy, getEntity());
				audioProxy.detach();
				audioProxy.synchronizeWithMultiplay();

				// Play defuse animation
				AnimationHelper animationHelper(getEntity());
				animationHelper.playAnimation(animationHelper.getAnimationSEKRobotDefuseBomb(), true, false, qsf::Time::fromSeconds(2.0f));

				mCurrentState = STATE_FINISH_DEFUSE;
				// Fall through by design
			}

			case STATE_FINISH_DEFUSE:
			{
				if (canBombBeDefused())
				{
					// Continue defusing process till bomb is exploded or defused
					return qsf::action::RESULT_CONTINUE;
				}

				// Stop defuse animation
				getEntity().getOrCreateComponentSafe<qsf::MeshAnimationComponent>().stopAllAnimations();

				mCurrentState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DONE:
			{
				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool DefuseBombAction::validateUserAndTarget()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		if (!mTargetEntity.valid())
			return false;

		// TODO(mk) Use the right distance check here
		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getArrestGangsterDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*mTargetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		DefuseBombCommand* defuseBombCommand = static_cast<DefuseBombCommand*>(QSFGAME_COMMAND.getCommandManager().getById<DefuseBombCommand>(DefuseBombCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != defuseBombCommand, "DefuseBombAction::validateUserAndTarget(): Could not find defuse bomb command instance", return false);
		if (!(defuseBombCommand->checkCaller(*commandContext.mCaller) && defuseBombCommand->checkContext(commandContext)))
			return false;

		return true;
	}

	void DefuseBombAction::startDefuseBomb()
	{
		setBombState(true);
	}

	void DefuseBombAction::abortDefuseBomb()
	{
		setBombState(false);
	}

	void DefuseBombAction::setBombState(bool value)
	{
		if (mTargetEntity.valid())
		{
			BombComponent* bombComponent = mTargetEntity->getComponent<BombComponent>();
			if (nullptr != bombComponent)
			{
				bombComponent->setIsDefuseBombActive(value);
			}
		}
	}

	bool DefuseBombAction::canBombBeDefused()
	{
		if (mTargetEntity.valid())
		{
			const BombComponent* bombComponent = mTargetEntity->getComponent<BombComponent>();
			if (nullptr != bombComponent)
			{
				return bombComponent->canBeDefused();
			}
		}
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

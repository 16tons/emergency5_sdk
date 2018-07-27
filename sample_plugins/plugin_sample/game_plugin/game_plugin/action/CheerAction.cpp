// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/action/CheerAction.h"
#include "game_plugin/command/CheerCommand.h"
#include "game_plugin/component/IndicatorComponent.h"

#include <em5/action/ActionPriority.h>
#include <em5/command/CommandContext.h>
#include <em5/game/animation/AnimationHelper.h>
#include <em5/map/EntityHelper.h>

#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/equipment/EquipmentComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/animation/MeshAnimationChannel.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier CheerAction::ACTION_ID = "user::CheerAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CheerAction::CheerAction() :
		qsf::Action(ACTION_ID),
		mState(STATE_INIT),
		mCheerAnimationChannel(nullptr)
	{
		// Nothing here
	}

	CheerAction::~CheerAction()
	{
		// Nothing here
	}

	void CheerAction::init()
	{
		// Most actions need some parameters or do some preparation work in initialization;
		// this one does not, so this method is quite useless right now, nevermind
		// Remember this function can called in the moment the action is pushed, eg from the command.
		// Often it is necessary to pass the target of the action
	}

	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool CheerAction::onStartup()
	{
		// Most actions need some extra checks to check if the action can still executed correctly. 
		// Everything can happen between given the command and the start of the action
		// It current practice in em5 to check the action requirements with the corresponding command (if existing)

		// Build context
		em5::CommandContext commandContext;
		commandContext.mCaller = &getEntity();
		commandContext.mAllowSelfExecution = true;

		// Call "checkCaller" && "checkContext" of the commando for validation
		CheerCommand* cheerCommand = static_cast<CheerCommand*>(QSFGAME_COMMAND.getCommandManager().getById<CheerCommand>(CheerCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != cheerCommand, "cheerCommand::onStartup(): Could not find cheer command instance", return false);
		if (!(cheerCommand->checkCallerWithoutPriority(*commandContext.mCaller) && cheerCommand->checkContext(commandContext)))
			return false;

		// All checks are passed, we can change settings, like reserve logic for the caller/target or locking for multiplayer

		// Everything all right, start the action
		return true;
	}

	qsf::action::Result CheerAction::updateAction(const qsf::Clock& clock)
	{
		// Keep track of the current action progress via enum states. In actions the transitions are usually linear.
		switch (mState)
		{
			case STATE_INIT:
			{
				// Start the animation
				em5::AnimationHelper animationHelper(getEntity());
				mCheerAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationCheer());
				
				mState = STATE_INDICATOR;
				// Just go on, "updateAction" will get called next game tick again
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_INDICATOR:
			{
				// Create or update the indicator component on our entity, set a new random color
				IndicatorComponent& indicatorComponent = getEntity().getOrCreateComponentSafe<IndicatorComponent>();
				indicatorComponent.setRandomColor();
				indicatorComponent.setIsAnimated(true);

				mState = STATE_CHEER;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CHEER:
			{
				// Here we ask if the animation is finished. Means it is at 100% percent.
				if (mCheerAnimationChannel.valid() && !mCheerAnimationChannel->isFinished())
				{
					// Wait for finish the animation
					return qsf::action::RESULT_CONTINUE;
				}
				
				mState = STATE_END;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_END:
			{
				onAbort();

				// Inform the game about the completed cheering
				// Send a message for everyone how is interesting in
				// The message parameters can contain additional information like the entity id of the main actor of the action
				qsf::MessageParameters parameters;
				parameters.setParameter("EntityId", getEntityId());
				// Use the filter of the message configuration to filter for unneeded information
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration("user::CheerActionFinished", getEntityId()), parameters);

				// Action is over
				return qsf::action::RESULT_DONE;
			}
		}

		// Error
		return qsf::action::RESULT_DONE;
	}

	void CheerAction::onAbort()
	{
		// Empty, just for demonstration
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user

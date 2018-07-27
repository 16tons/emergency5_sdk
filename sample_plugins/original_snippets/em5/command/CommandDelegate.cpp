// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/CommandDelegate.h"
#include "em5/command/Command.h"
#include "em5/command/CommandContext.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/mode/DefaultCommandMode.h"
#include "em5/command/move/MoveCommand.h"
#include "em5/command/fire/AreaExtinguishCommand.h"
#include "em5/command/police/BarrierTape/PlaceBarrierTapeCommand.h"
#include "em5/command/police/SEK/RopingFromHelicopterCommand.h"
#include "em5/command/tech/PlacePersonByFlyingHeliCommand.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/audio/AudioFeedbackHelper.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/player/Player.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/network/multiplayer/client/Client.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/asset/AssetProxy.h>
#include <qsf/map/Entity.h>
#include <qsf/log/LogSystem.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CommandDelegate::CommandDelegate()
	{
		// Fill set with command ids, which starts an command mode (These are not REAL commands...)
		mCommandsWhichActivatesCommandModes.emplace(AreaExtinguishCommand::PLUGINABLE_ID);
		mCommandsWhichActivatesCommandModes.emplace(PlaceBarrierTapeCommand::PLUGINABLE_ID);
		mCommandsWhichActivatesCommandModes.emplace(RopingFromHelicopterCommand::PLUGINABLE_ID);
		mCommandsWhichActivatesCommandModes.emplace(PlacePersonByFlyingHeliCommand::PLUGINABLE_ID);
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::CommandDelegate methods           ]
	//[-------------------------------------------------------]
	bool CommandDelegate::isCallerAllowed(qsf::Entity& caller, const qsf::game::CommandContext& context)
	{
		// Filter out non local player units
		const CommandableComponent* commandableComponent = caller.getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			// In EMERGENCY 5 we have only em5::CommandContext instances
			const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);

			// Check if the command is executed from the same player as to which the unit is assigned
			Player* player = em5CommandContext.mPlayer;
			if (nullptr != player && player->getPlayerIndex() != commandableComponent->getPlayerId())
			{
				return false;
			}
		}

		// Filter out injured persons
		const HealthComponent* healthComponent = caller.getComponent<HealthComponent>();
		if (nullptr != healthComponent && !healthComponent->isMovable())
		{
			return false;
		}
		else
		{
			// Filter out damaged vehicles
			const RoadVehicleComponent* roadVehicleComponent = caller.getComponent<RoadVehicleComponent>();
			if (nullptr != roadVehicleComponent && !roadVehicleComponent->isMovable())
			{
				return false;
			}
		}

		// Everything okay
		return true;
	}

	bool CommandDelegate::isTargetAllowed(const qsf::game::CommandContext& context)
	{
		if (context.hasTargetPosition() && nullptr != context.mTargetEntity)
		{
			qsf::Map& map = context.mTargetEntity->getMap();
			if (!MapHelper(map).isInMapBoundaries(context.getTargetPosition()))
				return false;

			if (!EntityHelper(*context.mTargetEntity).isInMapBoundaries())
			{
				// Terrain and walkable are allowed to be outside (this is the case e.g. in Munich map)
				if (nullptr == context.mTargetEntity->getComponentById("qsf::TerrainComponent") &&
					nullptr == context.mTargetEntity->getComponentById("qsf::WalkableComponent"))
					return false;
			}
		}

		if (nullptr != context.mTargetEntity)
		{
			// In EMERGENCY 5 we have only em5::CommandContext instances
			const CommandContext& em5CommandContext = static_cast<const CommandContext&>(context);

			// If we have a caller use the caller for the check otherwise fallback to the provided player instance (if provided)
			if (nullptr != context.mCaller)
			{
				// Check if the target isn't lock by other player
				if (!EventIdComponent::isUnlockedForTeam(*context.mTargetEntity, *context.mCaller))
				{
					return false;
				}
			}
			else if (nullptr != em5CommandContext.mPlayer)
			{
				// Check if the target isn't lock by other player/team
				if (!EventIdComponent::isUnlockedForTeam(*context.mTargetEntity, em5CommandContext.mPlayer->getTeamId()))
				{
					return false;
				}
			}
		}

		return true;
	}

	bool CommandDelegate::preCommandExecution(const qsf::game::Command& command, const std::vector<qsf::Entity*>& callers, qsf::game::CommandContext& context)
	{
		multiplayer::client::Client* multiplayerClient = EM5_NETWORK.getMultiplayerClient();
		if (nullptr != multiplayerClient)
		{
			// Only send real commands to the host, commands, which are activating an command mode are NOT real commands
			const auto commandIterator = mCommandsWhichActivatesCommandModes.find(command.getId());

			// Check if command is *not* contained in the command mode activator list
			if (commandIterator == mCommandsWhichActivatesCommandModes.end())
			{
				// In case of an multiplayer client all REAL commands are ignored
				multiplayerClient->onCommandExecuted(command, callers, context);
				return false;
			}
		}

		// Allow command execution
		return true;
	}

	bool CommandDelegate::preCommandExecution(const qsf::game::Command& command, qsf::game::CommandContext& context)
	{
		// Emit message "PreCommandExecute" and react to the response
		uint64 callerId = qsf::getUninitialized<uint64>();
		uint64 targetId = callerId;
		if (nullptr != context.mCaller)
		{
			callerId = context.mCaller->getId();
		}
		if (nullptr != context.mTargetEntity)
		{
			targetId = context.mTargetEntity->getId();
		}

		qsf::MessageParameters parameters;
		parameters.setParameter("context", &context);

		bool result = false;
		QSF_MESSAGE.emitMessageWithResponse(qsf::MessageConfiguration(Messages::EM5_PRE_COMMAND_EXECUTE, command.getId(), callerId, targetId), result, parameters);
		if (result)
		{
			return false;
		}

		// Allow command execution
		return true;
	}

	void CommandDelegate::postCommandExecution(const qsf::game::Command& command, const qsf::game::CommandContext& context)
	{
		// Log message
		QSF_LOG_PRINTS(TRACE, "Player executed command: " << command.getName());

		uint64 callerId = qsf::getUninitialized<uint64>();
		uint64 targetId = callerId;
		if (nullptr != context.mCaller)
		{
			callerId = context.mCaller->getId();
		}
		if (nullptr != context.mTargetEntity)
		{
			targetId = context.mTargetEntity->getId();
		}

		// Emit message "PostCommandExecute"
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, command.getId(), callerId, targetId));

		playCommandResponse(command, context);
	}

	qsf::game::CommandMode* CommandDelegate::getDefaultCommandMode()
	{
		return new DefaultCommandMode();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CommandDelegate::playCommandResponse(const qsf::game::Command& command, const qsf::game::CommandContext& context)
	{
		// Only play audio, when the command is not executed by automatism and comes from local player
		const CommandContext* em5CommandContext = dynamic_cast<const CommandContext*>(&context);
		if (nullptr != context.mCaller && nullptr != em5CommandContext && !em5CommandContext->mIsAutomatism && em5CommandContext->mPlayer->isPlayerLocal())
		{
			// Only play the command response, when the command is executed the first time in a row of multiple command execution ("at the same time")
			bool playResponse = EntityHelper(*context.mCaller).canPlayCommandFeedback();

			// Special handling for move command only play it when a certain limit is exceeded
			if (command.getId() == MoveCommand::PLUGINABLE_ID)
			{
				// (fw): This looks a bit strange, but maybe it's correct this way?
				playResponse = !EntityHelper(*context.mCaller).isEntityVisibleOnScreen() && EntityHelper(*context.mCaller).canPlayCommandFeedback();
			}

			if (playResponse && context.mCurrentCallerIndex == 0)
			{
				AudioFeedbackHelper::playCommandResponse(*context.mCaller);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

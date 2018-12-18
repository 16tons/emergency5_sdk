// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/EntityLogicHelper.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/Game.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/Command.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/command/evaluation/CommandEvaluation.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void EntityLogicHelper::buildCommandContext(CommandContext& outCommandContext, qsf::Entity* caller, qsf::Entity* target, const boost::optional<glm::vec3> hitPosition)
	{
		CommandableComponent* commandableComponent = caller->getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			// Automatism is part of the player's action, so give it the player ID of the unit
			outCommandContext.mPlayer = EM5_PLAYERS.getPlayerByIndex(commandableComponent->getPlayerId());
		}

		outCommandContext.mCaller = caller;
		outCommandContext.mTargetEntity = target;
		outCommandContext.mTargetPosition = hitPosition;
		outCommandContext.mIsAutomatism = true;
	}

	bool EntityLogicHelper::internalCheckAndExecuteAutomatism(uint32 commandId, qsf::Entity* caller, qsf::Entity* target, const boost::optional<glm::vec3> hitPosition)
	{
		qsf::game::Command* command = QSFGAME_COMMAND.getCommandManager().getById<qsf::game::Command>(commandId);
		if (nullptr != command)
		{
			CommandContext commandContext;
			buildCommandContext(commandContext, caller, target, hitPosition);

			qsf::game::CommandEvaluation::EntityArray callerEntities;
			callerEntities.push_back(caller);
			return QSFGAME_COMMAND.getCommandEvaluation().processCommand(*command, callerEntities, commandContext);
		}

		// Error!
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

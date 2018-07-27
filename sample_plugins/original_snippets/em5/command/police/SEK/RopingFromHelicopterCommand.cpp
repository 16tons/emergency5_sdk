// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/SEK/RopingFromHelicopterCommand.h"
#include "em5/command/base/ExitVehicleCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/mode/RopingFromHelicopterCommandMode.h"
#include "em5/command/police/SEK/PerformRopeDownFromHelicopterCommand.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandContext.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 RopingFromHelicopterCommand::PLUGINABLE_ID = qsf::StringHash("em5::RopingFromHelicopterCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 RopingFromHelicopterCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool RopingFromHelicopterCommand::allowRoping(qsf::Entity& helicopterEntity)
	{
		// Check if helicopter is in air
		const glm::vec3& position = helicopterEntity.getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();

		// GroundMapQuery fills mGroundHeight with the height of the ground map
		float groundHeight = 0;
		if (!qsf::GroundMapQuery(helicopterEntity.getMap(), GroundMaps::FILTER_DEFAULT).getHeightAt(position, groundHeight))
			return false;

		// Is the helicopter already landed?
		if (groundHeight == position.y)
		{
			return false;
		}

		glm::vec3 groundPosition = position;
		groundPosition.y = groundHeight;

		CollisionHelper collisionsHelper(helicopterEntity.getMap());

		// Don't land in water
		if (collisionsHelper.isInWater(groundPosition, true))
			return false;

		glm::vec3 boxCheck(4.0f, 2.0f, 4.0f);
		qsf::Transform targetTransform(groundPosition);
		if (collisionsHelper.hasCollisionEM3(boxCheck, targetTransform))
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RopingFromHelicopterCommand::RopingFromHelicopterCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Higher priority than the exit vehicle Command
		mPriority = 120;

		// This command is triggered from a HUD element, we need an icon
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "police_sek_rappelfromheli";

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "rappel_from_heli";

		mIconSettings.mTooltip = "EM5_Command_85";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_SEK_HELI_PERSONAL_EXIT";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool RopingFromHelicopterCommand::checkAvailable()
	{
		return true;
	}

	bool RopingFromHelicopterCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		qsf::EntityVectorScratchBuffer entitiesToExit;
		ExitVehicleCommand::getEntitiesToExit(caller, entitiesToExit);
		if (entitiesToExit.empty())
			return false;

		// Only in air
		HelicopterComponent* helicopterComponent = caller.getComponent<HelicopterComponent>();
		if (nullptr == helicopterComponent || !helicopterComponent->isFlying())
			return false;

		return true;
	}

	bool RopingFromHelicopterCommand::checkContext(const qsf::game::CommandContext& context)
	{
		if (nullptr != context.mTargetEntity && context.mTargetEntity->getId() == context.mCaller->getId())
		{
			if (allowRoping(*context.mCaller))
			{
				return true;
			}
		}

		if (nullptr != context.mTargetEntity || context.mTargetPosition.is_initialized())
		{
			return false;
		}

		// Valid. This commando starts an commando mode. This checks if the target position is valid.
		return true;
	}

	void RopingFromHelicopterCommand::execute(const qsf::game::CommandContext& context)
	{
		if (nullptr != context.mTargetEntity && context.mTargetEntity->getId() == context.mCaller->getId())
		{
			// Self target
			PerformRopeDownFromHelicopterCommand::pushCommandToEntity(*context.mCaller, const_cast<qsf::game::CommandContext&>(context));
		}
		else
		{
			// Create a command mode, this has future validation checks
			RopingFromHelicopterCommandMode* ropingFromHelicopterCommandMode = new RopingFromHelicopterCommandMode();
			ropingFromHelicopterCommandMode->initialize(*context.mCaller);
			QSFGAME_COMMAND.setCommandMode(*ropingFromHelicopterCommandMode);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

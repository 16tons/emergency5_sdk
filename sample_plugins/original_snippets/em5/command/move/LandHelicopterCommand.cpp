// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/move/LandHelicopterCommand.h"
#include "em5/command/base/ExitVehicleCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/mode/LandHelicopterCommandMode.h"
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
	const uint32 LandHelicopterCommand::PLUGINABLE_ID = qsf::StringHash("em5::LandHelicopterCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 LandHelicopterCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool LandHelicopterCommand::allowLanding(qsf::Entity& helicopterEntity)
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
	LandHelicopterCommand::LandHelicopterCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// This command is triggered from a HUD element, we need an icon
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "placefromheli";	// Icon looks like landing

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "place_person_by_flying_heli";	// Icon looks like landing

		mIconSettings.mTooltip = "EM5_Command_87";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool LandHelicopterCommand::checkAvailable()
	{
		return true;
	}

	bool LandHelicopterCommand::checkCaller(qsf::Entity& caller)
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

	bool LandHelicopterCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// This command can only be started via button
		if (nullptr != context.mTargetEntity || context.mTargetPosition.is_initialized())
		{
			return false;
		}

		return true;
	}

	void LandHelicopterCommand::execute(const qsf::game::CommandContext& context)
	{
		// Create a command mode, it has future validation checks
		LandHelicopterCommandMode* landHelicopterCommandMode = new LandHelicopterCommandMode();
		landHelicopterCommandMode->initialize(*context.mCaller);
		QSFGAME_COMMAND.setCommandMode(*landHelicopterCommandMode);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/tech/PlacePersonByFlyingHeliCommand.h"
#include "em5/command/tech/PerformPlacePersonByFlyingHeliCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/vehicle/parts/HelicopterHatchComponent.h"
#include "em5/command/mode/PlacePersonByFlyingHeliCommandMode.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"

#include <qsf_game/command/CommandContext.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PlacePersonByFlyingHeliCommand::PLUGINABLE_ID = qsf::StringHash("em5::PlacePersonByFlyingHeliCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PlacePersonByFlyingHeliCommand::ACTION_PRIORITY = action::COMMAND_STD;
	const uint32 PlacePersonByFlyingHeliCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool PlacePersonByFlyingHeliCommand::allowRoping(qsf::Entity& helicopterEntity)
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
	PlacePersonByFlyingHeliCommand::PlacePersonByFlyingHeliCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// This command is triggered from a HUD element, we need an icon
		mIconSettings.mShowAsGuiButton = true;
		mIconSettings.mButtonIconPath = "placefromheli";
		mIconSettings.mTooltip = "EM5_Command_16";

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "place_person_by_flying_heli";

		// Set the help page name
		mHelpPageName = "TUTORIAL_HELPTEXT_INTERACTION_BHC";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool PlacePersonByFlyingHeliCommand::checkAvailable()
	{
		return true;
	}

	bool PlacePersonByFlyingHeliCommand::checkCallerWithoutPriority(qsf::Entity& caller)
	{
		if (!EntityHelper(caller).isHelicopter())
			return false;

		// Check if entity is right and get the helicopter hatch component
		HelicopterComponent* helicopterComponent = caller.getComponent<HelicopterComponent>();
		if (nullptr == helicopterComponent)
			return false;

		qsf::Entity* specialEntity = caller.getMap().getEntityById(helicopterComponent->getSpecialEntity());
		if (nullptr == specialEntity)
			return false;

		HelicopterHatchComponent* helicopterHatchComponent = specialEntity->getComponent<HelicopterHatchComponent>();
		if (nullptr == helicopterHatchComponent)
			return false;

		// An injured person blockade is needed for the action, thats our target
		if (!helicopterHatchComponent->isTransportingInjuredPerson())
			return false;

		return true;
	}

	bool PlacePersonByFlyingHeliCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriorityNoBlocking(caller, action::BLOCKING))
			return false;

		if (!checkCallerWithoutPriority(caller))
			return false;

		return true;
	}

	bool PlacePersonByFlyingHeliCommand::checkContext(const qsf::game::CommandContext& context)
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

	void PlacePersonByFlyingHeliCommand::execute(const qsf::game::CommandContext& context)
	{
		// TODO(mk) Check if Commando modus is already active
		if (nullptr != context.mTargetEntity && context.mTargetEntity->getId() == context.mCaller->getId())
		{
			// Place person on ground
			qsf::game::CommandContext newContext = context;
			float groundHeight = 0.0f;
			qsf::GroundMapQuery(newContext.mTargetEntity->getMap(), GroundMaps::FILTER_MASK_ALL).getHeightAt(newContext.getTargetPosition(), groundHeight);
			newContext.mTargetPosition->y = groundHeight;

			// Self target
			PerformPlacePersonByFlyingHeliCommand::pushCommandToEntity(*context.mCaller, newContext);
		}
		else
		{
			// Create a command mode, this has future validation checks
			PlacePersonByFlyingHeliCommandMode* placePersonByFlyingHeliCommandMode = new PlacePersonByFlyingHeliCommandMode();
			placePersonByFlyingHeliCommandMode->initialize(*context.mCaller);
			QSFGAME_COMMAND.setCommandMode(*placePersonByFlyingHeliCommandMode);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

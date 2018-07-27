// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/SpeakToHostageTakerCommand.h"
#include "em5/action/police/SpeakToHostageTakerAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/command/CommandContext.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/logic/local/police/GangsterInBuildingLogic.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/math/CoordinateSystem.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SpeakToHostageTakerCommand::PLUGINABLE_ID = qsf::StringHash("em5::SpeakToHostageTakerCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 SpeakToHostageTakerCommand::ACTION_PRIORITY = action::BLOCKING;
	const uint32 SpeakToHostageTakerCommand::ACTION_PRIORITY_AUTOMATISM = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SpeakToHostageTakerCommand::SpeakToHostageTakerCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "megaphone";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool SpeakToHostageTakerCommand::checkAvailable()
	{
		return true;
	}

	bool SpeakToHostageTakerCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		return true;
	}

	bool SpeakToHostageTakerCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Target entity needed
		if (nullptr == context.mTargetEntity)
			return false;

		// Check if target is building
		const BuildingComponent* targetBuildingComponent = context.mTargetEntity->getComponent<BuildingComponent>();
		if (nullptr == targetBuildingComponent)
			return false;

		// Target needs gangsters inside
		if (!targetBuildingComponent->isContainingGangsters())
			return false;

		qsf::GameLogicComponent* gameLogicComponent = context.mTargetEntity->getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return false; // only valid for buildings with hostages

		GangsterInBuildingLogic* gangsterInBuildingLogic = gameLogicComponent->getGameLogic<GangsterInBuildingLogic>();
		if (nullptr == gangsterInBuildingLogic)
			return false;

		if (!gangsterInBuildingLogic->getCanSpeakWithHostageTaker() || gangsterInBuildingLogic->getInUseFromSpeaker())
			return false;

		return true;
	}

	void SpeakToHostageTakerCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Check if the same command with the same target is already in the action plan, return in this case without clearing the action plan
		const SpeakToHostageTakerAction* speakToHostageTakerAction = actionComponent.getAction<SpeakToHostageTakerAction>();
		if (nullptr != speakToHostageTakerAction)
		{
			if (speakToHostageTakerAction->getTargetEntityId() == context.mTargetEntity->getId())
			{
				// Don't push the same command twice
				return;
			}
		}

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();

		const uint32 usedActionPriority = (static_cast<const CommandContext&>(context).mIsAutomatism ? ACTION_PRIORITY_AUTOMATISM : ACTION_PRIORITY);

		glm::vec3 position = getSpeaktoHostageTakerPosition(*context.mTargetEntity, *context.mCaller);
		qsf::logic::TargetPoint moveTargetConfig;
		moveTargetConfig.mPosition = position;
		moveTargetConfig.mTolerance = 1.0f;

		actionComponent.pushAction<MoveAction>(action::COMMAND_STD).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig));
		actionComponent.pushAction<SpeakToHostageTakerAction>(usedActionPriority).init(*context.mTargetEntity);
	}

	glm::vec3 SpeakToHostageTakerCommand::getSpeaktoHostageTakerPosition(qsf::Entity& buildingEntity, qsf::Entity& policeEntity)
	{
		BuildingComponent* buildingComponent = buildingEntity.getComponent<BuildingComponent>();

		CollisionHelper collisionHelper(buildingEntity.getMap());

		// Find a position infront of the door, search in 7, 5.5, 4, 2.5 and 1 distance to the door
		float offset = 7.f;
		while (offset > 0)
		{
			glm::vec3 newPosition = buildingComponent->getDoorPosition() + (buildingComponent->getDoorRotation() * qsf::CoordinateSystem::getIn() * offset);
			if (!collisionHelper.hasCollisionEM3(policeEntity, newPosition, 0.5f))
			{
				return newPosition;
			}

			offset -= 1.5f;
		}

		// Fallback, use a normal position search
		glm::vec3 result = buildingComponent->getDoorPosition();
		qsf::Transform transform(result, buildingComponent->getDoorRotation());
		collisionHelper.findFreePosition(policeEntity, transform, 5.0f, 1.0f, 0, result);

		return result;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

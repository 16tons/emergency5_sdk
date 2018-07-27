// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/base/StopGuidePersonCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/StopGuidePersonAction.h"
#include "em5/command/CommandContext.h"
#include "em5/command/mode/DefaultCommandMode.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/placement/WalkableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/renderer/terrain/TerrainComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 StopGuidePersonCommand::PLUGINABLE_ID = qsf::StringHash("em5::StopGuidePersonCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 StopGuidePersonCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StopGuidePersonCommand::StopGuidePersonCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "place_person";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool StopGuidePersonCommand::checkAvailable()
	{
		return true;
	}

	bool StopGuidePersonCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Must be guiding another person
		const uint64 guidedPersonId = EntityHelper(caller).getCarryTarget(EntityHelper::CARRY_PERSON);
		qsf::Entity* guidedEntity = caller.getMap().getEntityById(guidedPersonId);
		if (guidedEntity == nullptr)
			return false;

		// ... but not a contaminated person
		if (EntityHelper(*guidedEntity).isPersonContaminated())
			return false;

		return true;
	}

	bool StopGuidePersonCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// We need a click onto the firefighter himself
		if (!context.mAllowSelfExecution)
		{
			return false;
		}

		return true;
	}

	void StopGuidePersonCommand::execute(const qsf::game::CommandContext& context)
	{
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);

		// Clear plan before the actions are pushed
		actionComponent.clearPlan();
		actionComponent.pushAction<StopGuidePersonAction>(action::BLOCKING);
	}

	bool StopGuidePersonCommand::highlightTarget() const
	{
		// No command target highlighting please
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

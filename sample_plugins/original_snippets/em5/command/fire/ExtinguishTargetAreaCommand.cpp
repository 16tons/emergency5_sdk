// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/ExtinguishTargetAreaCommand.h"
#include "em5/action/fire/AreaExtinguishAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/CommandContext.h"
#include "em5/specs/FireSpecsGroup.h"

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ExtinguishTargetAreaCommand::PLUGINABLE_ID = qsf::StringHash("em5::ExtinguishTargetAreaCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 ExtinguishTargetAreaCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExtinguishTargetAreaCommand::ExtinguishTargetAreaCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// No mouse cursor, no button - command is pushed implicitly by the command mode
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool ExtinguishTargetAreaCommand::checkAvailable()
	{
		// This command will only be pushed explicitly and should never be chosen by some automatism
		return false;
	}

	bool ExtinguishTargetAreaCommand::checkCaller(qsf::Entity& caller)
	{
		// This command will only be pushed explicitly and should thus always have a valid caller
		return true;
	}

	bool ExtinguishTargetAreaCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// This command will only be pushed explicitly and should thus always have a valid context
		return true;
	}

	void ExtinguishTargetAreaCommand::execute(const qsf::game::CommandContext& context)
	{
		// Configuring the areaExtinguishAction
		const FireSpecsGroup& fireSpecsGroup = FireSpecsGroup::getInstanceSafe();
		const float extinguishRadius = fireSpecsGroup.getRadiusCommandoCarExtinguish();
		const qsf::Time extinguishDurration = qsf::Time::fromSeconds(fireSpecsGroup.getDurationCommandoCarExtinguish());
		const float extinguishPower = fireSpecsGroup.getPowerCommandoCar();
		const float coolingPower = fireSpecsGroup.getCoolingCommandoCar();

		qsf::ActionComponent& actionComponent = context.mCaller->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.pushAction<AreaExtinguishAction>(action::BLOCKING).init(context.getTargetPosition(), extinguishDurration, extinguishRadius, extinguishPower, coolingPower);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

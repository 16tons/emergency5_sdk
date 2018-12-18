// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/fire/ExtinguishCoolBaseCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExtinguishCoolBaseCommand::ExtinguishCoolBaseCommand(qsf::game::CommandManager* commandManager, uint32 commandId) :
		Command(commandManager, commandId)
	{
		// To be overwritten in sub-classes
		mIsCoolCommand = false;

		// Priority is at default value

		// No icon settings are set, nor does it start a special command mode. This is a base class
	}

	bool ExtinguishCoolBaseCommand::checkTarget(const qsf::game::CommandContext& context)
	{
		// Don't extinguish yourself
		if (context.mCaller == context.mTargetEntity)
			return false;

		if (mIsCoolCommand)
		{
			// Check the target is able to burn and currently not burning
			const FireComponent* fireComponent = context.mTargetEntity->getComponent<FireComponent>();
			if (nullptr != fireComponent)
			{
				// A fire component can be deactivated (gets excluded from fire simulation update)
				// When deactivated or currently burning the target is not a valid target for this command
				if (!fireComponent->isActive() || fireComponent->isBurning())
					return false;

				// Evaluation for highlighting icons?
				if (static_cast<const CommandContext&>(context).mEvaluationReason == CommandContext::EVALUATIONREASON_SHOWCOMMAND_ABOVE_ENTITY)
				{
					// Show an icon only if there's a bit of heat already above the usual level
					if (fireComponent->getRelativeCombustionEnergy() > 0.75f)
						return false;
				}

				return true;
			}

			const ComplexFireComponent* complexFireComponent = context.mTargetEntity->getComponent<ComplexFireComponent>();
			if (nullptr != complexFireComponent)
			{
				if (complexFireComponent->isBurning())
					return false;

				// Evaluation for highlighting icons?
				if (static_cast<const CommandContext&>(context).mEvaluationReason == CommandContext::EVALUATIONREASON_SHOWCOMMAND_ABOVE_ENTITY)
				{
					// Show an icon only if there's a bit of heat already above the usual level
					if (complexFireComponent->getRelativeCombustionEnergy() > 0.75f)
						return false;
				}

				return true;
			}
		}
		else
		{
			// Check if the target is able to burn, existing of fire component is enough
			const FireComponent* fireComponent = context.mTargetEntity->getComponent<FireComponent>();
			if (nullptr != fireComponent)
			{
				// A fire component can be deactivated (gets excluded from fire simulation update)
				// When deactivated the target is not a valid target for this action
				return (fireComponent->isActive() && fireComponent->isBurning());
			}

			const ComplexFireComponent* complexFireComponent = context.mTargetEntity->getComponent<ComplexFireComponent>();
			if (nullptr != complexFireComponent)
			{
				return complexFireComponent->isBurning();
			}
		}

		// Checks fails
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

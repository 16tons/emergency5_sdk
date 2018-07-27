// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/DisableMegaphoneCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/movement/PersonMagnetComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/command/CommandContext.h>
#include <qsf_game/equipment/EquipmentComponent.h>

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DisableMegaphoneCommand::PLUGINABLE_ID = qsf::StringHash("em5::DisableMegaphoneCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 DisableMegaphoneCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DisableMegaphoneCommand::DisableMegaphoneCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mShowAsGuiButton = false;
		mIconSettings.mMouseIconName = "megaphone_no";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool DisableMegaphoneCommand::checkAvailable()
	{
		return true;
	}

	bool DisableMegaphoneCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Check if equipment matches
		if (!entityHelper.checkIsActiveEquipment(equipment::POLICE_MEGAPHONE))
			return false;

		// get equipment entity
		qsf::Entity* equipmentEntity = entityHelper.getActiveEquipmentEntity();
		if (nullptr == equipmentEntity)
			return false;

		// MagnetComponent should be created and enabled (only check for visualization here to be multiplayer compatible)
		PersonMagnetComponent* personMagnetComponent = equipmentEntity->getComponent<PersonMagnetComponent>();
		if (nullptr == personMagnetComponent || !personMagnetComponent->isVisualizationEnabled())
			return false;

		return true;
	}

	bool DisableMegaphoneCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// Allow self-executed commands
		if (!context.mAllowSelfExecution)
		{
			// We need a target entity
			if (nullptr == context.mTargetEntity)
				return false;

			// Unit has to command itself here
			return context.mCaller == context.mTargetEntity;
		}

		return true;
	}

	void DisableMegaphoneCommand::execute(const qsf::game::CommandContext& context)
	{
		if (nullptr != context.mCaller)
		{
			EntityHelper entityHelper(*context.mCaller);

			qsf::Entity* equipmentEntity = entityHelper.getActiveEquipmentEntity();
			if (nullptr != equipmentEntity)
			{
				PersonMagnetComponent* personMagnetComponent = equipmentEntity->getComponent<PersonMagnetComponent>();
				if (personMagnetComponent)
				{
					personMagnetComponent->disableMagnet();
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/EnableMegaphoneCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/movement/PersonMagnetComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/EM5Helper.h"

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
	const uint32 EnableMegaphoneCommand::PLUGINABLE_ID = qsf::StringHash("em5::EnableMegaphoneCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 EnableMegaphoneCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EnableMegaphoneCommand::EnableMegaphoneCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mShowAsGuiButton = false;
		mIconSettings.mMouseIconName = "megaphone";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool EnableMegaphoneCommand::checkAvailable()
	{
		return true;
	}

	bool EnableMegaphoneCommand::checkCaller(qsf::Entity& caller)
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

		// Get equipment entity
		qsf::Entity* equipmentEntity = entityHelper.getActiveEquipmentEntity();
		if (nullptr == equipmentEntity)
			return false;

		// Magnet component should not be created or not enabled (only check for visualization here to be multiplayer compatible)
		PersonMagnetComponent* personMagnetComponent = equipmentEntity->getComponent<PersonMagnetComponent>();
		if (nullptr != personMagnetComponent && personMagnetComponent->isVisualizationEnabled())
			return false;

		return true;
	}

	bool EnableMegaphoneCommand::checkContext(const qsf::game::CommandContext& context)
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

	void EnableMegaphoneCommand::execute(const qsf::game::CommandContext& context)
	{
		if (nullptr != context.mCaller)
		{
			EntityHelper entityHelper(*context.mCaller);

			qsf::Entity* equipmentEntity = entityHelper.getActiveEquipmentEntity();
			if (nullptr != equipmentEntity)
			{
				const float INFLUENCE_RADIUS = LogicSpecsGroup::getInstanceSafe().getPoliceMegaphoneRadius();
				const int MAX_FOLLOWERS = LogicSpecsGroup::getInstanceSafe().getPoliceMegaphoneMaxAffected();

				PersonMagnetComponent& personMagnetComponent = equipmentEntity->getOrCreateComponentSafe<PersonMagnetComponent>();
				personMagnetComponent.setCarrierEntityId(context.mCaller->getId());
				personMagnetComponent.enableMagnet(INFLUENCE_RADIUS);
				personMagnetComponent.setMaxFollowers(MAX_FOLLOWERS);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

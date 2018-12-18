// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/UsePistolCommand.h"
#include "em5/action/base/HuntAndShootAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/time/Time.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 UsePistolCommand::PLUGINABLE_ID = qsf::StringHash("em5::UsePistolCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 UsePistolCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UsePistolCommand::UsePistolCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "use_pistol";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool UsePistolCommand::checkAvailable()
	{
		return true;
	}

	bool UsePistolCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Check if equipment matches
		if (!entityHelper.checkIsActiveEquipment(equipment::POLICE_PISTOL) && !entityHelper.checkIsActiveEquipment(equipment::POLICE_SEK_PISTOL))
			return false;

		// Not already carrying someone?
		if (entityHelper.isCarryingAnotherPerson())
			return false;

		return true;
	}

	bool UsePistolCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// We need a target entity
		if (nullptr == context.mTargetEntity)
			return false;

		// Check for the person component
		EntityHelper entityHelper(*context.mTargetEntity);
		if (entityHelper.isGangsterPerson())
		{
			if (entityHelper.isPersonInjured())
				return false;

			return true;
		}

		// Check for a gangster vehicles
		if (entityHelper.isGangsterRoadVehicle())
		{
			const float GIVE_UP_VEHICLE_PURSUIT = LogicSpecsGroup::getInstanceSafe().getSEKGiveUpVehiclePursuit();
			const float distance = qsf::game::DistanceHelper::getDistance(*context.mCaller, *context.mTargetEntity);
			if (distance > GIVE_UP_VEHICLE_PURSUIT)
				return false;

			return true;
		}

		return false;
	}

	void UsePistolCommand::execute(const qsf::game::CommandContext& context)
	{
		// TODO(mk) Add check if action is currently executed on the same target

		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		actionComponent.clearPlan();

		weapon::WeaponConfiguration weaponConfiguration;
		weaponConfiguration.mWeaponType = weapon::PISTOL;
		weaponConfiguration.mDamageHealth = LogicSpecsGroup::getInstanceSafe().getSEKPistolHealthDamage();
		weaponConfiguration.mDamageVehicle = LogicSpecsGroup::getInstanceSafe().getSEKPistolVehicleDamage();
		weaponConfiguration.mShootRange = LogicSpecsGroup::getInstanceSafe().getSEKPistolShootRange();
		weaponConfiguration.mTimeToAim = qsf::Time::fromSeconds(0.5f);
		weaponConfiguration.mTimeToWaitAfterShot = qsf::Time::fromSeconds(0.7f);
		actionComponent.pushAction<HuntAndShootAction>(ACTION_PRIORITY).init(*context.mTargetEntity, weaponConfiguration);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

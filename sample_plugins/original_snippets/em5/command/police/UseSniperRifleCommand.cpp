// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/UseSniperRifleCommand.h"
#include "em5/action/base/HuntAndShootAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandContext.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 UseSniperRifleCommand::PLUGINABLE_ID = qsf::StringHash("em5::UseSniperRifleCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 UseSniperRifleCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UseSniperRifleCommand::UseSniperRifleCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mMouseIconName = "use_sniper_rifle";
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool UseSniperRifleCommand::checkAvailable()
	{
		return true;
	}

	bool UseSniperRifleCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Check if equipment matches
		if (!entityHelper.checkIsActiveEquipment(equipment::POLICE_SNIPERRIFLE))
			return false;

		// Not already carrying someone?
		if (entityHelper.isCarryingAnotherPerson())
			return false;

		return true;
	}

	bool UseSniperRifleCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// We need a target entity
		if (nullptr == context.mTargetEntity)
			return false;

		// Check for the person component
		const PersonComponent* targetPersonComponent = context.mTargetEntity->getComponent<PersonComponent>();
		if (nullptr != targetPersonComponent)
		{
			// Target person has to be a gangster
			if (!targetPersonComponent->getGangsterFlag())
				return false;

			// target person can't be injured
			if (EntityHelper(*context.mTargetEntity).isPersonInjured())
				return false;

			return true;
		}

		// Check for a gangster vehicles
		if (EntityHelper(*context.mTargetEntity).isGangsterRoadVehicle())
		{
			const float RIFLE_SHOOT_RANGE = LogicSpecsGroup::getInstanceSafe().getSEKRifleShootRange();
			const float distance = qsf::game::DistanceHelper::getDistance(*context.mCaller, *context.mTargetEntity);
			if (distance > RIFLE_SHOOT_RANGE)
				return false;

			return true;
		}

		return false;
	}

	void UseSniperRifleCommand::execute(const qsf::game::CommandContext& context)
	{
		// Access the caller's action plan
		qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
		actionComponent.clearPlan();

		weapon::WeaponConfiguration weaponConfiguration;
		weaponConfiguration.mWeaponType = weapon::RIFLE;
		weaponConfiguration.mDamageHealth = LogicSpecsGroup::getInstanceSafe().getSEKRifleHealthDamage();
		weaponConfiguration.mDamageVehicle = LogicSpecsGroup::getInstanceSafe().getSEKRifleVehicleDamage();
		weaponConfiguration.mShootRange = LogicSpecsGroup::getInstanceSafe().getSEKRifleShootRange();
		weaponConfiguration.mTimeToAim = qsf::Time::fromSeconds(1.0f);
		weaponConfiguration.mTimeToWaitAfterShot = qsf::Time::fromSeconds(1.0f);
		actionComponent.pushAction<HuntAndShootAction>(ACTION_PRIORITY).init(*context.mTargetEntity, weaponConfiguration);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

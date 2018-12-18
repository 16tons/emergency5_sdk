// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/drone/FireDroneLogic.h"
#include "em5/action/drone/LandDroneAction.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/effect/FireDroneScanComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/drone/CallBackDroneCommand.h"
#include "em5/command/move/MoveCommand.h"
#include "em5/game/units/OrderInfo.h"
#include "em5/game/units/OrderInfoManager.h"
#include "em5/game/Game.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/physics/collision/CollisionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 FireDroneLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::FireDroneLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FireDroneLogic::FireDroneLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool FireDroneLogic::onStartup()
	{
		// Set the drone transporter entity for the fire drone
		qsf::LinkComponent* linkComponent = getEntity()->getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			mTransporterEntity = static_cast<qsf::Entity*>(linkComponent->getParentPrototype());
			if (mTransporterEntity.valid())
			{
				// Help the commandable component to find the right player/information about the drone entity
				CommandableComponent* commandableComponent = getEntity()->getComponent<CommandableComponent>();
				CommandableComponent* parentCommandableComponent = mTransporterEntity->getComponent<CommandableComponent>();
				if (nullptr != commandableComponent && nullptr != parentCommandableComponent)
				{
					commandableComponent->setPlayerId(parentCommandableComponent->getPlayerId());
					commandableComponent->setUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE);

					OrderInfo* orderInfo = EM5_GAME.getOrderInfoManager().findElement(CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE);
					if (nullptr != orderInfo)
					{
						commandableComponent->setMiniMapIconType(orderInfo->getMiniMapIconType());
					}
				}
			}
		}

		// Its important to stop the swing effect for a good visualization of the drone effect
		HelicopterComponent* helicopterComponent = getEntity()->getComponent<HelicopterComponent>();
		if (nullptr != helicopterComponent)
		{
			helicopterComponent->stopSwing();
		}

		mOnPreMoveMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, MoveCommand::PLUGINABLE_ID, getEntityId()), boost::bind(&FireDroneLogic::onPreMoveMessage, this, _1));
		mOnEndMoveMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_FINISH_MOVE_ACTION, getEntityId()), boost::bind(&FireDroneLogic::onEndMoveMessage, this, _1));
		if (mTransporterEntity.valid())
		{
			mOnCallBackDroneMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, CallBackDroneCommand::PLUGINABLE_ID, mTransporterEntity->getId()), boost::bind(&FireDroneLogic::onPreMoveMessage, this, _1));

			// Deactivate effect if the drone transporter is broken
			mOnTransporterStartBurningProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_FIRECOMPONENT_START_BURNING, mTransporterEntity->getId()), boost::bind(&FireDroneLogic::onPreMoveMessage, this, _1));
		}

		// Remove collision box
		qsf::CollisionComponent* collisionComponent = getEntity()->getComponent<qsf::CollisionComponent>();
		if (nullptr != collisionComponent)
			collisionComponent->setActive(false);

		return true;
	}

	void FireDroneLogic::onShutdown()
	{
		// Unregister jobs
		mOnPreMoveMessageProxy.unregister();
		mOnEndMoveMessageProxy.unregister();
		mOnCallBackDroneMessageProxy.unregister();
		mOnTransporterStartBurningProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void FireDroneLogic::onPreMoveMessage(const qsf::MessageParameters& parameters)
	{
		// Deactivate fire drone scan
		FireDroneScanComponent* fireDroneScanComponent = getEntity()->getComponent<FireDroneScanComponent>();
		if (nullptr != fireDroneScanComponent)
		{
			fireDroneScanComponent->setActive(false);
		}
	}

	void FireDroneLogic::onEndMoveMessage(const qsf::MessageParameters& parameters)
	{
		if (mTransporterEntity.valid())
		{
			const bool isDroneLanding = (nullptr != mTransporterEntity->getOrCreateComponentSafe<qsf::ActionComponent>().getAction<LandDroneAction>());

			// Activate fire drone scan if it stays in air
			FireDroneScanComponent* fireDroneScanComponent = getEntity()->getComponent<FireDroneScanComponent>();
			if (nullptr != fireDroneScanComponent)
			{
				fireDroneScanComponent->setActive(isDroneTransporterIntact() && !isDroneLanding);
			}
		}
	}

	bool FireDroneLogic::isDroneTransporterIntact() const
	{
		if (mTransporterEntity.valid())
		{
			// Check if vehicle is damaged
			EntityHelper entityHelper(mTransporterEntity);
			if (!entityHelper.isBurning() && !entityHelper.isBurned() && !entityHelper.isVehicleDestroyed())
			{
				// Vehicle is not burning (maybe damaged, thats ok)
				return true;
			}
		}

		// Entity is properly deleted
		return false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

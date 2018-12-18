// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/drone/DroneTransportLogic.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/game/Game.h"
#include "em5/gui/hud/SEKDroneScanWindow.h"
#include "em5/gui/IngameHud.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/plugin/Messages.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/component/MapPropertiesComponent.h>
#include <qsf/physics/collision/CollisionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DroneTransportLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::DroneTransportLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DroneTransportLogic::DroneTransportLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID),
		mIsDroneUnitedWithTransporter(true)
	{
		// Nothing here
	}

	qsf::Entity* DroneTransportLogic::getDroneEntity() const
	{
		return mDroneEntity.get();
	}

	bool DroneTransportLogic::getIsDroneUnitedWithLeader() const
	{
		return mIsDroneUnitedWithTransporter;
	}

	void DroneTransportLogic::setIsDroneUnitedWithLeader(bool value)
	{
		mIsDroneUnitedWithTransporter = value;

		if (EntityHelper(*getEntity()).isUnit(CommandableComponent::UNITTAG_POLICE_DRONE_TRANSPORT))
		{
			// Force a GUI update
			onChangeSelectionSEKDroneTransporter(qsf::MessageParameters());
		}
	}

	const glm::vec3& DroneTransportLogic::getLocalDronePodPosition() const
	{
		return mLocalDronePodPosition;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool DroneTransportLogic::onStartup()
	{
		RoadVehicleComponent* roadVehicleComponent = getEntity()->getComponent<RoadVehicleComponent>();
		if (nullptr != roadVehicleComponent)
		{
			mDroneEntity = getMap().getEntityById(roadVehicleComponent->getSpecialEntity());
			if (mDroneEntity.valid())
			{
				// Initially saving the position of the drone
				qsf::LinkComponent* linkComponent = mDroneEntity->getComponent<qsf::LinkComponent>();
				if (nullptr != linkComponent)
				{
					mLocalDronePodPosition = linkComponent->getLocalPosition();
				}

				// Remove collision box of drone
				qsf::CollisionComponent* collisionComponent = mDroneEntity->getComponent<qsf::CollisionComponent>();
				if (nullptr != collisionComponent)
					collisionComponent->setActive(false);
			}

			// If the transporter starts burning, destroy the drone
			mOnStartBurningProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_FIRECOMPONENT_START_BURNING, getEntityId()), boost::bind(&DroneTransportLogic::onTransporterStartsBurning, this, _1));

			// Show or hide the SEK drone window, depends if the unit is selected
			mOnSelectionChangeProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION), boost::bind(&DroneTransportLogic::onChangeSelectionSEKDroneTransporter, this, _1));
		}

		return true;
	}

	void DroneTransportLogic::onShutdown()
	{
		// Unregister jobs
		mOnStartBurningProxy.unregister();
		mOnSelectionChangeProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DroneTransportLogic::onTransporterStartsBurning(const qsf::MessageParameters& parameters)
	{
		if (mDroneEntity.valid())
		{
			qsf::ActionComponent& actionComponent = mDroneEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();

			// If drone is in the air, fly out of map
			if (!mIsDroneUnitedWithTransporter)
			{
				qsf::logic::TargetPoint moveTargetConfig;
				moveTargetConfig.mPosition = calcClosestPointToBorders(mDroneEntity.getSafe());
				actionComponent.pushAction<MoveAction>(action::BLOCKING).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig));
			}

			// To avoid starting/landing drones without transporter we destroy every drone
			actionComponent.pushAction<DisappearAction>(action::BLOCKING);
		}
	}

	void DroneTransportLogic::onChangeSelectionSEKDroneTransporter(const qsf::MessageParameters& parameters)
	{
		if (!EntityHelper(*getEntity()).isUnit(CommandableComponent::UNITTAG_POLICE_DRONE_TRANSPORT))
		{
			// Currently we haven't this unit identifier at startup(), so register it and unregister it for every following unit
			mOnSelectionChangeProxy.unregister();
			return;
		}

		// Check if vehicle is intact
		if (EntityHelper(*getEntity()).isRoadVehicleDamaged() || !mDroneEntity.valid())
			return;

		// Support multiple drone transporter on the map, maybe cycle through the full selection and search for droneTransportLogic
		bool isSelected = false;
		for (uint64 entityId : EM5_GAME.getSelectionManager().getSelectedIdSet())
		{
			qsf::Entity* selectedEntity = getEntity()->getMap().getEntityById(entityId);
			if (nullptr != selectedEntity)
			{
				// Check if the selected Entity has a drone transport logic
				EntityHelper entityHelper(*selectedEntity);
				qsf::GameLogicComponent* gameLogicComponent = selectedEntity->getComponent<qsf::GameLogicComponent>();
				if (nullptr != gameLogicComponent &&
					!entityHelper.isRoadVehicleDamaged() && entityHelper.isUnit(CommandableComponent::UNITTAG_POLICE_DRONE_TRANSPORT))
				{
					// Check if the selected entity is a drone transport with unlinked(started) drone
					DroneTransportLogic* droneTransportLogic = gameLogicComponent->getGameLogic<DroneTransportLogic>();
					if (nullptr != droneTransportLogic && !droneTransportLogic->getIsDroneUnitedWithLeader())
					{
						isSelected = true;
						break;
					}
				}
			}
		}

		// Toggle visibility of the drone SEK scan window
		SEKDroneScanWindow& sekDroneScanWindow = EM5_GUI.getIngameHud().getSEKDroneScanWindow();
		if (isSelected)
		{
			sekDroneScanWindow.showFocused();
			sekDroneScanWindow.setCameraDrone(mDroneEntity.getSafe());
		}
		else
		{
			sekDroneScanWindow.hide();
		}
	}

	glm::vec3 DroneTransportLogic::calcClosestPointToBorders(qsf::Entity& entity)
	{
		qsf::TransformComponent& transformComponent = entity.getOrCreateComponentSafe<qsf::TransformComponent>();
		glm::vec3 position = transformComponent.getPosition();
		glm::vec3 result = position;

		// Get the map boundaries via the core entity from the map
		const qsf::Entity& coreEntity = entity.getMap().getCoreEntity();
		qsf::MapPropertiesComponent* mapPropertiesComponent = coreEntity.getComponent<qsf::MapPropertiesComponent>();
		if (nullptr != mapPropertiesComponent)
		{
			// The position we push can't be exactly on the border, we need a small offset. i assume, less then 1m is ok, but take more for sure
			static const float DISTANCE_TO_MAP_BORDER = 5.0f;

			glm::vec3 topLeftPosition = mapPropertiesComponent->getMapBoundaryTopLeft();
			glm::vec3 bottomRightPosition = mapPropertiesComponent->getMapBoundaryBottomRight();

			float distanceClosestPoint = glm::abs(topLeftPosition.x - position.x);
			result = glm::vec3(topLeftPosition.x + DISTANCE_TO_MAP_BORDER, position.y, position.z);

			const float distanceRight = glm::abs(bottomRightPosition.x - position.x);
			if (distanceRight < distanceClosestPoint)
			{
				distanceClosestPoint = distanceRight;
				result = glm::vec3(bottomRightPosition.x - DISTANCE_TO_MAP_BORDER, position.y, position.z);
			}

			const float distanceTop = glm::abs(topLeftPosition.z - position.z);
			if (distanceTop < distanceClosestPoint)
			{
				distanceClosestPoint = distanceTop;
				result = glm::vec3(position.x, position.y, topLeftPosition.z + DISTANCE_TO_MAP_BORDER);
			}

			const float distanceBottom = glm::abs(bottomRightPosition.z - position.z);
			if (distanceBottom < distanceClosestPoint)
			{
				// distanceClosestPoint = distanceBottom; // We're the last, so no update required
				result = glm::vec3(position.x, position.y, bottomRightPosition.z -DISTANCE_TO_MAP_BORDER);
			}
		}

		return result;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

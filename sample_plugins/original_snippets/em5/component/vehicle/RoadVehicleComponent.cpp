// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/vehicle/parts/WaterCannonComponent.h"
#include "em5/component/vehicle/parts/DLKLadderComponent.h"
#include "em5/component/vehicle/parts/SupportLegsComponent.h"
#include "em5/component/vehicle/parts/VehicleCraneComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/component/equipment/InventoryComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/action/move/MoveAction.h"
#include "em5/game/collision/CollisionTypes.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/fire/component/HydrantComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/plugin/Jobs.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/environment/weather/WeatherComponent.h>
#include <qsf_game/environment/weather/WeatherState.h>

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/job/JobArguments.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Random.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/base/ScratchBuffer.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 RoadVehicleComponent::COMPONENT_ID		= qsf::StringHash("em5::RoadVehicleComponent");
	const uint32 RoadVehicleComponent::VEHICLE_WHEELS	= qsf::StringHash("VehicleWheels");
	const uint32 RoadVehicleComponent::VEHICLE_LIGHTS	= qsf::StringHash("VehicleLights");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	const std::vector<uint64>& RoadVehicleComponent::getVehicleWheelIdsByType(WheelComponent::WheelType wheelType) const
	{
		if (mWheelTypeToEntityIdsMap.empty())
		{
			precalculateWheelsToEntitiesMap();
		}
		auto searchResult = mWheelTypeToEntityIdsMap.find(wheelType);
		if (searchResult != mWheelTypeToEntityIdsMap.end())
		{
			return searchResult->second;
		}

		// Didn't find entities with this type
		return EMPTY_ENTITY_VECTOR;
	}

	const std::vector<uint64>& RoadVehicleComponent::getVehicleLightIdsByType(qsf::game::LightControllerComponent::LightPosition lightPosition) const
	{
		if (!mLightTypeToEntityIdsMapInitialized)
		{
			precalculateLightsToEntitiesMap();
		}

		if (!mLightTypeToEntityIdsMap.empty())
		{
			auto searchResult = mLightTypeToEntityIdsMap.find(lightPosition);
			if (searchResult != mLightTypeToEntityIdsMap.end())
			{
				return searchResult->second;
			}
		}

		// Didn't find entities with this type
		return EMPTY_ENTITY_VECTOR;
	}

	void RoadVehicleComponent::updateLightStates()
	{
		setHeadLightState(mIsHeadLightActive, true);
		setBrakeLightState(mIsBrakeLightActive, true);
		setBlueLightState(mBlueLightActive, true);
		setBlinkerState(mBlinkerState, true);
	}

	void RoadVehicleComponent::setBlinkerState(BlinkerState blinkerState, bool force)
	{
		// State change?
		if (mBlinkerState != blinkerState || force)
		{
			// Backup the given value
			mBlinkerState = blinkerState;

			{ // Update blinker lights
				const bool leftBlinkerState = (mBlinkerState == BLINKERS_LEFT_ON || mBlinkerState == BLINKERS_BOTH_ON);
				const std::vector<uint64>& leftBlinkerIds = getVehicleLightIdsByType(qsf::game::LightControllerComponent::LIGHTPOSITION_LEFT_BLINKER);
				updateLightsList(leftBlinkerIds, qsf::Time::ZERO, true, leftBlinkerState, force);
			}

			{ // Update blinker lights
				const bool rightBlinkerState = (mBlinkerState == BLINKERS_RIGHT_ON || mBlinkerState == BLINKERS_BOTH_ON);
				const std::vector<uint64>& rightBlinkerIds = getVehicleLightIdsByType(qsf::game::LightControllerComponent::LIGHTPOSITION_RIGHT_BLINKER);
				updateLightsList(rightBlinkerIds, qsf::Time::ZERO, true, rightBlinkerState, force);
			}
		}
	}

	void RoadVehicleComponent::setHeadLightState(bool enabled, bool force)
	{
		// State change?
		if (mIsHeadLightActive != enabled || force)
		{
			// Backup the given value
			mIsHeadLightActive = enabled;

			// Update lights
			const std::vector<uint64>& headLightIds = getVehicleLightIdsByType(qsf::game::LightControllerComponent::LIGHTPOSITION_HEAD);
			updateLightsList(headLightIds, qsf::Time::ZERO, false, mIsHeadLightActive, force);
		}
	}

	void RoadVehicleComponent::setBrakeLightState(bool enabled, bool force)
	{
		// State change?
		if (mIsBrakeLightActive != enabled || force)
		{
			// Backup the given value
			mIsBrakeLightActive = enabled;

			// Update lights
			const std::vector<uint64>& brakeLightIds = getVehicleLightIdsByType(qsf::game::LightControllerComponent::LIGHTPOSITION_BRAKE);
			updateLightsList(brakeLightIds, qsf::Time::ZERO, false, mIsBrakeLightActive, force);
		}
	}

	void RoadVehicleComponent::setBlueLightState(bool enabled, bool force)
	{
		// State change?
		if (mBlueLightActive != enabled || force)
		{
			// Backup the given value
			mBlueLightActive = enabled;

			// Update lights
			const std::vector<uint64>& blueLightIds = getVehicleLightIdsByType(qsf::game::LightControllerComponent::LIGHTPOSITION_BLUE);
			updateLightsList(blueLightIds, qsf::Time::ZERO, false, mBlueLightActive, force);
		}
	}

	void RoadVehicleComponent::setEmergencyFlasher(bool enabled)
	{
		setBlinkerState(enabled ? BLINKERS_BOTH_ON : BLINKERS_OFF);
	}

	WaterCannonComponent* RoadVehicleComponent::getWaterCannonComponent() const
	{
		// Ask all linked childs for component
		const qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		return (nullptr != linkComponent) ? linkComponent->getComponentFromEntityOrLinkedChild<WaterCannonComponent>() : nullptr;
	}

	void RoadVehicleComponent::disableVehicleHydrants()
	{
		qsf::LinkComponent* linkComponent = getEntity().getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			for (auto& childComponent : linkComponent->getChildLinks())
			{
				HydrantComponent* hydrantComponent = childComponent->getComponentFromEntityOrLinkedChild<HydrantComponent>();
				if (nullptr != hydrantComponent)
				{
					hydrantComponent->setActive(false);
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	void RoadVehicleComponent::onSetIntact()
	{
		// Call the base implementation
		VehicleComponent::onSetIntact();

		// Enable simulation job
		if (isSimulating())
		{
			updateLightStates();

			if (!mSimulationJobProxy.isValid())
			{
				registerSimulationJobProxy();
			}
		}

		// Show blue-lights in editor
		if (!isSimulating())
		{
			setBlueLightState(true);
			setHeadLightState(true);
		}
	}

	void RoadVehicleComponent::onSetDamagedLevel1()
	{
		// Call the base implementation
		VehicleComponent::onSetDamagedLevel1();

		// Enable simulation job
		if (isSimulating() && !mSimulationJobProxy.isValid())
		{
			registerSimulationJobProxy();
		}

		// Show blue-lights in editor
		if (isSimulating())
		{
			sendRoadVehicleDamagedMessage();

			updateLightStates();
		}
		else
		{
			setBlueLightState(true);
			setHeadLightState(true);
		}
	}

	void RoadVehicleComponent::onSetDamagedLevel2()
	{
		// Call the base implementation
		VehicleComponent::onSetDamagedLevel2();

		// Disable simulation job
		if (isSimulating())
		{
			mSimulationJobProxy.unregister();
		}

		// Turn off the lights
		setBlueLightState(false, true);
		setHeadLightState(false, true);
		setBrakeLightState(false, true);
		setBlinkerState(BlinkerState::BLINKERS_OFF, true);

		if (isSimulating())
		{
			mIsCommandable = false;
			sendRoadVehicleDamagedMessage();

			disableVehicleHydrants();

			// Make it immovable
			EntityHelper(getEntity()).makeImmovable();
			getEntity().destroyComponent<InventoryComponent>();
			StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE, !getCanReallyNotBeTowed());
		}
	}

	void RoadVehicleComponent::onSetDestroyed()
	{
		// Call the base implementation
		VehicleComponent::onSetDestroyed();

		// Disable simulation job
		if (isSimulating())
		{
			mSimulationJobProxy.unregister();
		}

		// Turn off the lights
		setBlueLightState(false, true);
		setHeadLightState(false, true);
		setBrakeLightState(false, true);
		setBlinkerState(BlinkerState::BLINKERS_OFF, true);

		qsf::Entity& entity = getEntity();
		if (isSimulating())
		{
			FireComponent* fireComponent = entity.getComponent<FireComponent>();
			if (nullptr != fireComponent)
			{
				// To avoid crashes after setting the property in the editor, use pointer here, instead of reference
				const EventIdComponent* eventIdComponent = entity.getComponent<EventIdComponent>();
				FreeplayEvent* freeplayEvent = nullptr;
				if (nullptr != eventIdComponent)
				{
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_VEHICLE_DESTROYED, getEntityId()));
					freeplayEvent = eventIdComponent->getFreeplayEvent();
				}

				// Always destroy the fire component, vehicles can burn only once, doesn't matter how intensive the extinguish process was
				fireComponent->setFireStateByEvent(FireComponent::FIRESTATE_DESTROYED, freeplayEvent);
			}

			sendRoadVehicleDamagedMessage();

			disableVehicleHydrants();

			// Make it immovable
			EntityHelper(entity).makeImmovable();
			StatusOverlayComponent::showIcon(entity, StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE, !getCanReallyNotBeTowed());
			entity.destroyComponent<InventoryComponent>();
		}
	}

	void RoadVehicleComponent::onFireStarted(FreeplayEvent* freeplayEvent)
	{
		// TODO(fw): Check if vehicle is not parking
		{
			personsFleeFromFire(mCanSpawnDriver);
			mCanSpawnDriver = false;
		}

		// Call base implementation
		VehicleComponent::onFireStarted(freeplayEvent);
	}

	void RoadVehicleComponent::onExplosion(FireComponent* fireComponent)
	{
		VehicleComponent::onExplosion(fireComponent);

		if (nullptr != fireComponent)
		{
			const EventIdComponent* eventIdComponent = getEntity().getComponent<EventIdComponent>();
			FreeplayEvent* freeplayEvent = nullptr;
			if (nullptr != eventIdComponent)
			{
				freeplayEvent = eventIdComponent->getFreeplayEvent();
			}

			// Persons in the vehicle are dead now
			placeAllPassengerDeadAroundVehicle(freeplayEvent);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void RoadVehicleComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base class implementation
		VehicleComponent::serialize(serializer);

		serializer.serialize(mVehicleWheelIds);
		serializer.serialize(mVehicleLightIds);

		serializer.serialize(mSpecialEntityId);

		serializer.serialize(mFrontWheelDirection);

		serializer.serializeAs<uint8>(mBlinkerState);
		serializer.serialize(mBlinkerTime);
		serializer.serialize(mBlueLightActive);
		serializer.serialize(mBlueLightTime);
		serializer.serialize(mIsBrakeLightActive);
		serializer.serialize(mBrakeLightTimeout);

		serializer.serialize(mIsHeadLightActive);

		serializer.serialize(mSwingTimeout);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	bool RoadVehicleComponent::onStartup()
	{
		// Call base class implementation
		if (!VehicleComponent::onStartup())
			return false;

		const qsf::TransformComponent* transformComponent = getPrototype().getComponent<qsf::TransformComponent>();
		if (nullptr != transformComponent)
		{
			// Initialize angle of vehicle to calculate rotation
			mChainAngle = qsf::EulerAngles::quaternionToEuler(transformComponent->getRotation()).x;
		}

		// Initialize wheels
		for (uint64 wheelEntityId : mVehicleWheelIds)
		{
			initializeWheel(wheelEntityId);
		}

		mIsCommandable = EntityHelper(getEntity()).isCommandable();

		// Done
		return true;
	}

	void RoadVehicleComponent::onShutdown()
	{
		// Call base class implementation
		VehicleComponent::onShutdown();
	}

	void RoadVehicleComponent::onSetSimulating(bool simulating)
	{
		// Call base class implementation
		VehicleComponent::onSetSimulating(simulating);

		if (simulating)
		{
			// Fill component pointers
			mMovableComponent = getEntity().getComponent<qsf::MovableComponent>();

			// Disable all lights
			const qsf::Map& map = getEntity().getMap();
			for (uint64 lightEntityId : mVehicleLightIds)
			{
				const qsf::Entity* lightEntity = map.getEntityById(lightEntityId);
				if (nullptr != lightEntity)
				{
					qsf::game::LightControllerComponent* lightControllerComponent = lightEntity->getComponent<qsf::game::LightControllerComponent>();
					if (nullptr != lightControllerComponent)
					{
						lightControllerComponent->setActive(false);
					}
				}
			}

			// Register jobs
			registerSimulationJobProxy();
			registerMovableObjectRenderingListener();
		}
		else
		{
			// Unregister jobs
			mSimulationJobProxy.unregister();
			unregisterMovableObjectRenderingListener();
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::VehicleComponent methods       ]
	//[-------------------------------------------------------]
	void RoadVehicleComponent::restoreEntitiesFromPrefab(const qsf::Prototype& prototype, std::unordered_map<uint64, uint64>& prototypeIdMap)
	{
		const qsf::Entity& entity = getEntity();
		qsf::Map& map = entity.getMap();

		const RoadVehicleComponent* prototypeRoadVehicleComponent = prototype.getComponent<RoadVehicleComponent>();
		const qsf::LinkComponent* prototypeLinkComponent = prototype.getComponent<qsf::LinkComponent>();

		// Restore the fire entities from the prefab. Used for combined vehicles to activate fire child entities.
		FireComponent* prototypeFireComponent = prototype.getComponent<FireComponent>();
		FireComponent* currentFireComponent = getEntity().getComponent<FireComponent>();
		if (nullptr != prototypeFireComponent && nullptr != currentFireComponent && nullptr != prototypeLinkComponent)
		{
			// Update fire material
			// HACK: If there's an edited EM20 fire material in this entity instance, don't change to the prefab's fire material
			if (!boost::starts_with(currentFireComponent->getFireMaterialName(), "em20_"))
			{
				currentFireComponent->setFireMaterialByName(prototypeFireComponent->getFireMaterialName());
			}

			// Fire
			cloneEntitiesFromPrefab(map, entity, prototypeFireComponent->FireParticlesArray.getVariable(), currentFireComponent->FireParticlesArray.getVariable(), *prototypeLinkComponent, prototypeIdMap);

			// Fire entities should restore and linked
			for (uint64 fireEntityId : currentFireComponent->FireParticlesArray.getVariable())
			{
				prototypeIdMap.emplace(fireEntityId, fireEntityId);
			}
		}

		if (nullptr != prototypeRoadVehicleComponent && nullptr != prototypeLinkComponent)
		{
			// Additional components which need to be restored
			std::vector<uint32> componentIds;
			componentIds.reserve(5);
			componentIds.push_back(WaterCannonComponent::COMPONENT_ID);
			componentIds.push_back(DlkLadderComponent::COMPONENT_ID);
			componentIds.push_back(SupportLegsComponent::COMPONENT_ID);
			componentIds.push_back(VehicleCraneComponent::COMPONENT_ID);
			componentIds.push_back(qsf::MeshComponent::COMPONENT_ID);

			cloneEntitiesFromPrefabByComponentId(map, entity, componentIds, *prototypeLinkComponent, prototypeIdMap);

			// Doors (VehicleComponent)
			mVehicleDoorsIds.clear();	// Clear the entities in the slot
			cloneEntitiesFromPrefab(map, entity, prototypeRoadVehicleComponent->mVehicleDoorsIds, mVehicleDoorsIds, *prototypeLinkComponent, prototypeIdMap);
			precalculateDoorsToEntitiesMap();

			// Lights
			mVehicleLightIds.clear();	// Clear the entities in the slot
			cloneEntitiesFromPrefab(map, entity, prototypeRoadVehicleComponent->mVehicleLightIds, mVehicleLightIds, *prototypeLinkComponent, prototypeIdMap);
			precalculateLightsToEntitiesMap();

			// Wheels
			mVehicleWheelIds.clear();	// Clear the entities in the slot
			cloneEntitiesFromPrefab(map, entity, prototypeRoadVehicleComponent->mVehicleWheelIds, mVehicleWheelIds, *prototypeLinkComponent, prototypeIdMap);
			precalculateWheelsToEntitiesMap();

			// Initialize wheels
			for (uint64 wheelEntityId : mVehicleWheelIds)
			{
				initializeWheel(wheelEntityId);
			}
		}
		else
		{
			QSF_WARN("QSF failed to restore the original prefab of entity ID " << entity.getId(), QSF_REACT_NONE);
		}
	}

	void RoadVehicleComponent::initializePrefabClearComponentExceptions()
	{
		mComponentExceptionIds.reserve(2);
		mComponentExceptionIds.push_back(FireComponent::COMPONENT_ID);
		mComponentExceptionIds.push_back(qsf::ParticlesComponent::COMPONENT_ID);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	void RoadVehicleComponent::onObjectRenderingStartup()
	{
		// Call the base implementation
		VehicleComponent::onObjectRenderingStartup();

		// Put the simulation job proxy into berserk-mode
		mSimulationJobProxy.changeTimeBetweenCalls(qsf::Time::fromMilliseconds(33));
	}

	void RoadVehicleComponent::onObjectRenderingShutdown()
	{
		// Put the simulation job proxy into sleepy-mode
		mSimulationJobProxy.changeTimeBetweenCalls(qsf::Time::fromMilliseconds(1000));

		// Call the base implementation
		VehicleComponent::onObjectRenderingShutdown();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	void RoadVehicleComponent::onUpdateMovableObjectRenderingJob(const qsf::JobArguments& jobArguments)
	{
		// Ask environment for night time, rain or fog so we can turn on the lights
		setHeadLightState(checkEnvironmentForHeadlight());

		// Brake lights: Update and calculate the brake light state
		updateBrakeLightsState(jobArguments.getTimePassed());

		// Check for emergency flasher active (for squat vehicles when they are standing and have blue lights on)
		if (mIsCommandable)
		{
			// Squad road vehicle

			// When current action isn't a move action turn on emergency flasher
			const qsf::ActionComponent* actionComponent = getEntity().getComponent<qsf::ActionComponent>();
			if (nullptr != actionComponent)
			{
				if (getIsOnEmergencyOperations())
				{
					const qsf::Action* currentAction = actionComponent->getCurrentAction();
					setEmergencyFlasher((nullptr == currentAction || currentAction->getTypeId() != MoveAction::ACTION_ID));
				}
				else
				{
					setEmergencyFlasher(false);
				}
			}
		}

		// Wheels
		updateWheels(jobArguments.getTimePassed());

		// Swinging
		updateSwingAnimation(jobArguments.getTimePassed());

		// Call other implementations
		VehicleComponent::onUpdateMovableObjectRenderingJob(jobArguments);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void RoadVehicleComponent::sendRoadVehicleDamagedMessage()
	{
		qsf::MessageParameters parameters;
		parameters.setParameter("EntityId", getEntityId());
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSDAMAGED_VEHICLE, EntityHelper(getEntity()).getFreeplayEventId(), getEntityId()), parameters);
	}

	void RoadVehicleComponent::placeAllPassengerDeadAroundVehicle(FreeplayEvent* freeplayEvent)
	{
		qsf::TransformComponent& vehicleTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		qsf::Transform& vehicleTransform = vehicleTransformComponent.getTransform();

		std::vector<qsf::Transform> driverDoors;
		getDoorPositions(DoorComponent::DOORTYPE_DRIVER, driverDoors, vehicleTransform);

		// Position and rotation, take the vehicle rotation as fallback
		glm::vec3 startPosition = vehicleTransform.getPosition();
		glm::quat startRotation = vehicleTransform.getRotation();
		if (!driverDoors.empty())
		{
			startPosition = driverDoors.begin()->getPosition();
			startRotation = driverDoors.begin()->getRotation();
		}

		qsf::EntityVectorScratchBuffer entitiesToExit;
		for (auto iterator : getLinks())
		{
			entitiesToExit.push_back(&iterator.second->getTargetEntity());
		}

		for (qsf::Entity* entityInside : entitiesToExit)
		{
			HealthComponent* healthComponent = entityInside->getComponent<HealthComponent>();
			if (nullptr != healthComponent && !healthComponent->isDead())
			{
				VisualVitalSigns visualVitalSigns;	// No visual changes;
				healthComponent->killPerson(freeplayEvent, &visualVitalSigns);
			}

			EntityHelper entityHelper(*entityInside);
			entityHelper.leaveContainer(false);
			entityHelper.fadeIn();

			// Place the person around the vehicle
			qsf::MovableComponent* movableComponent = entityInside->getComponent<qsf::MovableComponent>();
			if (nullptr != movableComponent)
			{
				// Test the place where we want to spawn the unit (position is out parameter)
				glm::vec3 position = startPosition;
				if (!CollisionHelper(getEntity().getMap()).findFreePositionEM3(*entityInside, vehicleTransform, 2.0f, 0.1f, position))
				{
					// Extra check in big range
					CollisionHelper(getEntity().getMap()).findFreePositionEM3(*entityInside, vehicleTransform, 8.0f, 0.1f, position);
				}
				movableComponent->warpToPositionAndRotation(position, startRotation);
			}
		}

		// Vehicle can't be cut open now any more
		setCanBeCut(false);
	}

	void RoadVehicleComponent::registerSimulationJobProxy()
	{
		qsf::jobs::JobConfiguration jobConfiguration;
		jobConfiguration.mTimeBetweenCalls = qsf::Time::fromMilliseconds(1000);
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_VEHICLE, boost::bind(&RoadVehicleComponent::updateSimulation, this, _1));
	}

	void RoadVehicleComponent::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// This job is only active during DAMAGESTATE_INTACT and DAMAGESTATE_DAMAGED_LEVEL1
		// Blue light and emergency operations logic is done inside the move action and commands

		// Start swinging if necessary
		checkSwing();
	}

	void RoadVehicleComponent::initializeWheel(uint64 entityId)
	{
		// Get wheel entity
		qsf::Entity* entity = getEntity().getMap().getEntityById(entityId);
		if (nullptr == entity)
		{
			// No entity -> early out
			return;
		}

		// Get wheel component, create one if necessary
		WheelComponent& wheelComponent = entity->getOrCreateComponentSafe<WheelComponent>();
		if (!wheelComponent.isChain())
		{
			// Get transform and link component
			const qsf::TransformComponent* transformComponent = entity->getComponent<qsf::TransformComponent>();
			const qsf::LinkComponent* linkComponent = entity->getComponent<qsf::LinkComponent>();

			if (nullptr != transformComponent && nullptr != linkComponent)
			{
				// Get wheel's local rotation; use only the yaw angle and ignore the others
				const float yaw = qsf::EulerAngles::quaternionToEuler(linkComponent->getLocalRotation()).x;
				wheelComponent.setOriginalLocalRotation(qsf::EulerAngles::eulerToQuaternion(yaw, 0.0f, 0.0f));

				// Estimate wheel's radius by its pivot position (= center) above the vehicle's pivot (which is on the ground)
				const float wheelRadius = glm::dot(linkComponent->getLocalPosition() * transformComponent->getScale(), qsf::CoordinateSystem::getUp());
				wheelComponent.setRadius(wheelRadius);
			}
		}
	}

	void RoadVehicleComponent::updateWheels(const qsf::Time& timePassed)
	{
		// We need a movable component here
		if (mMovableComponent.valid())
		{
			// Get move distance of the vehicle
			// -> Early out (performance): If the movable is not moving, do nothing
			const float speed = mMovableComponent->getSpeed();
			if (0.0f != speed)
			{
				const float moveDistance = speed * timePassed.getSeconds();

				{ // Update front wheel direction
					const float wheelDirectionLimit = 30.0f;		// In degrees
					const float wheelDirectionChangeRate = 90.0f;	// In degrees per second

					float currentCurve = 0.0f;	// Curvature, i.e. change in direction angle, expressed in radians per meter

					// Check if moving by AI
					qsf::ai::NavigationComponent* navigationComponent = getEntity().getComponent<qsf::ai::NavigationComponent>();
					if (nullptr != navigationComponent)
					{
						// TODO(fw): Move this to the navigation component

						const qsf::ai::Path& path = navigationComponent->getPath();
						if (path.getCurrentNodeIndex() < path.getNodes().size())
						{
							const boost::optional<qsf::ai::TurningConfiguration>& turningConfiguration = path.getCurrentNode().mTurningConfiguration;
							if (turningConfiguration.is_initialized())
							{
								// It's a curve
								currentCurve = 1.0f / turningConfiguration->mRadius;
								if (turningConfiguration->mClockwise)
									currentCurve = -currentCurve;
							}
							else
							{
								// It's a straight line, so use the default curve value of 0.0f
							}
						}
					}

					// Get front wheel direction to be set
					float targetFrontWheelDirection = glm::degrees(currentCurve) * 3.0f;	// TODO(fw): This factor is just chosen using trial and error
					targetFrontWheelDirection = glm::clamp(targetFrontWheelDirection, -wheelDirectionLimit, wheelDirectionLimit);

					// Correction for driving backwards
					if (moveDistance < 0.0f)
						targetFrontWheelDirection = -targetFrontWheelDirection;

					// Interpolate actual front wheel direction over time towards the calculated value
					if (targetFrontWheelDirection != mFrontWheelDirection)
					{
						if (targetFrontWheelDirection > mFrontWheelDirection)
						{
							mFrontWheelDirection = std::min(mFrontWheelDirection + wheelDirectionChangeRate * timePassed.getSeconds(), targetFrontWheelDirection);
						}
						else
						{
							mFrontWheelDirection = std::max(mFrontWheelDirection - wheelDirectionChangeRate * timePassed.getSeconds(), targetFrontWheelDirection);
						}
					}
				}

				const qsf::Map& map = getEntity().getMap();
				for (uint64 wheelId : mVehicleWheelIds)
				{
					// Get wheel entity
					const qsf::Entity* entity = map.getEntityById(wheelId);
					if (entity)
					{
						WheelComponent* wheelComponent = entity->getComponent<WheelComponent>();
						QSF_CHECK(wheelComponent != nullptr, "RoadVehicleComponent::updateWheels(): To vehicle connected wheel has no em5::WheelComponent. Check the editing of the prefab, skip this entity in update", continue);

						if (wheelComponent->isChain())
						{
							float angle = mChainAngle;
							const qsf::TransformComponent* transformComponent = getPrototype().getComponent<qsf::TransformComponent>();
							if (nullptr != transformComponent)
							{
								// Find out the difference in rotation
								angle = qsf::EulerAngles::quaternionToEuler(transformComponent->getRotation()).x;
							}

							wheelComponent->updateWheel(moveDistance, angle - mChainAngle);
							mChainAngle = angle;
						}
						else
						{
							// Update the rotation to the wheel
							wheelComponent->updateWheel(moveDistance, mFrontWheelDirection);
						}
					}
				}
			}
		}
	}

	void RoadVehicleComponent::updateLightsList(const std::vector<uint64>& lightIds, const qsf::Time& timePassed, bool update, bool show, bool force)
	{
		// Update state of the lights
		const qsf::Map& map = getEntity().getMap();
		for (uint64 lightId : lightIds)
		{
			const qsf::Entity* lightEntity = map.getEntityById(lightId);
			if (nullptr != lightEntity)
			{
				qsf::game::LightControllerComponent* lightControllerComponent = lightEntity->getComponent<qsf::game::LightControllerComponent>();
				if (nullptr != lightControllerComponent)
				{
					if (force)
					{
						// We need to force an update -> invert the active state before setting it again
						lightControllerComponent->setActive(!show);

						// Also force an update of the emissive map state.
						lightControllerComponent->setForceEmissiveMapUpdate(force);
					}

					// Set light active state (these won't do anything at all in case the light state already has the desired value)
					lightControllerComponent->setActive(show);
				}
			}
		}
	}

	void RoadVehicleComponent::updateBrakeLightsState(const qsf::Time& timePassed)
	{
		// Update brake lights
		// Check if they has to be active
		const bool breakCheckResult = checkBrakeLights();
		if (breakCheckResult == true)
		{
			// Only allow extending the brake light active state, abort only via timer
			setBrakeLightState(breakCheckResult);
			mBrakeLightTimeout = qsf::Time::fromSeconds(RoadVehicleSpecsGroup::getInstanceSafe().getBrakeLightDelay());
		}
		if (mIsBrakeLightActive == true)
		{
			// Update the brake time
			mBrakeLightTimeout -= timePassed;
			if (mBrakeLightTimeout <= qsf::Time::ZERO)
			{
				// Time is over, deactivate the brake light state
				setBrakeLightState(false);
				mBrakeLightTimeout = qsf::Time::ZERO;
			}
		}
	}

	bool RoadVehicleComponent::checkBrakeLights()
	{
		// We need a movable component here
		if (mMovableComponent.valid())
		{
			// When braking, the acceleration is negative
			const float acceleration = glm::dot(mMovableComponent->getAcceleration(), mMovableComponent->getRotation() * qsf::CoordinateSystem::getIn());
			const float threshold = RoadVehicleSpecsGroup::getInstanceSafe().getBrakeLightAccelerationThreshold();

			// Did the vehicle just brake?
			if (acceleration < threshold)
			{
				// Show brake lights
				return true;
			}
		}

		// Don't show brake lights
		return false;
	}

	void RoadVehicleComponent::checkSwing()
	{
		// We need a movable component here
		if (mMovableComponent.valid())
		{
			// Did the vehicle just brake?
			if (glm::dot(mMovableComponent->getAcceleration(), mMovableComponent->getRotation() * qsf::CoordinateSystem::getIn()) < RoadVehicleSpecsGroup::getInstanceSafe().getSwingAccelerationThreshold())
			{
				mSwingTimeout = qsf::Time::fromSeconds(RoadVehicleSpecsGroup::getInstanceSafe().getSwingTime());
			}
		}
	}

	void RoadVehicleComponent::updateSwingAnimation(const qsf::Time& timePassed)
	{
		// TODO(fw): Disabled this, needs to be reworked
#if 0
		if (mSwingTimeout > qsf::Time::ZERO)
		{
			mSwingTimeout -= timePassed;
			if (mSwingTimeout < qsf::Time::ZERO)
				mSwingTimeout = qsf::Time::ZERO;

			// Get entity's movable component
			if (mMovableComponent.valid())
			{
				const float time = 3.0f - mSwingTimeout.getSeconds();
				const float speed = 4.0f;
				const float amount = 0.5f;

				const float d = (time * speed) - (speed / 2.0f);
				const float angle = (-sinf(d * 3.14f/2.0f) * amount) / ((time*time * amount) + 1.0f);

				qsf::Transform transform;
				transform.setRotation(glm::angleAxis(angle, glm::vec3(1.0f, 0.0f, 0.0f)));
				mMovableComponent->setTransformBias(transform);
			}
		}
#endif
	}

	void RoadVehicleComponent::precalculateWheelsToEntitiesMap() const
	{
		mWheelTypeToEntityIdsMap.clear();

		const qsf::Map& map = getEntity().getMap();
		for (uint64 wheelEntityId : mVehicleWheelIds)
		{
			// Get the entity
			const qsf::Entity* wheelEntity = map.getEntityById(wheelEntityId);
			if (nullptr != wheelEntity)
			{
				const WheelComponent* wheelComponent = wheelEntity->getComponent<WheelComponent>();
				QSF_CHECK(nullptr != wheelComponent, "RoadVehicleComponent::precalculateWheelsToEntitiesMap(): entity in wheel slot has no wheel component, error in editing, skip this entity", continue);

				// Add entity ID in the precalculated list
				auto searchResult = mWheelTypeToEntityIdsMap.find(wheelComponent->getWheelType());
				if (searchResult != mWheelTypeToEntityIdsMap.end())
				{
					// Extent an existing entry
					searchResult->second.push_back(wheelEntityId);
				}
				else
				{
					// Add a new entry
					std::vector<uint64> entityList;
					entityList.push_back(wheelEntityId);
					mWheelTypeToEntityIdsMap.emplace(wheelComponent->getWheelType(), entityList);
				}
			}
		}
	}

	void RoadVehicleComponent::precalculateLightsToEntitiesMap() const
	{
		mLightTypeToEntityIdsMap.clear();

		// We might have lights entities with a light controller component which aren't directly linked to this entity instead to child entities
		// So we have to loop through all child entities to get all light entities with a light controller component
		qsf::EntityVectorScratchBuffer linkedEntities;
		qsf::LinkComponent::getConnectedLinkGroup(getEntity(), qsf::LinkComponent::TRANSFORM, true, linkedEntities);
		for (const qsf::Entity* lightEntity : linkedEntities)
		{
			const qsf::game::LightControllerComponent* lightControllerComponent = lightEntity->getComponent<qsf::game::LightControllerComponent>();
			if (nullptr != lightControllerComponent)
			{
				// Add entity ID in the precalculated list
				auto searchResult = mLightTypeToEntityIdsMap.find(lightControllerComponent->getLightPosition());
				if (searchResult != mLightTypeToEntityIdsMap.end())
				{
					// Extent an existing entry
					searchResult->second.push_back(lightEntity->getId());
				}
				else
				{
					// Add a new entry
					std::vector<uint64> entityList;
					entityList.push_back(lightEntity->getId());
					mLightTypeToEntityIdsMap.emplace(lightControllerComponent->getLightPosition(), entityList);
				}
			}
		}

		mLightTypeToEntityIdsMapInitialized = true;
	}

	bool RoadVehicleComponent::checkEnvironmentForHeadlight()
	{
		if (!isIntact())
			return false;

		bool result = MapHelper(getEntity().getMap()).isNightTime();
		if (!result)
		{
			qsf::game::WeatherComponent* weatherComponent = QSF_MAINMAP.getCoreEntity().getComponent<qsf::game::WeatherComponent>();
			if (nullptr != weatherComponent && (weatherComponent->getCurrentWeatherState().getRainIntensity() > 0.3f || weatherComponent->getCurrentWeatherState().getAtmosphereFogDensity() >= 0.004f))
			{
				result = true;
			}
		}

		// Give an random chance to activate the headlight, so it looks more random and living
		if (result && !mIsHeadLightActive)
		{
			result = qsf::Random::getRandomChance(0.10f);
		}

		return result;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

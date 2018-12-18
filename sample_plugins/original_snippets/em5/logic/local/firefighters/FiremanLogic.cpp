// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/firefighters/FiremanLogic.h"
#include "em5/logic/local/EntityLogicHelper.h"
#include "em5/action/base/PullObjectAction.h"
#include "em5/action/fire/ChangeMaskAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/CommandContext.h"
#include "em5/command/base/StartPullObjectCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/fire/CutRoadVehicleCommand.h"
#include "em5/command/fire/DecontaminatePersonCommand.h"
#include "em5/command/fire/ExtinguishWithExtinguisherCommand.h"
#include "em5/command/fire/ExtinguishWithHoseCommand.h"
#include "em5/command/vegetation/CutTreeCommand.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/debug/GameDebugGroup.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/fire/component/HydrantComponent.h"
#include "em5/fire/component/simplefirehose/SimpleFireHoseComponent.h"
#include "em5/fire/FireHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/collision/CollisionTypes.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/game/Game.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/CollisionHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/specs/FireSpecsGroup.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/equipment/EquipmentComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/debug/request/CircleDebugDrawRequest.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/debug/DebugDrawManager.h>
#include <qsf/job/JobArguments.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 FiremanLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::FiremanLogic");

	const std::string FiremanLogic::ASSET_ABC_SUIT = "em5/mesh/default/hu_m_squad_007_firefighter_abc";
	const std::string FiremanLogic::ASSET_DUST_MASK = "em5/mesh/default/hu_m_squad_006_firefighter_with_mask";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FiremanLogic::FiremanLogic() :
		CarryPersonLogic(GAMELOGIC_TYPE_ID),
		mActiveClothing(CLOTHING_DEFAULT)
	{
		// Nothing here
	}

	uint64 FiremanLogic::getPullingPersonId() const
	{
		return mPullingPersonEntity.valid() ? mPullingPersonEntity->getId() : qsf::getUninitialized<uint64>();
	}

	void FiremanLogic::setPullingPersonId(uint64 id)
	{
		qsf::Entity* ownerEntity = getEntity();
		if (nullptr != ownerEntity)
		{
			mPullingPersonEntity = ownerEntity->getMap().getEntityById(id);
		}

		if (mPullingPersonEntity.valid())
		{
			// Register to catch if our unit person was injured while pulling a person
			mMessageProxyInjured.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSINJURED_PERSON, getEntityId()), boost::bind(&FiremanLogic::onPersonInjured, this, _1));
		}
		else
		{
			mMessageProxyInjured.unregister();
		}
	}

	bool FiremanLogic::isConnectedToHydrant() const
	{
		return mConnectedHydrant.valid();
	}

	qsf::Entity* FiremanLogic::getConnectedHydrant() const
	{
		return mConnectedHydrant.valid() ? &mConnectedHydrant->getEntity() : nullptr;
	}

	void FiremanLogic::connectToHydrant(qsf::Entity& hydrant)
	{
		if (mConnectedHydrant.valid())
		{
			if (&mConnectedHydrant->getEntity() == &hydrant)
			{
				// No change, so nothing to do
				return;
			}

			// Remove old connection first
			disconnectFromHydrant();
		}

		HydrantComponent* hydrantComponent = hydrant.getComponent<HydrantComponent>();
		QSF_CHECK(nullptr != hydrantComponent, "Tried to connect to hydrant entity " << hydrant.getId() << " which has no hydrant component", return);

		// Establish connection
		mConnectedHydrant = hydrantComponent;
		hydrantComponent->connectFireman(*getEntity());

		// Update debug
		onSetDebug(true);
	}

	void FiremanLogic::disconnectFromHydrant()
	{
		if (mConnectedHydrant.valid())
		{
			// Remove connection
			mConnectedHydrant->disconnectFireman();
			mConnectedHydrant.clear();
		}

		// Update debug
		onSetDebug(false);
	}

	SimpleFireHoseComponent* FiremanLogic::getFireHose() const
	{
		return mConnectedFireHose.get();
	}

	void FiremanLogic::createFireHose()
	{
		// Create the fire hose
		QSF_CHECK(mConnectedHydrant.valid(), "Tried to create a fire hose without being connected to a hydrant", return);

		// TODO(fw): This could be moved into a helper method somewhere (e.g. HydrantComponent)
		qsf::Entity* connectorEntity = nullptr;
		{
			qsf::LinkComponent* linkComponent = mConnectedHydrant->getEntity().getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent && !linkComponent->getChildLinks().empty())
			{
				connectorEntity = &(*linkComponent->getChildLinks().begin())->getEntity();
			}
			else
			{
				connectorEntity = &mConnectedHydrant->getEntity();
			}
		}

		qsf::Entity* nozzleEntity = EntityHelper(*getEntity()).getActiveEquipmentEntity();

		if (nullptr != connectorEntity && nullptr != nozzleEntity)
		{
			const std::string connectorBoneName = (mConnectedHydrant->getHydrantType() == HydrantComponent::FIXED_HYDRANT || mConnectedHydrant->getHydrantType() == HydrantComponent::GROUND_HYDRANT) ? "hose_bone_hydrant_1" : "";
			mConnectedFireHose = SimpleFireHoseComponent::createFirehoseBetweenEntities(*connectorEntity, connectorBoneName, *nozzleEntity, "hose_bone");
		}
	}

	void FiremanLogic::destroyFireHose()
	{
		if (mConnectedFireHose.valid())
		{
			// Destroy the fire hose
			MapHelper::destroyEntity(mConnectedFireHose->getEntity());
			mConnectedFireHose.clear();
		}
	}

	void FiremanLogic::performAutomatism()
	{
		// Check if firefighter has an active equipment
		EntityHelper entityHelper(*getEntity());
		qsf::game::EquipmentComponent* equipmentComponent = entityHelper.getActiveEquipmentComponent();
		std::string equipmentName = (nullptr != equipmentComponent ? equipmentComponent->getEquipmentName() : "");

		GameSettingsGroup::AutomatismValue automatismValue = GameSettingsGroup::getInstanceSafe().getAutomatismValue();

		if ((equipmentName == equipment::FIRE_EXTINGUISHER || equipmentName == equipment::FIRE_EXTINGUISHER_HISTORICAL)
			&& automatismValue >= GameSettingsGroup::AUTOMATISM_MEDIUM)
		{
			performExtinguisherAutomatism();
		}
		else if ((equipmentName == equipment::HOSE_NOZZLE || equipmentName == equipment::HOSE_ROLLED ||
			equipmentName == equipment::HOSE_NOZZLE_HISTORICAL || equipmentName == equipment::HOSE_ROLLED_HISTORICAL)
			&& automatismValue >= GameSettingsGroup::AUTOMATISM_MEDIUM)
		{
			performExtinguishWithHoseAutomatism();
		}
		else if (equipmentName == equipment::RESCUE_CUTTER
			&& automatismValue >= GameSettingsGroup::AUTOMATISM_ALL)
		{
			performRescueCutterAutomatism();
		}
		else if ((equipmentName == equipment::CHAIN_SAW || equipmentName == equipment::CHAIN_SAW_HISTORICAL)
			&& automatismValue >= GameSettingsGroup::AUTOMATISM_ALL)
		{
			performCuttingblockingTreesAutomatism();
		}
		else if (equipmentName.empty() && automatismValue >= GameSettingsGroup::AUTOMATISM_ALL)
		{
			if (entityHelper.isCarrying(EntityHelper::CarryLoad(EntityHelper::CARRY_PERSON|EntityHelper::PULL_PERSON)))
			{
				performDecontaminatePersonAutomatism();
			}
			else
			{
				performPullObjectFromPersonAutomatism();
			}
		}
	}

	FiremanLogic::Clothing FiremanLogic::getActiveClothing() const
	{
		return mActiveClothing;
	}

	void FiremanLogic::setActiveClothing(Clothing clothing, bool changeMesh)
	{
		if (changeMesh)
		{
			qsf::MeshComponent& meshComponent = getEntity()->getComponentSafe<qsf::MeshComponent>();

			if (clothing == CLOTHING_ABC_SUIT)
			{
				// Change to the ABC suit
				qsf::AssetProxy asset(ASSET_ABC_SUIT);
				meshComponent.setMesh(asset);
			}
			else if (clothing == CLOTHING_DUST_MASK)
			{
				// Change to dust mask
				qsf::AssetProxy asset(ASSET_DUST_MASK);
				meshComponent.setMesh(asset);
			}
			else
			{
				meshComponent.setMesh(mAssetDefaultSuit);
			}
		}
		mActiveClothing = clothing;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool FiremanLogic::onStartup()
	{
		// Don't do anything, when on multiplay client side
		if (nullptr == EM5_NETWORK.getMultiplayerClient())
		{
			// Save default mesh
			qsf::MeshComponent& meshComponent = getEntity()->getComponentSafe<qsf::MeshComponent>();
			mAssetDefaultSuit = meshComponent.getMesh();

			// Register job
			// -> For performance reasons, do updates not in every tick, but in fixed time intervals
			qsf::jobs::JobConfiguration jobConfiguration;
			jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(0.2f);
			mAnimationJobProxy.registerAt(Jobs::ANIMATION_HIGHLIGHT, boost::bind(&FiremanLogic::updateAnimation, this, _1), jobConfiguration);

			jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
			mSimulationJobProxy.registerAt(Jobs::ANIMATION_HIGHLIGHT, boost::bind(&FiremanLogic::updateSimulation, this, _1), jobConfiguration);

			jobConfiguration.mTimeBetweenCalls = qsf::Time::ZERO;
			jobConfiguration.mNumberOfCalls = 1;
			mFirstUpdateJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&FiremanLogic::postInitialize, this, _1), jobConfiguration);

			mHeatEnergyProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSHEATENERGY_PERSON, getEntityId(), 0), boost::bind(&FiremanLogic::onReceiveHeatEnergy, this, _1));
			mContaminationAreaProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_FIREFIGHTER_INSIDE_CONTAMINATION_AREA, getEntityId()), boost::bind(&FiremanLogic::onReceiveHeatEnergy, this, _1));	// This does the same as on receive heat energy
		}

		return true;
	}

	void FiremanLogic::onShutdown()
	{
		// Unregister job
		mFirstUpdateJobProxy.unregister();
		mSimulationJobProxy.unregister();
		mAnimationJobProxy.unregister();

		mMessageProxyInjured.unregister();
		mHeatEnergyProxy.unregister();
		mContaminationAreaProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void FiremanLogic::onSetDebug(bool debug)
	{
		if (debug)
		{
			// Do nothing
		}
		else
		{
			// Disable the debug draw
			mDebugDrawProxyRange.unregister();
			mDebugDrawProxyHose.unregister();
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void FiremanLogic::updateAnimation(const qsf::JobArguments& jobArguments)
	{
#ifndef ENDUSER
		updateHydrantVisualisation();
#endif
	}

	void FiremanLogic::onReceiveHeatEnergy(const qsf::MessageParameters& parameters)
	{
		if (getEntity() == nullptr)
			return;

		// No mask changes for historic units
		CommandableComponent* commandableComponent = getEntity()->getComponent<CommandableComponent>();
		if (nullptr != commandableComponent && (commandableComponent->isFireFighterPersonHistoric() || commandableComponent->isPoliceGuardsmanHistoric()))
			return;

		if (mActiveClothing == CLOTHING_DEFAULT)
		{
			// Change to the Dust mask
			qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();
			qsf::AssetProxy assetABC(ASSET_DUST_MASK);
			actionComponent.pushAction<ChangeMaskAction>(action::AUTOMATISM_STD, qsf::action::INSERT_AT_FRONT).init(ChangeMaskAction::ADD_MASK);

			mActiveClothing = CLOTHING_DUST_MASK;	// Tell it already here it has that clothing
		}

		mTimeRemoveDustMask = qsf::Time::ZERO;
	}

	void FiremanLogic::updateHydrantVisualisation()
	{
		// Clear the debug draw
		mDebugDrawProxyHose.unregister();
		mDebugDrawProxyRange.unregister();

		const bool isSelected = EM5_GAME.getSelectionManager().isIdSelected(getEntityId());
		if (isSelected && isConnectedToHydrant())
		{
			glm::vec3 hydrantPosition;
			glm::vec3 callerPosition;

			// Get the hydrant entity
			const qsf::Entity* hydrantEntity = getConnectedHydrant();
			if (nullptr != hydrantEntity)
			{
				// Get the transform component of the hydrant
				const qsf::TransformComponent* hydrantTransformComponent = hydrantEntity->getComponent<qsf::TransformComponent>();
				if (nullptr != hydrantTransformComponent)
				{
					// Get the position from the hydrant
					hydrantPosition = hydrantTransformComponent->getPosition();
				}
			}

			// Get the transform component of the hydrant
			const qsf::TransformComponent* callerTransformComponent = getEntity()->getComponent<qsf::TransformComponent>();
			if (nullptr != callerTransformComponent)
			{
				// Get the position from the hydrant
				callerPosition = callerTransformComponent->getPosition();
			}

			if (GameDebugGroup::getInstanceSafe().getShowDebugTextComponent())
			{
				// Draw the hydrant
				mDebugDrawProxyRange.registerAt(QSF_DEBUGDRAW);
				static const glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);
				mDebugDrawProxyRange.addRequest(qsf::CircleDebugDrawRequest(hydrantPosition, rotationAxis, FireSpecsGroup::getInstanceSafe().getRangeHydrant(), qsf::Color4::WHITE, true));
			}
		}
	}

	void FiremanLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// Access the caller's action plan
		const qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

		// Check if the paramedic is currently busy
		bool isBusy = (actionComponent.getCurrentPriority() >= action::AUTOMATISM_STD);	// Overwrite idle actions and low-priority automatisms

		if (!MapHelper::isUnitAutomatismActive())
		{
			// No automatism active
			isBusy = true;
		}

		// Check if the person is inside a road vehicle
		EntityHelper entityHelper(*getEntity());
		if (entityHelper.isEntityInRoadVehicle() || entityHelper.isPersonInjured())
		{
			isBusy = true;
		}

		if (isBusy)
		{
			// Reset wait time
			mTimeWaiting = qsf::Time::ZERO;

			// Reset timers
			mTimeRemoveDustMask = qsf::Time::ZERO;

			// In case of injury, disconnect the hydrant
			if (entityHelper.isPersonInjured())
			{
				disconnectFromHydrant();
			}
		}
		else
		{
			// Wait some time until getting active
			mTimeWaiting += jobArguments.getTimePassed();

			// Waited long enough now?
			if (mTimeWaiting >= qsf::Time::fromSeconds(LogicSpecsGroup::getInstanceSafe().getFiremanMinimumWaitingTime()))
			{
				// Perform the automatism now
				performAutomatism();

				// Reset wait time
				mTimeWaiting = qsf::Time::ZERO;
			}

			if (mActiveClothing == CLOTHING_DUST_MASK)
			{
				// Wait some time to remove Dust mask
				mTimeRemoveDustMask += jobArguments.getTimePassed();

				// Check when we last got heat energy
				HealthComponent* healthComponent = getEntity()->getComponent<HealthComponent>();
				if (nullptr != healthComponent)
				{
					const qsf::Time& timeSinceLastHeat = healthComponent->getElapsedTimeAfterLastHeatEnergyReceived();
					if (mTimeRemoveDustMask > timeSinceLastHeat)
						mTimeRemoveDustMask = timeSinceLastHeat;
				}

				// Never remove it while we have the firehose nozzle
				if (EntityHelper(*getEntity()).checkIsActiveEquipment(equipment::HOSE_NOZZLE))
					mTimeRemoveDustMask = qsf::Time::ZERO;

				// Time to remove it?
				const float SECONDS_UNTIL_REMOVE_DUST_MASK = FireSpecsGroup::getInstanceSafe().getRemoveMaskTime();
				if (mTimeRemoveDustMask >= qsf::Time::fromSeconds(SECONDS_UNTIL_REMOVE_DUST_MASK))
				{
					// Do so
					qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();
					actionComponent.pushAction<ChangeMaskAction>(action::AUTOMATISM_STD, qsf::action::INSERT_AT_FRONT).init(ChangeMaskAction::REMOVE_MASK);

					mActiveClothing = CLOTHING_DEFAULT;
				}
			}
		}
	}

	void FiremanLogic::postInitialize(const qsf::JobArguments& jobArguments)
	{
		// The following is done here because it's not safe to do this in startup already
		//  -> The commandable component may not be initialized correctly

		// If this is an ABC firefighter, set the suit enabled
		CommandableComponent* commandableComponent = getEntity()->getComponent<CommandableComponent>();
		if (nullptr != commandableComponent && commandableComponent->isUnitType(CommandableComponent::UNITTAG_FIREFIGHTERS_FIREFIGHTER_ABC))
		{
			// We don't need to set another mesh
			setActiveClothing(CLOTHING_ABC_SUIT, false);
		}
	}

	void FiremanLogic::performExtinguisherAutomatism()
	{
		// Optimization: To avoid constant allocations/deallocations, use a static instance (not multi-threading safe, of course)
		static std::vector<FireComponent*> burningFireComponentVector;
		FireHelper::getSortedTargetsInRange(burningFireComponentVector, *getEntity(), LogicSpecsGroup::getInstanceSafe().getFiremanSearchRadius());

		for (FireComponent* fireComponent : burningFireComponentVector)
		{
			// Use here the main entity, because the fire component might be part of an entity which is only a child of another entity (e.g. building)
			if (EntityLogicHelper::checkAndExecuteAutomatism<ExtinguishWithExtinguisherCommand>(getEntity(), &fireComponent->getMainEntity()))
			{
#ifndef ENDUSER
				if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
				{
					const glm::vec3 ownerPosition = getEntity()->getComponentSafe<qsf::TransformComponent>().getPosition();
					QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:ExtinguishWithExtinguisherCommand", ownerPosition, qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
				}
#endif
				break;
			}
		}
	}

	void FiremanLogic::performExtinguishWithHoseAutomatism()
	{
		// Optimization: To avoid constant allocations/deallocations, use a static instance (not multi-threading safe, of course)
		static std::vector<FireComponent*> burningFireComponentVector;

		float extinguishRange = (UnitHelper(*getEntity()).isMedievalGuardsman() ? FireSpecsGroup::getInstanceSafe().getRangeHoseMax()-1 : LogicSpecsGroup::getInstanceSafe().getFiremanSearchRadius());
		FireHelper::getSortedTargetsInRange(burningFireComponentVector, *getEntity(), extinguishRange);

		for (FireComponent* fireComponent : burningFireComponentVector)
		{
			// Use here the main entity, because the fire component might be part of an entity which is only a child of another entity (e.g. building)
			if (EntityLogicHelper::checkAndExecuteAutomatism<ExtinguishWithHoseCommand>(getEntity(), &fireComponent->getMainEntity()))
			{
#ifndef ENDUSER
				if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
				{
					const glm::vec3& ownerPosition = getEntity()->getComponentSafe<qsf::TransformComponent>().getPosition();
					QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:ExtinguishWithHoseCommand", ownerPosition, qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
				}
#endif
				break;
			}
		}
	}

	void FiremanLogic::performPullObjectFromPersonAutomatism()
	{
		qsf::Entity* entity = getEntity();
		const qsf::TransformComponent& transformComponent = entity->getOrCreateComponentSafe<qsf::TransformComponent>();
		std::vector<BuryComponent*> buryComponentVector;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(transformComponent.getPosition(), LogicSpecsGroup::getInstanceSafe().getFiremanSearchRadius(), buryComponentVector);

		// Filter components
		std::vector<BuryComponent*> validBuryComponentVector;
		for (BuryComponent* buryComponent : buryComponentVector)
		{
			// Has targets?
			if (buryComponent->BuriedEntitiesArray.getVariable().empty())
				continue;

			// Visible + draggable targets
			if (buryComponent->getIsHidden() == false && buryComponent->getDraggable())
			{
				validBuryComponentVector.push_back(buryComponent);
			}
		}

		for (BuryComponent* buryComponent : validBuryComponentVector)
		{
			if (EntityLogicHelper::checkAndExecuteAutomatism<StartPullObjectCommand>(entity, &buryComponent->getEntity()))
			{
#ifndef ENDUSER
				if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
				{
					const glm::vec3& ownerPosition = entity->getComponentSafe<qsf::TransformComponent>().getPosition();
					QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:StartPullObjectCommand", ownerPosition, qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
				}
#endif

				// Best chance for a target is probably somewhere behind the unit doing the pulling
				glm::vec3 offsetLocal(0.0f, 0.0f, -1.0f);
				qsf::TransformComponent* targetTransformComponent = buryComponent->getEntity().getOrCreateComponent<qsf::TransformComponent>();
				qsf::Transform transform(targetTransformComponent->getTransform());
				transform.setPosition(transform.getPosition() + transform.vec3DirectionLocalToWorld(offsetLocal));

				glm::vec3 outPos;
				CollisionHelper collisionHelper(entity->getMap());
				collisionHelper.findFreePositionEM3(buryComponent->getEntity(), transform, 3.0f, 0.0f, outPos);

				qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();
				qsf::logic::TargetPoint targetPoint(outPos, false, 0.5f, 0.5f);
				actionComponent.pushAction<MoveAction>(action::AUTOMATISM_STD).init(new qsf::ai::ReachSinglePointGoal(targetPoint), MovementModes::MOVEMENT_MODE_WALK_BACKWARDS);
				actionComponent.pushAction<PullObjectAction>(action::AUTOMATISM_STD).init(buryComponent->getEntity(), PullObjectAction::STOP, true);

				break;
			}
		}
	}

	void FiremanLogic::performRescueCutterAutomatism()
	{
		qsf::Entity* entity = getEntity();
		const qsf::TransformComponent& transformComponent = entity->getOrCreateComponentSafe<qsf::TransformComponent>();

		// Can we cut road vehicles?
		CutRoadVehicleCommand* cutRoadVehicleCommand = static_cast<CutRoadVehicleCommand*>(QSFGAME_COMMAND.getCommandManager().getById<CutRoadVehicleCommand>(CutRoadVehicleCommand::PLUGINABLE_ID));
		if (nullptr != cutRoadVehicleCommand && cutRoadVehicleCommand->checkCaller(*entity))
		{
			CommandContext commandContext;
			commandContext.mCaller = entity;

			// Get vehicles in area
			std::vector<RoadVehicleComponent*> vehicleComponents;
			qsf::ComponentMapQuery(getMap()).getInstancesInCircle(transformComponent.getPosition(), LogicSpecsGroup::getInstanceSafe().getFiremanSearchRadius(), vehicleComponents);
			for (RoadVehicleComponent* vehicleComponent : vehicleComponents)
			{
				commandContext.mTargetEntity = &vehicleComponent->getEntity();
				if (cutRoadVehicleCommand->checkContext(commandContext))
				{
					// Cut it open
					if (EntityLogicHelper::checkAndExecuteAutomatism<CutRoadVehicleCommand>(entity, commandContext.mTargetEntity))
					{
						return;
					}
				}
			}
		}
	}

	void FiremanLogic::performCuttingblockingTreesAutomatism()
	{
		qsf::Entity* entity = getEntity();
		const qsf::TransformComponent& transformComponent = entity->getOrCreateComponentSafe<qsf::TransformComponent>();

		// Can we cut trees?
		CutTreeCommand* cutTreeCommand = QSFGAME_COMMAND.getCommandManager().getById<CutTreeCommand>(CutTreeCommand::PLUGINABLE_ID);
		if (nullptr != cutTreeCommand && cutTreeCommand->checkCaller(*entity))
		{
			CommandContext commandContext;
			commandContext.mCaller = entity;

			// Get objects with bury component in area (cutting only blocking trees)
			std::vector<BuryComponent*> buryComponents;
			qsf::ComponentMapQuery(getMap()).getInstancesInCircle(transformComponent.getPosition(), LogicSpecsGroup::getInstanceSafe().getFiremanSearchRadius(), buryComponents);
			for (BuryComponent* vehicleComponent : buryComponents)
			{
				commandContext.mTargetEntity = &vehicleComponent->getEntity();
				if (cutTreeCommand->checkContext(commandContext))
				{
					// Cut it open
					if (EntityLogicHelper::checkAndExecuteAutomatism<CutTreeCommand>(entity, commandContext.mTargetEntity))
					{
						return;
					}
				}
			}
		}
	}

	void FiremanLogic::performDecontaminatePersonAutomatism()
	{
		qsf::Entity* entity = getEntity();
		const qsf::TransformComponent& transformComponent = entity->getOrCreateComponentSafe<qsf::TransformComponent>();

		// Could we decontaminate a person?
		DecontaminatePersonCommand* decontaminatePersonCommand = static_cast<DecontaminatePersonCommand*>(QSFGAME_COMMAND.getCommandManager().getById<DecontaminatePersonCommand>(DecontaminatePersonCommand::PLUGINABLE_ID));
		if (nullptr != decontaminatePersonCommand && decontaminatePersonCommand->checkCaller(*entity))
		{
			CommandContext commandContext;
			commandContext.mCaller = entity;

			// Find vehicles in the area (can't search just for decontamination vehicles so we look for all)
			std::vector<CommandableComponent*> commandableComponents;
			qsf::ComponentMapQuery(getMap()).getInstancesInCircle(transformComponent.getPosition(), LogicSpecsGroup::getInstanceSafe().getFiremanSearchRadius(), commandableComponents);
			for (CommandableComponent* commandableComponent : commandableComponents)
			{
				commandContext.mTargetEntity = &commandableComponent->getEntity();
				if (decontaminatePersonCommand->checkContext(commandContext))
				{
					// We can decontaminate - let's do it
					if (EntityLogicHelper::checkAndExecuteAutomatism<DecontaminatePersonCommand>(entity, commandContext.mTargetEntity))
					{
						return;
					}
				}
			}
		}
	}

	void FiremanLogic::onPersonInjured(const qsf::MessageParameters& parameters)
	{
		// Fireman gets injured while pulling a person -> unlink them
		if (getEntity() != nullptr)
		{
			if (mPullingPersonEntity.valid())
			{
				AnimationHelper(mPullingPersonEntity.getSafe()).clearAnimation();
				EntityHelper(mPullingPersonEntity.getSafe()).unlinkFromParent();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

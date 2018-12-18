// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"
#include "em5/action/ambulance/CancelDogSearchAction.h"
#include "em5/action/ambulance/CombineDogWithLeaderAction.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/base/SetInvincibleAction.h"
#include "em5/action/SignalAction.h"
#include "em5/action/base/ReservateTargetAction.h"
#include "em5/action/base/WaitForEntityAction.h"
#include "em5/action/base/WaitForEnterVehicleAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/WaitForSignalAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/equipment/GetDogFromVehicleAction.h"
#include "em5/base/GameplayAssets.h"
#include "em5/command/base/EnterVehicleCommand.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/Game.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/EnterTrunkDoorTargetPointProvider.h"
#include "em5/game/targetpoint/OnTouchTargetPointProvider.h"
#include "em5/game/targetpoint/GetRescueDogFromVehicleTargetPointProvider.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/logic/local/MiniMapIconRegistrationLogic.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/audio/AudioHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/network/NetworkManager.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf_game/component/base/SelectableComponent.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/component/link/LinkComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 RescueDogLeaderLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::RescueDogLeaderLogic");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 RescueDogLeaderLogic::ACTION_PRIORITY = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Static public methods                                 ]
	//[-------------------------------------------------------]
	qsf::Entity* RescueDogLeaderLogic::createRescueDogEntity(qsf::Entity& rescueDogLeaderEntity)
	{
		qsf::StringHash dogPrefab = (UnitHelper(rescueDogLeaderEntity).isAmbulanceRescueDogLeader() ? assets::PREFAB_RESCUEDOG : assets::PREFAB_POLICEDOG);
		qsf::Entity* rescueDogEntity = MapHelper(rescueDogLeaderEntity.getMap()).createObjectByLocalPrefabAssetId(dogPrefab);

		if (nullptr != rescueDogEntity)
		{
			EntityHelper(rescueDogLeaderEntity).moveEntityInOwnerLayer(*rescueDogEntity);

			if (nullptr != EM5_NETWORK.getMultiplayerHost())
			{
				EntityHelper::setupEntityAsGhost(*rescueDogEntity, rescueDogLeaderEntity);
			}

			// Setup the Rescue dog leader logic
			qsf::GameLogicComponent& gamelogicComponent = rescueDogLeaderEntity.getOrCreateComponentSafe<qsf::GameLogicComponent>();
			RescueDogLeaderLogic& rescueDogLeaderLogic = gamelogicComponent.getOrCreateGameLogicSafe<RescueDogLeaderLogic>();
			rescueDogLeaderLogic.setRescueDog(rescueDogEntity);

			// Create the commandable component. This is needed to avoid that the dog gets marked as an command target (e.g. pepper spray) while leaving the vehicle
			rescueDogEntity->getOrCreateComponentSafe<CommandableComponent>().setActive(false);

			// Never touch the doggy
			EntityHelper(*rescueDogEntity).setInvincible(true);
		}

		return rescueDogEntity;
	}

	qsf::Entity* RescueDogLeaderLogic::getRescueDogEntityFromLeader(qsf::Entity& rescueDogLeaderEntity)
	{
		qsf::GameLogicComponent* gameLogicComponent = rescueDogLeaderEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return nullptr;

		RescueDogLeaderLogic* rescueDogLeaderLogic = gameLogicComponent->getGameLogic<RescueDogLeaderLogic>();
		if (nullptr == rescueDogLeaderLogic)
			return nullptr;

		return rescueDogLeaderLogic->getRescueDog();
	}

	void RescueDogLeaderLogic::delinkDogFromLeader(qsf::Entity& rescueDogLeaderEntity)
	{
		// Tell the logic about the freedom of the dog
		qsf::GameLogicComponent* gameLogicComponent = rescueDogLeaderEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return;

		RescueDogLeaderLogic* rescueDogLeaderLogic = gameLogicComponent->getGameLogic<RescueDogLeaderLogic>();
		if (nullptr == rescueDogLeaderLogic)
			return;

		rescueDogLeaderLogic->setIsDogUnitedWithLeader(false);

		qsf::Entity* rescueDogEntity = rescueDogLeaderLogic->getRescueDog();
		if (nullptr == rescueDogEntity)
			return;

		// Reactivate the movement component
		rescueDogEntity->getOrCreateComponentSafe<qsf::MovableComponent>().setActive(true);

		// Mark dog as dog, but without having the component active. Warning, not all information are set
		{
			const bool isRescueDog = UnitHelper(rescueDogLeaderEntity).isAmbulanceRescueDogLeader();
			const CommandableComponent& leaderCommandableComponent = rescueDogLeaderEntity.getComponentSafe<CommandableComponent>();
			CommandableComponent& commandableComponent = rescueDogEntity->getOrCreateComponentSafe<CommandableComponent>();
			commandableComponent.setUnitType(isRescueDog ? CommandableComponent::UNITTAG_AMBULANCE_RESCUEDOG : CommandableComponent::UNITTAG_POLICE_POLICEDOG);
			commandableComponent.setPlayerId(leaderCommandableComponent.getPlayerId());
			commandableComponent.setTeamId(leaderCommandableComponent.getTeamId());
			commandableComponent.setMiniMapIconType(isRescueDog ? CommandableComponent::MINIMAP_ICON_UNIT_AMBULANCE_DOG : CommandableComponent::MINIMAP_ICON_UNIT_POLICE_DOG);
			commandableComponent.setActive(false);
		}

		rescueDogEntity->getOrCreateComponentSafe<qsf::game::SelectableComponent>();

		// Destroy the linkage
		rescueDogEntity->destroyComponent<qsf::LinkComponent>();

		// TODO(mk): Hack: create collision box, we have manually removed the collision
		qsf::ai::RouterComponent* routerComponent = rescueDogEntity->getComponent<qsf::ai::RouterComponent>();
		if (nullptr != routerComponent)
			routerComponent->createCollisionBox();

		// With an inactive navigation component we are not using ai reservations from this entity, so we need to enable it
		qsf::ai::NavigationComponent* navigationComponent = rescueDogEntity->getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setActive(true);
		}

		// Reset the animation of the dog, to avoid starting with wrong movement state
		PersonComponent* personComponent = rescueDogEntity->getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			personComponent->forceIdleAnimationUpdate();
		}

		// Create mini-map icon for dog
		EM5_GAME.getMiniMapIconRegistrationLogic().registerUnitIcon(*rescueDogEntity);
	}

	void RescueDogLeaderLogic::linkDogWithLeader(qsf::Entity& rescueDogLeaderEntity, bool leavingVehicle)
	{
		qsf::GameLogicComponent* gameLogicComponent = rescueDogLeaderEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return;

		RescueDogLeaderLogic* rescueDogLeaderLogic = gameLogicComponent->getGameLogic<RescueDogLeaderLogic>();
		if (nullptr == rescueDogLeaderLogic)
			return;

		// Combine them again
		rescueDogLeaderLogic->setIsDogUnitedWithLeader(true);

		qsf::Entity* rescueDogEntity = rescueDogLeaderLogic->getRescueDog();
		if (nullptr == rescueDogEntity)
			return;

		//rescueDogEntity->destroyComponent<CommandableComponent>();
		rescueDogEntity->destroyComponent<qsf::game::SelectableComponent>();

		static const glm::vec3 DOG_OFFSET(0.6f, 0, 0.214f);
		if (leavingVehicle)
		{
			// If dog leave the vehicle, don't link him, to support better animation

			// Set dog position
			qsf::TransformComponent& transformComponent = rescueDogLeaderEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
			qsf::Transform turnedRescueDogLeaderTransform = transformComponent.getTransform();
			turnedRescueDogLeaderTransform.setRotation(transformComponent.getRotation() * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::pi<float>()));	// Rotate 180°

			const glm::vec3 newPosition = turnedRescueDogLeaderTransform.vec3PositionLocalToWorld(DOG_OFFSET);

			// Set the new position, rotation to the dog
			rescueDogEntity->getOrCreateComponentSafe<qsf::MovableComponent>().warpToPositionAndRotation(newPosition, turnedRescueDogLeaderTransform.getRotation());
		}
		else
		{
			// Link both together
			qsf::LinkComponent& linkComponent = rescueDogEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
			linkComponent.linkToParent(rescueDogLeaderEntity);

			// Deactivate animation interpolation via movement
			rescueDogEntity->getOrCreateComponentSafe<qsf::MovableComponent>().setActive(false);

			// Set dog position
			qsf::TransformComponent& transformComponent = rescueDogLeaderEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
			const glm::vec3 newPosition = transformComponent.getTransform().vec3PositionLocalToWorld(DOG_OFFSET);

			// Set the new position, rotation to the dog
			rescueDogEntity->getOrCreateComponentSafe<qsf::MovableComponent>().warpToPositionAndRotation(newPosition, transformComponent.getRotation());
		}

		// Remove collision box of the dog, so the leader didn't collide with his dog
		qsf::ai::RouterComponent* routerComponent = rescueDogEntity->getComponent<qsf::ai::RouterComponent>();
		if (nullptr != routerComponent)
			routerComponent->removeCollisionBox();

		// With an active navigation component we are still using ai reservations from this entity, so we need to disable it
		qsf::ai::NavigationComponent* navigationComponent = rescueDogEntity->getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setActive(false);
		}

		// Remove mini-map icon for dog
		EM5_GAME.getMiniMapIconRegistrationLogic().unregisterIcon(*rescueDogEntity);
	}

	void RescueDogLeaderLogic::callRescueDogBack(qsf::Entity& rescueDogLeaderEntity)
	{
		qsf::Entity* rescueDogEntity = getRescueDogEntityFromLeader(rescueDogLeaderEntity);
		if (nullptr != rescueDogEntity)
		{
			AnimationHelper(*rescueDogEntity).clearAnimation();

			qsf::ActionComponent& actionComponent = rescueDogEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();

			// Push action
			actionComponent.pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachObjectGoal(*rescueDogEntity, rescueDogLeaderEntity, OnTouchTargetPointProvider::TARGET_POINT_ID));
			actionComponent.pushAction<CombineDogWithLeaderAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(rescueDogLeaderEntity);
		}
	}

	void RescueDogLeaderLogic::playDogPantAudio(qsf::Entity& rescueDogLeaderEntity, bool pant)
	{
		qsf::GameLogicComponent* gameLogicComponent = rescueDogLeaderEntity.getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return;

		RescueDogLeaderLogic* rescueDogLeaderLogic = gameLogicComponent->getGameLogic<RescueDogLeaderLogic>();
		if (nullptr == rescueDogLeaderLogic)
			return;

		qsf::Entity* rescueDogEntity = rescueDogLeaderLogic->getRescueDog();
		if (pant && nullptr != rescueDogEntity)
		{
			AudioHelper::playRescueDogPant(rescueDogLeaderLogic->mAudioProxy, *rescueDogEntity);
			rescueDogLeaderLogic->mAudioProxy.synchronizeWithMultiplay();
		}
		else
		{
			rescueDogLeaderLogic->mAudioProxy.stop();
			rescueDogLeaderLogic->mAudioProxy.synchronizeWithMultiplay();
		}
	}

	bool RescueDogLeaderLogic::checkIsRescueDogUnitedWithLeader(qsf::Entity& rescueDogLeaderEntity)
	{
		qsf::LinkComponent* linkComponent = rescueDogLeaderEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			for (const qsf::LinkComponent* childLink : linkComponent->getChildLinks())
			{
				if (nullptr != childLink)
				{
					const PersonComponent* personComponent = childLink->getEntity().getComponent<PersonComponent>();
					if (nullptr != personComponent && personComponent->getGender() == PersonComponent::GENDER_ANIMAL)
					{
						return true;
					}
				}
			}
		}

		return false;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RescueDogLeaderLogic::RescueDogLeaderLogic() :
		CarryPersonLogic(GAMELOGIC_TYPE_ID),
		mIsDogUnitedWithLeader(false),
		mSafetyStuckResolvingTime(qsf::Time::ZERO)
	{
		// Nothing here
	}

	void RescueDogLeaderLogic::setIsDogUnitedWithLeader(bool value)
	{
		mIsDogUnitedWithLeader = value;
	}

	bool RescueDogLeaderLogic::getIsDogUnitedWithLeader()
	{
		return mIsDogUnitedWithLeader;
	}

	void RescueDogLeaderLogic::setRescueDog(qsf::Entity* dogEntity)
	{
		mRescueDogEntity = dogEntity;
	}

	qsf::Entity* RescueDogLeaderLogic::getRescueDog()
	{
		return mRescueDogEntity.get();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool RescueDogLeaderLogic::onStartup()
	{
		// Don't do anything, when on multiplay client side
		if (nullptr == EM5_NETWORK.getMultiplayerClient())
		{
			// Register messages
			mEnterVehicleProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ON_COMMAND_EXECUTE, EnterVehicleCommand::PLUGINABLE_ID, getEntityId()), boost::bind(&RescueDogLeaderLogic::putDogInVehicle, this, _1));
			mExitVehicleProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_PERSON_EXIT_VEHICLE, getEntityId()), boost::bind(&RescueDogLeaderLogic::takeDogFromVehicle, this, _1));
			mLeaderInjured.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSINJURED_PERSON, getEntityId()), boost::bind(&RescueDogLeaderLogic::dieRescueDog, this, _1));
			mLeaderContaminated.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_HEALTH_PERSON_CONTAMINATED, getEntityId()), boost::bind(&RescueDogLeaderLogic::onContaminated, this, _1));
		}

		return true;
	}

	void RescueDogLeaderLogic::onShutdown()
	{
		// Unregister messages and job
		mExitVehicleProxy.unregister();
		mEnterVehicleProxy.unregister();
		mLeaderInjured.unregister();
		mLeaderContaminated.unregister();

		mSafetyStuckResolvingJob.unregister();

		// The rescue dog leader entity seems to be going down -> Remove rescue dog when not linked
		if (nullptr == EM5_NETWORK.getMultiplayerClient() && mRescueDogEntity.valid())
		{
			if (nullptr == mRescueDogEntity->getComponent<qsf::LinkComponent>())
			{
				MapHelper(mRescueDogEntity->getMap()).destroyEntity(mRescueDogEntity.getSafe());
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void RescueDogLeaderLogic::takeDogFromVehicle(const qsf::MessageParameters& parameters)
	{
		if (Messages::EM5_ACTION_PERSON_EXIT_VEHICLE.getHash() == parameters.getFilter(0))
		{
			// The dog leader is about to leave the vehicle
			const uint64 vehicleId = parameters.getFilter(2);
			qsf::Entity* rescueDogVehicle = getEntity()->getMap().getEntityById(vehicleId);
			QSF_CHECK(nullptr != rescueDogVehicle, "RescueDogLeaderLogic::takeDogFromVehicle(): There is no vehicle with the ID '" << vehicleId << "' where the rescue dog leader '" << getEntityId() << "' could be inside", return);

			// Push dog leaders actions
			qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();

			// Reserve owner and make him invincible
			ReserveLogic::createReservation<ReserveEnterVehicleLogic>(*rescueDogVehicle, getEntityId());
			EntityHelper(*getEntity()).setInvincible(true);	// Here immediately set the entity invincible

			// Push actions
			actionComponent.pushAction<MoveAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(new qsf::ai::ReachObjectGoal(*getEntity(), *rescueDogVehicle, GetRescueDogFromVehicleTargetPointProvider::TARGET_POINT_ID));
			actionComponent.pushAction<GetDogFromVehicleAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(*rescueDogVehicle, false);

			// Make owner normal again
			actionComponent.pushAction<ReservateTargetAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).initDeleteReservation(vehicleId);
			actionComponent.pushAction<SetInvincibleAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(false);

			// Signal the vehicle that we took the dog out
			qsf::MessageConfiguration message(Messages::EM5_ACTION_SIGNAL, getId());
			actionComponent.pushAction<SignalAction>(action::BLOCKING, qsf::action::APPEND_TO_BACK).init(message);

			// The vehicle should wait for the person has taken the dog out of the trunk door
			qsf::ActionComponent& vehicleActionComponent = rescueDogVehicle->getOrCreateComponentSafe<qsf::ActionComponent>();
			vehicleActionComponent.pushAction<WaitForSignalAction>(action::BLOCKING).init(message, qsf::Time::fromSeconds(10.f));

			// Because the path finding can lead to situations were the rescue dog leader is not able to reach the back of the vehicle and get his dog from vehicle
			// we add an extra safety mechanism with the job to detect this kind of stucking and resolve it by link the rescue dog leader with his dog without animation
			qsf::jobs::JobConfiguration jobConfiguration;
			jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
			mSafetyStuckResolvingJob.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&RescueDogLeaderLogic::updateStuckResolving, this, _1), jobConfiguration);
			mSafetyStuckResolvingTime = qsf::Time::ZERO;
		}
	}

	void RescueDogLeaderLogic::putDogInVehicle(const qsf::MessageParameters& parameters)
	{
		if (Messages::EM5_ON_COMMAND_EXECUTE.getHash() == parameters.getFilter(0) && EnterVehicleCommand::PLUGINABLE_ID == parameters.getFilter(1) &&
			getEntityId() == parameters.getFilter(2))
		{
			// TODO(mk) Currently we reach this code if the dog leaders wants to enter an other vehicle than the rescue dog vehicle!

			if (nullptr == getRescueDog())
			{
				// Do nothing, when the dog doesn't exists anymore. The dog gets deleted, when it enters the car
				return;
			}

			const uint64 vehicleId = parameters.getFilter(3);
			qsf::Entity* rescueDogVehicle = getEntity()->getMap().getEntityById(vehicleId);
			QSF_CHECK(nullptr != rescueDogVehicle, "RescueDogLeaderLogic::putDogInVehicle(): There is no entity with the ID '" << vehicleId << "' for the enter vehicle command of the rescue dog leader '" << getEntityId() << "'", return);

			// Reserve target vehicle for us
			if (!ReserveLogic::checkReservation(*rescueDogVehicle, getEntityId()))
			{
				// Respond no priority at all to signal abort of command execution
				parameters.respond(static_cast<uint32>(action::NONE));
				return;
			}
			ReserveLogic::createReservation<ReserveEnterVehicleLogic>(*rescueDogVehicle, getEntityId());

			// Reset the action priority of the command actions
			parameters.respond(static_cast<uint32>(action::BLOCKING));

			// Push dog leaders actions
			qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

			// Make him normal again
			actionComponent.pushAction<SetInvincibleAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(false);

			// Get the dog
			actionComponent.pushAction<ReservateTargetAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(vehicleId, ReserveEnterVehicleLogic::GAMELOGIC_TYPE_ID);
			actionComponent.pushAction<GetDogFromVehicleAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(*rescueDogVehicle, true);

			// Make him invincible
			actionComponent.pushAction<SetInvincibleAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(true);

			// We push the signal action after the move action so that the vehicle waits when the unit starts putting back its equipment into the vehicle
			pushSignalActionAndSetupMessageHandler(vehicleId, actionComponent);

			// At first, move to target
			actionComponent.pushAction<MoveAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachObjectGoal(*getEntity(), *rescueDogVehicle, GetRescueDogFromVehicleTargetPointProvider::TARGET_POINT_ID));

			// Call the dog, if necessary
			if (mIsDogUnitedWithLeader == false && nullptr != getRescueDog())
			{
				actionComponent.pushAction<WaitForEntityAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(*getRescueDog(), 2.f);	// Here we wait for the rescue dog, wait till the dog and the leader is combined and then enter the vehicle
				actionComponent.pushAction<CancelDogSearchAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT);
			}
		}
	}

	void RescueDogLeaderLogic::dieRescueDog(const qsf::MessageParameters& parameters)
	{
		if (!mRescueDogEntity.valid())
			return;

		// Let the rescue dog die
		qsf::ActionComponent& actionComponent = mRescueDogEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.clearPlan();
		actionComponent.pushAction<qsf::WaitAction>(action::BLOCKING).init(qsf::Time::fromSeconds(0.5f));
		actionComponent.pushAction<DisappearAction>(action::BLOCKING).init(qsf::Time::fromSeconds(1.f));
	}

	void RescueDogLeaderLogic::onContaminated(const qsf::MessageParameters& parameters)
	{
		callRescueDogBack(*getEntity());
	}

	void RescueDogLeaderLogic::updateStuckResolving(const qsf::JobArguments& jobArguments)
	{
		static const qsf::Time STUCKRESOLVING_TIME = qsf::Time::fromSeconds(10.f);

		mSafetyStuckResolvingTime += jobArguments.getTimePassed();
		if (mSafetyStuckResolvingTime >= STUCKRESOLVING_TIME)
		{
			// Job Done
			mSafetyStuckResolvingJob.unregister();

			// Start checking
			if (!mRescueDogEntity.valid())
			{
				// The rescue dog leader hasn't finish his getDogFromVehicleAction correctly
				// -> stuck detection is active: help him via linking him with the dog

				// Force the creating and linking of the rescue dog
				createRescueDogEntity(*getEntity());
				linkDogWithLeader(*getEntity());

				// Cleanup all other states
				getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>().clearPlan();

				AnimationHelper(*getEntity()).clearAnimation();
			}
		}
	}

	void RescueDogLeaderLogic::startWaitForEnterVehicle(const qsf::MessageParameters& parameters)
	{
		// Block the vehicle the entity wants to enter
		uint64 vehicleId = qsf::getUninitialized<uint64>();
		parameters.getParameter("vehicleId", vehicleId);
		qsf::Entity* vehicleEntity = getMap().getEntityById(vehicleId);
		if (nullptr != vehicleEntity)
		{
			qsf::ActionComponent& vehicleActionComponent = vehicleEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

			// If we have already an wait action, take this
			WaitForEnterVehicleAction* waitForEnterVehicleAction = vehicleActionComponent.getAction<WaitForEnterVehicleAction>();
			if (nullptr == waitForEnterVehicleAction)
			{
				waitForEnterVehicleAction = &vehicleActionComponent.pushAction<WaitForEnterVehicleAction>(action::BLOCKING);
			}

			// Init or re-init the wait action
			waitForEnterVehicleAction->init(getEntityId(), qsf::Time::fromSeconds(10.f), true);
		}

		mEnterVehicleWithoutEquipmentProxy.unregister();
		mEnterVehicleWithoutEquipmentAbortProxy.unregister();
	}

	void RescueDogLeaderLogic::abortWaitForEnterVehicle(const qsf::MessageParameters& parameters)
	{
		// The only job here is to unregister the "mEnterVehicleWithoutEquipmentProxy"
		mEnterVehicleWithoutEquipmentProxy.unregister();
		mEnterVehicleWithoutEquipmentAbortProxy.unregister();
	}

	void RescueDogLeaderLogic::pushSignalActionAndSetupMessageHandler(uint64 vehicleEntityId, qsf::ActionComponent& actionComponent)
	{
		// At stuck resolving. Send a signal message and we listen to this message. Block the vehicle we want to enter
		qsf::MessageParameters parameters;
		parameters.setParameter("actorId", getEntityId());
		parameters.setParameter("vehicleId", vehicleEntityId);
		qsf::MessageConfiguration enterMessage(Messages::EM5_LOGIC_ENTER_VEHICLE_WITHOUT_EQUIPMENT, getEntityId());
		qsf::MessageConfiguration abortMessage("abortEnterVehicleWithoutEquipment", getEntityId());
		actionComponent.pushAction<SignalAction>(action::COMMAND_LOW, qsf::action::INSERT_AT_FRONT).init(enterMessage, abortMessage, parameters);

		mEnterVehicleWithoutEquipmentProxy.registerAt(enterMessage, boost::bind(&RescueDogLeaderLogic::startWaitForEnterVehicle, this, _1));
		mEnterVehicleWithoutEquipmentAbortProxy.registerAt(abortMessage, boost::bind(&RescueDogLeaderLogic::abortWaitForEnterVehicle, this, _1));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

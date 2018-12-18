// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/tech/UnloadBoatFromTransporterAction.h"
#include "em5/base/GameplayAssets.h"
#include "em5/command/tech/UnloadBoatFromTransporterCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/movement/SwingComponent.h"
#include "em5/component/objects/BoatDropPointComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/vehicle/parts/VehicleCraneComponent.h"
#include "em5/component/vehicle/parts/SupportLegsComponent.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/game/Game.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/SpecsGroupManager.h"
#include "em5/audio/AudioHelper.h"
#include "em5/audio/AudioProxy.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf/serialization/binary/GlmTypeSerialization.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/physics/collision/CollisionComponent.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/map/Map.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier UnloadBoatFromTransporterAction::ACTION_ID = "em5::UnloadBoatFromTransporterAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UnloadBoatFromTransporterAction::UnloadBoatFromTransporterAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT),
		mSecondsPassed(0.0f)
	{
		// Nothing here
	}

	UnloadBoatFromTransporterAction::~UnloadBoatFromTransporterAction()
	{
		// Nothing here
	}

	void UnloadBoatFromTransporterAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}

	uint64 UnloadBoatFromTransporterAction::getTargetId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void UnloadBoatFromTransporterAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mState);
		serializer.serialize(mDefaultRotation);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool UnloadBoatFromTransporterAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" && "checkContext" of the command for validation
		UnloadBoatFromTransporterCommand* unloadVehicleFromTowcarCommand = static_cast<UnloadBoatFromTransporterCommand*>(QSFGAME_COMMAND.getCommandManager().getById<UnloadBoatFromTransporterCommand>(UnloadBoatFromTransporterCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != unloadVehicleFromTowcarCommand, "UnloadVehicleFromTransporterCommand::onStartup(): Could not find place to unload vehicle by towcar command instance", return false);
		if (!(unloadVehicleFromTowcarCommand->checkContext(commandContext)))
			return false;

		// Check distance
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
		const float maxDistance = 1.0f;				// Hardcoded low maxDistance to ensure boat transporter animation can work

		if (currentDistance >= maxDistance)
			return false;

		return true;
	}

	qsf::action::Result UnloadBoatFromTransporterAction::updateAction(const qsf::Clock& clock)
	{
		// TargetEntity is the dropPoint
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return qsf::action::RESULT_DONE;	// Target entity ec.is not existing, error

		BoatDropPointComponent* boatDropPointComponent = targetEntity->getComponent<BoatDropPointComponent>();
		SupportLegsComponent* supportLegsComponent = getEntity().getComponent<SupportLegsComponent>();
		VehicleCraneComponent* vehicleCraneComponent = getEntity().getComponent<VehicleCraneComponent>();
		qsf::TransformComponent& targetTransformComponent = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
		getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

		if (nullptr != boatDropPointComponent && nullptr != supportLegsComponent && nullptr != vehicleCraneComponent)
		{
			switch (mState)
			{
				case STATE_INIT:
				{
					// Make vehicle invincible
					EntityHelper(getEntity()).setInvincible(true);

					supportLegsComponent->startExtending();

					mState = STATE_SUPPORTLEGS_EXPAND;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_SUPPORTLEGS_EXPAND:
				{
					if (!supportLegsComponent->isAnimationPlaying())
					{
						{ // Try get drop position of the boat from the boat drop point component
							qsf::Entity* entity = getMap().getEntityById(boatDropPointComponent->getBoatEntityId());
							qsf::TransformComponent* boatDropPointTransform = nullptr;
							if (nullptr != entity)
							{
								boatDropPointTransform = entity->getComponent<qsf::TransformComponent>();
							}

							if (nullptr != boatDropPointTransform)
							{
								mDropPosition = boatDropPointTransform->getPosition();

								// We need only the xz position of the point -> set the y position to the value of the transporter entity + offset
								// The offset is needed because the drop position is the position of the crane and not of the boat
								mDropPosition.y = targetTransformComponent.getPosition().y + 2.0f;
							}
							else
							{
								// Fallback to hardcoded way
								mDropPosition = vehicleCraneComponent->worldSpacePositionWithLocalOffset(targetTransformComponent, glm::vec3(1.0f, 2.0f, 5.0f));
							}
						}

						linkBoatToCraneEntity();
						vehicleCraneComponent->setSequenceType(VehicleCraneComponent::SEQUENCE_UNLOAD_BOAT_ALIGN_ARM);
						vehicleCraneComponent->alignArm(mDropPosition);

						mState = STATE_CRANE_TURN_TO_TARGET;
					}
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_CRANE_TURN_TO_TARGET:
				{
					if (!vehicleCraneComponent->isAnimationPlaying())
					{
						float height = 0.0f;
						qsf::GroundMapQuery(getMap(), GroundMaps::FILTER_LIQUIDS_ONLY).getHeightAtXZPosition(mDropPosition.x, mDropPosition.z, height);
						mDropPosition.y += height - targetTransformComponent.getPosition().y;	// To get local offset

						vehicleCraneComponent->alignArm(mDropPosition);

						mState = STATE_CRANE_LOWER_BOAT;
					}
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_CRANE_LOWER_BOAT:
				{
					if (!vehicleCraneComponent->isAnimationPlaying())
					{
						mSecondsPassed = 0.0f;

						mState = STATE_CRANE_WAIT_BEFORE_FALL;
					}
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_CRANE_WAIT_BEFORE_FALL:
				{
					mSecondsPassed += clock.getTimePassed().getSeconds();
					if (mSecondsPassed > 0.5f)
					{
						unLinkBoatEntity(boatDropPointComponent->getLakeId());
						vehicleCraneComponent->setVisibleStrap(false);
						mSecondsPassed = 0.0f;

						mState = STATE_CRANE_WAIT_AFTER_FALL;
					}
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_CRANE_WAIT_AFTER_FALL:
				{
					mSecondsPassed += clock.getTimePassed().getSeconds();
					if (mSecondsPassed > 1.0f)
					{
						vehicleCraneComponent->setSequenceType(VehicleCraneComponent::SEQUENCE_UNLOAD_BOAT_FOLD_ARM);
						vehicleCraneComponent->foldArm();

						mState = STATE_CRANE_TURN_BACK_TO_DEFAULT;
					}
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_CRANE_TURN_BACK_TO_DEFAULT:
				{
					if (!vehicleCraneComponent->isAnimationPlaying())
					{
						supportLegsComponent->hideExtending();

						mState = STATE_SUPPORTLEGS_HIDE;
					}
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_SUPPORTLEGS_HIDE:
				{
					if (supportLegsComponent->isAnimationPlaying())
					{
						// Wait for ending the animation
						return qsf::action::RESULT_CONTINUE;
					}
					// Let entity receive damage again
					EntityHelper(getEntity()).setInvincible(false);

					mState = STATE_DONE;

					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_DONE:
				{
					// Fallthrough by design
				}
			}
		}
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void UnloadBoatFromTransporterAction::linkBoatToCraneEntity()
	{
		RoadVehicleComponent* roadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
		if (nullptr != roadVehicleComponent)
		{
			qsf::Entity* boatEntity = getMap().getEntityById(roadVehicleComponent->getSpecialEntity());
			if (nullptr != boatEntity)
			{
				// add collision box
				qsf::CollisionComponent* collisionComponent = boatEntity->getComponent<qsf::CollisionComponent>();
				if (nullptr != collisionComponent)
					collisionComponent->setActive(true);
				qsf::ai::RouterComponent* routerComponent = boatEntity->getComponent<qsf::ai::RouterComponent>();
				if (nullptr != routerComponent)
					routerComponent->setActive(true);


				VehicleCraneComponent* vehicleCraneComponent = getEntity().getComponent<VehicleCraneComponent>();
				if (nullptr != vehicleCraneComponent)
				{
					qsf::Entity* jibEntity = getFirstChildEntity(getMap().getEntityById(vehicleCraneComponent->getCraneEntityId()));
					qsf::Entity* stageEntity = getFirstChildEntity(jibEntity);
					qsf::Entity* strapEntity = getFirstChildEntity(stageEntity);

					if (nullptr != strapEntity)
					{
						// Create or reset link
						qsf::LinkComponent& linkComponent = boatEntity->getOrCreateComponentSafe<qsf::LinkComponent>();
						linkComponent.setParentId(strapEntity->getId());
						linkComponent.setParentLinkType(qsf::LinkComponent::HARD_LINK);
					}
				}
			}
		}
	}

	void UnloadBoatFromTransporterAction::unLinkBoatEntity(uint64 lakeId)
	{
		RoadVehicleComponent* roadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
		if (nullptr == roadVehicleComponent)
			return;

		qsf::Entity* boatEntity = getMap().getEntityById(roadVehicleComponent->getSpecialEntity());
		if (nullptr == boatEntity)
			return;

		BoatComponent* boatcomponent = boatEntity->getComponent<BoatComponent>();
		if (nullptr == boatcomponent)
			return;

		qsf::LinkComponent* linkComponent = boatEntity->getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return;

		qsf::TransformComponent& transformComponent = boatEntity->getOrCreateComponentSafe<qsf::TransformComponent>();

		// Delete link from boat
		roadVehicleComponent->setSpecialEntity(qsf::getUninitialized<uint64>());

		// Apply placement to get the boat on the water
		linkComponent->linkToParent(getEntity());	// To have correct unlinking we need to have the boat an child of the transporter
		EntityHelper(*boatEntity).leaveContainer(false);

		{ // TODO(sw) An Hack that the boat isn't properly placed, when the player isn't watching the unload process
			glm::vec3 targetBoatPosition = mDropPosition - glm::vec3(0.0f, 2.0f, 0.0f);
			glm::vec3 diff = glm::abs(transformComponent.getPosition() - targetBoatPosition);
			if (diff.x >= 0.5f || diff.y > 0.5f || diff.z >= 0.5f)
			{
				// We have also adjust the rotation
				glm::vec3 eulerAngles = qsf::EulerAngles::quaternionToEuler(transformComponent.getRotation());

				// Fixup pitch and roll: for both no rotation
				eulerAngles.y = 0.0f;
				eulerAngles.z = 0.0f;

				transformComponent.setPositionAndRotation(targetBoatPosition, qsf::EulerAngles::eulerToQuaternion(eulerAngles));
			}
		}

		// Set lake ID
		boatcomponent->setLakeId(lakeId);

		// TODO(sw) This doesn't work in multiplay for the client side...
		EntityHelper(*boatEntity).applyPlacement(true);

		// Play audio
		AudioProxy audioProxy;
		AudioHelper::playBoatUnloadIntoWater(audioProxy, getEntity());
		audioProxy.detach();
		audioProxy.synchronizeWithMultiplay();

		// Activate boat bobbing
		SwingComponent& swingComponent = boatEntity->getOrCreateComponentSafe<SwingComponent>();
		swingComponent.setAnimationType(SwingComponent::BOATBOBBING);
		swingComponent.setAnimationCycle(40.0f);
		swingComponent.setAnimationIntensity(4.0f);
		swingComponent.setVelocitySkew(0.15f);
		swingComponent.setActive(true);

		// Activate navigation component
		qsf::ai::NavigationComponent* navigationComponent = boatEntity->getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setActive(true);
		}

		// Particle effect from fall
		MapHelper(getEntity().getMap()).spawnParticle(assets::PARTICLE_WATERSPRAY2, 2.0f, transformComponent.getPosition());

		// Activate lights
		boatcomponent->enableLights(true);

		// The boat radar should turn after the unload process is finish
		boatcomponent->enableRadar(true);

		// Send message
		sendMessage(boatEntity->getId());

		// Selection handling
		{
			SelectionManager& selectionManager = EM5_GAME.getSelectionManager();
			const bool wasSelected = selectionManager.isIdSelected(getEntityId());
			const bool selectBoat = wasSelected && (selectionManager.getSelectionSize() == 1);

			// Deselect unit if selected
			selectionManager.removeIdFromSelection(getEntityId());

			// In case only the boat was selected, select the transporter after this
			if (selectBoat)
			{
				selectionManager.setSelectionById(boatEntity->getId());
			}

			// If selection changed, an update is needed
			if (wasSelected)
			{
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION));
			}
		}
	}

	qsf::Entity* UnloadBoatFromTransporterAction::getFirstChildEntity(qsf::Entity* entity)
	{
		// Just give back the first child entity
		if (nullptr == entity)
			return nullptr;

		const qsf::LinkComponent* linkComponent = entity->getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return nullptr;

		boost::container::flat_set<qsf::LinkComponent*>::const_iterator iterator = linkComponent->getChildLinks().begin();
		if (iterator == linkComponent->getChildLinks().end())
			return nullptr;

		return &((*iterator)->getEntity());
	}

	void UnloadBoatFromTransporterAction::sendMessage(uint64 entityId) const
	{
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_UNLOAD_VEHICLE_FROM_TRANSPORTER, entityId));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

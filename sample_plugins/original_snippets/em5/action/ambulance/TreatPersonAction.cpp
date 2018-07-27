// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ambulance/TreatPersonAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/CommandContext.h"
#include "em5/command/ambulance/TreatPersonCommand.h"
#include "em5/component/equipment/InventoryComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/debug/GameDebugGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/TreatPersonTargetPointProvider.h"
#include "em5/health/diagnosis/Diagnosis.h"
#include "em5/health/medicalcare/MedicalCare.h"
#include "em5/health/treatmentlogic/ReanimationTreatmentLogic.h"
#include "em5/health/HealthComponent.h"
#include "em5/health/HealthSystem.h"
#include "em5/health/HealthHelper.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/component/base/BoneToBoneLinkComponent.h>
#include <qsf_game/equipment/EquipmentComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_compositing/component/TintableMeshComponent.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier TreatPersonAction::ACTION_ID = "em5::TreatPersonAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TreatPersonAction::TreatPersonAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mMedikitPlaced(false),
		mDoctorIsMovedToMirroredTargetSide(false)
	{
		// Nothing here
	}

	TreatPersonAction::~TreatPersonAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());

		// Clean up treatment logic, give the animation time to do anything important
		deleteTreatmentLogic();

		if (mMedikitPlaced)
		{
			takeMedikit();
		}
	}

	void TreatPersonAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntity = &targetEntity;

		mTargetHealthComponent = mTargetEntity->getComponent<HealthComponent>();

		// To avoid multiple doctors at the same target, we reserve the target here.
		// It is only checked in command for automatism. If the doctor starts treatment, the logic is changed. Only reserve if no other reserve the target (its not bad if a moveTo reservation is missing)
		// Reserve the entity
		if (ReserveLogic::checkReservation(mTargetEntity.getSafe(), getEntityId()))
		{
			ReserveLogic::createReservation<ReserveMoveToTargetLogic>(mTargetEntity.getSafe(), getEntityId());
		}
	}

	uint64 TreatPersonAction::getTargetEntityId() const
	{
		return mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>();
	}

	uint64 TreatPersonAction::getMedikitId() const
	{
		return mMedikitEntity.valid() ? mMedikitEntity->getId() : qsf::getUninitialized<uint64>();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void TreatPersonAction::serialize(qsf::BinarySerializer& serializer)
	{
		if (serializer.isWriting())
		{
			uint64 targetEntityId = getTargetEntityId();
			serializer.serialize(targetEntityId);

			uint64 medikitEntityId = getMedikitId();
			serializer.serialize(medikitEntityId);

			// We can't save the health component, so we take it from the entity after loading
		}
		else
		{
			qsf::Map& map = getEntity().getMap();

			uint64 targetEntityId;
			serializer.serialize(targetEntityId);
			mTargetEntity = map.getEntityById(targetEntityId);

			uint64 medikitEntityId;
			serializer.serialize(medikitEntityId);
			mMedikitEntity = map.getEntityById(medikitEntityId);

			mTargetHealthComponent = mTargetEntity->getComponent<HealthComponent>();
		}

		serializer.serialize(mMedikitPlaced);
		serializer.serialize(mDoctorIsMovedToMirroredTargetSide);
		serializer.serialize(mOptimalDoctorPosition);
		serializer.serializeAs<uint16>(mState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool TreatPersonAction::onStartup()
	{
		qsf::Entity& callerEntity = getEntity();
		if (!mTargetEntity.valid())
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getTreatPersonDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(mTargetEntity.getSafe(), callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		TreatPersonCommand* treatPersonCommand = static_cast<TreatPersonCommand*>(QSFGAME_COMMAND.getCommandManager().getById<TreatPersonCommand>(TreatPersonCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != treatPersonCommand, "TreatPersonAction::onStartup(): Could not find treat person command instance", return false);
		if (!(treatPersonCommand->checkCaller(*commandContext.mCaller) && treatPersonCommand->checkContext(commandContext)))
			return false;

		// Check if an other doctor was faster
		if (mTargetEntity->getComponentSafe<HealthComponent>().isTreatmentActive())
			return false;

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(mTargetEntity.getSafe(), getEntityId()))
			return false;

		// Try to get event id component lock
		if (!EventIdComponent::tryLockForTeam(mTargetEntity.getSafe(), getEntity()))
		{
			return false;
		}

		// Mark this patient as being treated right now
		mTargetEntity->getComponentSafe<HealthComponent>().setTreatmentActive(true);

		// Success
		return true;
	}

	qsf::action::Result TreatPersonAction::updateAction(const qsf::Clock&)
	{
		// Validate the target
		if (!mTargetEntity.valid())
		{
			// Error
			return qsf::action::RESULT_DONE;
		}

		// Validate the target health component
		if (!mTargetHealthComponent.valid())
		{
			// Error
			return qsf::action::RESULT_DONE;
		}

		// Validate the medikit
		if (mMedikitPlaced && !mMedikitEntity.valid())
		{
			// Error
			return qsf::action::RESULT_DONE;
		}

		// Update debug output
		if (GameDebugGroup::getInstanceSafe().getShowHealthState())
		{
			showDebug();
		}
		else
		{
			mDebugDrawProxy.unregister();
		}

		switch (mState)
		{
			case STATE_INIT:
			{
				// Is person injured
				if (!mTargetHealthComponent->isInjured())
				{
					// Not injured! Nothing to do!
					return qsf::action::RESULT_DONE;
				}

				// Change reserve logic from "moveto" to "doctor" (has higher priority)
				ReserveLogic::deleteOwnReservation(getMap(), mTargetEntity->getId(), getEntityId());
				ReserveLogic::createReservation<ReserveDoctorLogic>(mTargetEntity.getSafe(), getEntityId());

				mOptimalDoctorPosition = calculateOptimalDoctorPosition();
				checkAndMoveDoctorToOptimalPosition();
				bringTargetInPosition();

				// Pushed animation via insert, so we see them at last all pushed actions
				PlayAnimationAction& playAnimationAction = getComponent().pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_BEFORE_CURRENT);
				playAnimationAction.init(AnimationHelper(getEntity()).getAnimationDoctorGetDown(), true);
				// TODO(mk) To solve the problem with flying medi cases, we enforce the update here, to the animation is played in every case.
				// Unfortunately the work around isn't working...
				playAnimationAction.setEnforceGraphicsSynchronization(true);

				mState = STATE_GET_DOWN;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_GET_DOWN:
			{
				placeMedikit();
				openMedikit();

				// Play audio
				AudioProxy audioProxy;
				AudioHelper::playDoctorOpenCase(audioProxy, getEntity());
				audioProxy.detach();
				audioProxy.synchronizeWithMultiplay();

				mState = STATE_OPEN_MEDIKIT;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_OPEN_MEDIKIT:
			{
				checkAndMoveTargetToOptimalPosition();
				setGloves(true);

				mState = STATE_DIAGNOSIS;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DIAGNOSIS:
			{
				startDoctorDiagnosis();

				mState = STATE_RUN_DIAGNOSIS;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_RUN_DIAGNOSIS:
			{
				// Get the game logic component
				if (isTreatmentLogicActive())
				{
					// Treatment is active, wait
					return qsf::action::RESULT_CONTINUE;
				}

				stopDoctorDiagnosis();

				// Register the player as "has interacted with" event object
				EventIdComponent::registerPlayer(*mTargetEntity, getEntity());

				if (checkReanimationState())
				{
					mTargetHealthComponent->updateInjuredIcon();

					if (mTargetHealthComponent->getState() == HealthComponent::STATE_DEAD)
					{
						// Emit message "Finish Treat Person"
						// Because we have observers, waiting for treating the persons, we send a message here to avoid ending up with open objectives
						QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_HEALTH_TREATED_PERSON, mTargetEntity->getId()));
					}

					mState = STATE_REANIMATE_START;
					return qsf::action::RESULT_CONTINUE;
				}
				startDoctorMedicalCare();

				mState = STATE_HEAL;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_HEAL:
			{
				// For safety set the medical care active flag every tick
				mTargetHealthComponent->setMedicalCareActive(true);

				if (isDoctorMedicalCareFinished())
				{
					stopDoctorMedicalCare();

					// Remove reservation here already, so paramedics can pickup the victim now
					ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());

					// Finish the action
					mState = STATE_CLOSE_MEDIKIT;
				}

				// Continue healing
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_REANIMATE_START:
			{
				startReanimationTreatment();

				mState = STATE_REANIMATE;

				// Fall through by design
			}

			case STATE_REANIMATE:
			{
				// Now reanimating the patient, the setting of the reanimation flag is done by the reanimation treatment
				if (mTargetHealthComponent->getDeathEnergy() >= mTargetHealthComponent->getMaximumDeathEnergy() ||
					mTargetHealthComponent->getState() == HealthComponent::STATE_DEAD)
				{
					stopReanimationTreatment();

					if (isTreatmentLogicActive())
					{
						// Wait till the reanimation is finished correctly
						return qsf::action::RESULT_CONTINUE;
					}

					// Reanimation succeeded, start with diagnosis
					mTargetHealthComponent->setReanimationActive(false);

					if (mTargetHealthComponent->getState() == HealthComponent::STATE_DEAD)
					{
						// In case the target is dead, end doctors treatment
						mState = STATE_CLOSE_MEDIKIT;
					}
					else
					{
						mState = STATE_RUN_DIAGNOSIS;
					}
				}

				// Continue reanimation
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CLOSE_MEDIKIT:
			{
				if (AnimationHelper(getEntity()).isAnimationPlaying())
				{
					// Wait till animation is finished
					return qsf::action::RESULT_CONTINUE;
				}

				// The treatment logic is deleted correctly, delete it here again for being sure
				deleteTreatmentLogic();

				// Adding stand up animation here for an cleaner look
				closeMedikit();
				setGloves(false);

				// Play audio
				AudioProxy audioProxy;
				AudioHelper::playDoctorCloseCase(audioProxy, getEntity());
				audioProxy.detach();
				audioProxy.synchronizeWithMultiplay();

				mState = STATE_GET_UP;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_GET_UP:
			{
				if (AnimationHelper(getEntity()).isAnimationPlaying())
				{
					// Wait till animation is finished
					return qsf::action::RESULT_CONTINUE;
				}

				takeMedikit();

				// Pushed animation via insert, so we see them at last all pushed actions
				getComponent().pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationDoctorGetDown(), true, false, true);

				// Finished with everything, return done
				return qsf::action::RESULT_DONE;
			}
		}

		// Run action in a loop, till the target is fully healed
		return qsf::action::RESULT_CONTINUE;
	}

	void TreatPersonAction::onShutdown()
	{
		setGloves(false);

		if (mTargetEntity.valid())
		{
			EventIdComponent::releaseTeamLock(mTargetEntity.getSafe(), getEntity());
		}

		if (mTargetHealthComponent.valid())
		{
			mTargetHealthComponent->setDiagnosisActive(false);
			mTargetHealthComponent->setMedicalCareActive(false);
			mTargetHealthComponent->setReanimationActive(false);
			mTargetHealthComponent->setTreatmentActive(false);
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	glm::vec3 TreatPersonAction::calculateOptimalDoctorPosition(bool originalSide) const
	{
		std::vector<qsf::logic::TargetPoint> targetPoints;

		const qsf::Transform& targetTransform = EntityHelper(mTargetEntity.getSafe()).getTransform();
		TreatPersonTargetPointProvider::calculateTargetPoints_static(mTargetEntity.getSafe(), targetTransform, targetPoints, originalSide);

		return targetPoints[0].mPosition;
	}

	void TreatPersonAction::checkAndMoveDoctorToOptimalPosition()
	{
		// Here we try to fix problems with the path finding. Because we have very detailed animations we need the right position of the doctor and the target.
		// First step is check if the doctor is at the wrong side of the target, in this case we move the doctor to the mirrored, optimal side of the target
		qsf::TransformComponent& ownerTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		const glm::vec3& ownerPosition = ownerTransformComponent.getPosition();
		if (ownerPosition != mOptimalDoctorPosition)
		{
			// We are not in the position as we should, check if we have to we have to move our doctor
			const qsf::TransformComponent& targetTransformComponent = mTargetEntity->getComponentSafe<qsf::TransformComponent>();

			// The rotation of the target to the right
			const glm::vec3 rightDirectionInWorldSpace = targetTransformComponent.getRotation() * qsf::CoordinateSystem::getRight();

			// Check if doctor is on the wrong side from the target
			const glm::vec3& targetPosition = targetTransformComponent.getPosition();
			if (glm::dot(rightDirectionInWorldSpace, (ownerPosition - targetPosition)) < 0.0f)
			{
				// Doctor is on the wrong side of the target. Move him to the opposite (left side) side of the target.
				// So we can optimally turn the target at 180°
				const glm::vec3 newPosition = calculateOptimalDoctorPosition(false);

				// Create the static target point for the AI to move towards the new Position
				// Push one extra movement to reach the mirrored side of the target, maybe this has more success
				qsf::logic::TargetPoint moveTargetConfig;
				moveTargetConfig.mPosition = newPosition;
				getComponent().pushAction<MoveAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig));

				// Remember this for future handling
				mDoctorIsMovedToMirroredTargetSide = true;
			}
		}

		// Calculate the final doctor rotation and give it by hand (not via target point as usual)
		const qsf::Transform& targetTransform = EntityHelper(mTargetEntity.getSafe()).getTransform();
		const glm::quat newDoctorRotation = TreatPersonTargetPointProvider::calculateRotation_static(targetTransform, !mDoctorIsMovedToMirroredTargetSide);

		// Turn to target via action. So the doctor has time to move and is rotate after moving is finished.
		getComponent().pushAction<TurnToAction>(getPriority(), qsf::action::INSERT_BEFORE_CURRENT).initWithRotation(newDoctorRotation);
	}

	void TreatPersonAction::checkAndMoveTargetToOptimalPosition() const
	{
		// Here we try to fix problems with the path finding. Because we have very detailed animations we need the right position of the doctor and the target.
		// In this case we repositioning the target to the doctor
		qsf::TransformComponent& ownerTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		qsf::TransformComponent& targetTransformComponent = mTargetEntity->getComponentSafe<qsf::TransformComponent>();
		if (mDoctorIsMovedToMirroredTargetSide)
		{
			// Turn target
			const glm::quat newRotation = targetTransformComponent.getRotation() * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::pi<float>());
			targetTransformComponent.setRotation(newRotation);

			// Get an 2D (x,z) rotation
			const glm::quat rotationInXZPlane = qsf::Math::getRotationByUpVector(qsf::CoordinateSystem::getUp(), newRotation);

			// The offset of the target to the right
			float targetOffsetX = 0.0f, targetOffsetZ = 0.0f;
			TreatPersonTargetPointProvider::calculateOffsets(*mTargetEntity, targetOffsetX, targetOffsetZ);

			const glm::vec3 positionOffsetX = rotationInXZPlane * qsf::CoordinateSystem::getRight() * -targetOffsetX;
			const glm::vec3 positionOffsetZ = rotationInXZPlane * qsf::CoordinateSystem::getIn() * -targetOffsetZ;

			const glm::vec3& ownerPosition = EntityHelper(getEntity()).getPosition();
			glm::vec3 newTargetPosition = ownerPosition + (positionOffsetX + positionOffsetZ);

			// Apply placement and ensure owner and target are at the highest y-position (no patients inside the ground, please)
			EntityHelper(*mTargetEntity).applyPlacementToPosition(newTargetPosition);
			if (newTargetPosition.y > ownerPosition.y)
			{
				// Patient wins, doctor has to update its y-position
				ownerTransformComponent.setPosition(glm::vec3(ownerPosition.x, newTargetPosition.y, ownerPosition.z));
			}
			else
			{
				// Doctor wins, patient has to update its y-position
				newTargetPosition.y = ownerPosition.y;
			}

			// Set new position
			targetTransformComponent.setPosition(newTargetPosition);
		}
		else if (ownerTransformComponent.getPosition() != mOptimalDoctorPosition)
		{
			// We are not in the position as we should, we have to move our target
			const glm::vec3& targetPosition = targetTransformComponent.getPosition();
			const glm::vec3 distance = targetPosition - mOptimalDoctorPosition;

			const glm::vec3& ownerPosition = EntityHelper(getEntity()).getPosition();
			glm::vec3 newTargetPosition = ownerPosition + distance;

			// Apply placement and ensure owner and target are at the highest y-position (no patients inside the ground, please)
			EntityHelper(*mTargetEntity).applyPlacementToPosition(newTargetPosition);
			if (newTargetPosition.y > ownerPosition.y)
			{
				// Patient wins, doctor has to update its y-position
				ownerTransformComponent.setPosition(glm::vec3(ownerPosition.x, newTargetPosition.y, ownerPosition.z));
			}
			else
			{
				// Doctor wins, patient has to update its y-position
				newTargetPosition.y = ownerPosition.y;
			}

			// Set new position
			targetTransformComponent.setPosition(newTargetPosition);
		}

		// Owner and target are in the right position
	}

	void TreatPersonAction::placeMedikit()
	{
		const EntityHelper entityHelper(getEntity());
		if (entityHelper.checkIsActiveEquipment(equipment::AMBULANCE_MEDIKIT))
		{
			mMedikitPlaced = true;
			const qsf::game::EquipmentComponent* equipmentComponent = entityHelper.getActiveEquipmentComponent();
			if (nullptr != equipmentComponent)
			{
				mMedikitEntity = equipmentComponent->getEntity();
				if (mMedikitEntity.valid())
				{
					// To be sure, delete both kinds of possible links
					mMedikitEntity->destroyComponent<qsf::game::BoneToBoneLinkComponent>();
					mMedikitEntity->destroyComponent<qsf::game::BoneLinkComponent>();

					// Destroy parent link
					qsf::LinkComponent* linkComponent = mMedikitEntity->getComponent<qsf::LinkComponent>();
					if (nullptr != linkComponent)
					{
						linkComponent->setParentId(qsf::getUninitialized<uint64>());
					}

					// TODO(mk) Delete from inventory
				}
			}
		}
	}

	void TreatPersonAction::takeMedikit()
	{
		mMedikitPlaced = false;

		if (mMedikitEntity.valid())
		{
			// Now we have to change the meshes back again
			qsf::MeshComponent* meshComponent = mMedikitEntity->getOrCreateComponent<qsf::MeshComponent>();
			meshComponent->setMesh(qsf::AssetProxy(equipment::MESH_MEDIKIT_CLOSE.getHash()));

			InventoryComponent& inventoryComponent = getEntity().getOrCreateComponentSafe<InventoryComponent>();
			inventoryComponent.setActiveEquipmentId(mMedikitEntity->getId());

			EntityHelper(mMedikitEntity.getSafe()).linkEquipmentToEntity(getEntity());
		}

		// Clear animation
		AnimationHelper(getEntity()).clearAnimation();
	}

	void TreatPersonAction::openMedikit() const
	{
		// Push open medikit animation to the doctor
		getComponent().pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationDoctorOpenMedicCase(), true);

		// Open the medikit via animation for the equipment
		if (!mMedikitEntity.valid())
			return;

		// TODO(mk) 2. Work around for placing the medi case, this here is not enough and ignores the rotation (I spend a lot of time, but i don't get it) but is helps to place the medi case on the ground
		{
			static const glm::vec3 MEDICASE_OFFSET(0.160f, 0.213f, -0.193f);
			static const glm::vec3 MEDICASE_OFFSET_ANGLES(glm::radians(-168.780f), glm::radians(5.048f), glm::radians(-175.924f));
			static glm::quat MEDICASE_ROTATION = qsf::EulerAngles::eulerToQuaternion(MEDICASE_OFFSET_ANGLES);

			qsf::Transform ownerTransform = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getTransform();
			glm::vec3 finalPosition(ownerTransform.vec3PositionLocalToWorld(MEDICASE_OFFSET));
			glm::quat finalRotation(ownerTransform.quatRotationLocalToWorld(MEDICASE_ROTATION));

			// Give the new position to the medicase
			qsf::TransformComponent& transformComponent = mMedikitEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			transformComponent.setPosition(finalPosition);
			transformComponent.setRotation(finalRotation);
		}

		// Now we have to change the closed medikit with the open medikit; switch the current mesh with the open variant
		qsf::MeshComponent* meshComponent = mMedikitEntity->getComponent<qsf::MeshComponent>();
		if (nullptr != meshComponent)
		{
			meshComponent->setMesh(qsf::AssetProxy(equipment::MESH_MEDIKIT_OPEN.getHash()));
		}

		qsf::ActionComponent* actionComponent = mMedikitEntity->getOrCreateComponent<qsf::ActionComponent>();
		if (nullptr == actionComponent)
			return;

		// Push open medikit animation to the equipments
		actionComponent->pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationEquipmentOpenMedikit());
	}

	void TreatPersonAction::closeMedikit() const
	{
		// This is the same as the openMedikit(), just all animations and processes reverted
		// Push close medikit animation to the doctor
		getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationDoctorOpenMedicCase(), true, false, true);

		// Open the medikit via animation for the equipment
		if (!mMedikitEntity.valid())
			return;

		qsf::ActionComponent* actionComponent = mMedikitEntity->getOrCreateComponent<qsf::ActionComponent>();
		if (nullptr == actionComponent)
			return;

		// Push close medikit animation to the equipments
		actionComponent->pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationEquipmentCloseMedikit());
	}

	bool TreatPersonAction::checkReanimationState() const
	{
		// First reanimate?
		const HealthComponent::State currentHealthState = mTargetHealthComponent->getState();
		return (currentHealthState == HealthComponent::STATE_DEATHFIGHT || currentHealthState == HealthComponent::STATE_DEAD);
	}

	void TreatPersonAction::bringTargetInPosition() const
	{
		// Special handling for kneeing person, put them slowly down
		std::string lightAnimation;
		std::string transitAnimation;
		PersonComponent::BoundingBoxType boundingBoxType = PersonComponent::BOX_STANDING;

		AnimationHelper animationHelper(mTargetEntity.getSafe());
		HealthHelper healthHelper(mTargetEntity.getSafe());
		if (mTargetHealthComponent->isHealthy())
		{
			// Handling standing persons
			if (healthHelper.isKneeingInjury())
			{
				transitAnimation = animationHelper.getAnimationKneeingToStanding();
			}
			lightAnimation = animationHelper.getAnimationGenericLightStanding();
			boundingBoxType = PersonComponent::BOX_STANDING;
		}
		else
		{
			// Handle lying persons (dead persons need no transition animation)
			if (healthHelper.isKneeingInjury() && !mTargetHealthComponent->isDead())
			{
				transitAnimation = animationHelper.getAnimationKneeingToLying();
			}
			lightAnimation = animationHelper.getAnimationGenericLightLying();
			boundingBoxType = PersonComponent::BOX_LYING;
		}

		qsf::ActionComponent& targetActionComponent = mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>();

		// To minimize errors with animation, we give an light animation
		targetActionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(lightAnimation, false, true);

		// The transit animation puts the target from an kneeing position to the final position
		if (!transitAnimation.empty())
		{
			targetActionComponent.pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(transitAnimation, true);
		}

		// Update the bounding box (especially for the case that we change from kneeing to lying)
		PersonComponent* personComponent = mTargetEntity->getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			personComponent->setBoundingBoxType(boundingBoxType);
		}
	}

	void TreatPersonAction::startDoctorDiagnosis() const
	{
		// Start diagnosis only once
		if (!mTargetHealthComponent->isDiagnosisComplete())
		{
			mTargetHealthComponent->setDiagnosisActive(true);
			const Diagnosis* diagnosis = EM5_HEALTH.getDiagnosisById(mTargetHealthComponent->getDiagnosisId());
			if (nullptr != diagnosis)
			{
				const std::string& treatmentLogicName = diagnosis->getTreatmentLogicName();

				qsf::Entity& ownerEntity = getEntity();
				qsf::GameLogicComponent& gameLogicComponent = ownerEntity.getOrCreateComponentSafe<qsf::GameLogicComponent>();
				BaseTreatmentLogic* baseTreatmentLogic = static_cast<BaseTreatmentLogic*>(gameLogicComponent.createGameLogicByTypeId(qsf::StringHash(treatmentLogicName)));

				baseTreatmentLogic->initWithDiagnosis(mTargetEntity.getSafe(), ownerEntity, *diagnosis);
			}
		}
	}

	void TreatPersonAction::stopDoctorDiagnosis() const
	{
		// Diagnosis finished
		mTargetHealthComponent->setDiagnosisActive(false);
		mTargetHealthComponent->setDiagnosisComplete(true);

		deleteTreatmentLogic();
	}

	void TreatPersonAction::startDoctorMedicalCare() const
	{
		bool useFallback = true;
		const Injury* injury = mTargetHealthComponent->getInjury();
		if (nullptr != injury)
		{
			const std::vector<uint32>& medicalCareIds = injury->MedicalCareArray.getVariable();
			if (!medicalCareIds.empty())
			{
				// We support only one medical care
				const MedicalCare* medicalCare = EM5_HEALTH.getMedicalCareById(medicalCareIds[0]);
				if (nullptr != medicalCare)
				{
					qsf::Entity& ownerEntity = getEntity();
					qsf::GameLogicComponent& gameLogicComponent = ownerEntity.getOrCreateComponentSafe<qsf::GameLogicComponent>();

					const std::string& treatmentLogicName = medicalCare->getTreatmentLogicName();
					BaseTreatmentLogic* baseTreatmentLogic = static_cast<BaseTreatmentLogic*>(gameLogicComponent.createGameLogicByTypeId(qsf::StringHash(treatmentLogicName)));
					if (nullptr != baseTreatmentLogic)
					{
						// Start treatment
						useFallback = false;
						baseTreatmentLogic->initWithMedicalCare(mTargetEntity.getSafe(), ownerEntity, *medicalCare);
					}
				}
			}
		}

		if (useFallback)
		{
			// This is the fallback for missing medical care
			getComponent().pushAction<PlayAnimationAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationDoctorMedicalCare(), false, true);
		}

		// Now healing the patient
		mTargetHealthComponent->setMedicalCareActive(true);
	}

	bool TreatPersonAction::isDoctorMedicalCareFinished() const
	{
		// Choose current case
		if (mTargetHealthComponent->getHealthEnergy() > 0.0f)
		{
			if (mTargetHealthComponent->getHealthEnergy() >= mTargetHealthComponent->getMaximumHealthEnergy())
			{
				return true;
			}
		}
		else
		{
			if (mTargetHealthComponent->getLifeEnergy() >= mTargetHealthComponent->getMaximumLifeEnergy())
			{
				return true;
			}
		}

		// Medical is not finished, return false
		return false;
	}

	void TreatPersonAction::stopDoctorMedicalCare() const
	{
		// Finish treatment
		qsf::GameLogicComponent* gameLogicComponent = getEntity().getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			BaseTreatmentLogic* baseTreatmentLogic = gameLogicComponent->getGameLogic<BaseTreatmentLogic>();
			if (nullptr != baseTreatmentLogic)
			{
				baseTreatmentLogic->playFinishAnimation();
			}
		}

		mTargetHealthComponent->doctorTreatmentFinished();
	}

	void TreatPersonAction::startReanimationTreatment() const
	{
		// Use the reanimation treatment class for all visual effects and animation handling
		qsf::GameLogicComponent& gameLogicComponent = getEntity().getOrCreateComponentSafe<qsf::GameLogicComponent>();
		ReanimationTreatmentLogic* reanimationTreatmentLogic = static_cast<ReanimationTreatmentLogic*>(gameLogicComponent.createGameLogicByTypeId(ReanimationTreatmentLogic::GAMELOGIC_TYPE_ID));
		if (nullptr != reanimationTreatmentLogic)
		{
			reanimationTreatmentLogic->init(getEntity(), mTargetEntity.getSafe());
		}
	}

	void TreatPersonAction::stopReanimationTreatment() const
	{
		qsf::GameLogicComponent* gameLogicComponent = getEntity().getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			ReanimationTreatmentLogic* reanimationTreatmentLogic = gameLogicComponent->getGameLogic<ReanimationTreatmentLogic>();
			if (nullptr != reanimationTreatmentLogic)
			{
				// Stop the treatment slowly, finish the animation, let the doctor put everything back in the medic case
				reanimationTreatmentLogic->stopReanimation();
			}
		}
	}

	bool TreatPersonAction::isTreatmentLogicActive() const
	{
		qsf::GameLogicComponent* gameLogicComponent = getEntity().getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			// Return true if doctor handles base or derived treatment logic
			return nullptr != gameLogicComponent->getGameLogic<BaseTreatmentLogic>();
		}

		// Error, game logic component must exist, return false to don't block other processes
		return false;
	}

	void TreatPersonAction::deleteTreatmentLogic() const
	{
		qsf::GameLogicComponent* gameLogicComponent = getEntity().getComponent<qsf::GameLogicComponent>();
		if (nullptr != gameLogicComponent)
		{
			BaseTreatmentLogic* baseTreatmentLogic = gameLogicComponent->getGameLogic<BaseTreatmentLogic>();
			if (nullptr != baseTreatmentLogic)
			{
				gameLogicComponent->deleteGameLogic(*baseTreatmentLogic);
			}
		}
	}

	void TreatPersonAction::setGloves(bool hasGloves) const
	{
		qsf::compositing::TintableMeshComponent* tintableMeshComponent = getEntity().getComponent<qsf::compositing::TintableMeshComponent>();
		if (nullptr != tintableMeshComponent)
		{
			tintableMeshComponent->setTintPaletteIndex(hasGloves ? 0 : 1);
		}
	}

	void TreatPersonAction::showDebug()
	{
		const qsf::TransformComponent* transformComponent = getEntity().getComponent<qsf::TransformComponent>();
		if (nullptr != transformComponent)
		{
			// Get the current state as string
			std::string state;
			switch (mState)
			{
				case STATE_INIT:
					state = "INIT";
					break;

				case STATE_GET_DOWN:
					state = "GET_DOWN";
					break;

				case STATE_OPEN_MEDIKIT:
					state = "OPEN_MEDIKIT";
					break;

				case STATE_DIAGNOSIS:
				case STATE_RUN_DIAGNOSIS:
					state = "DIAGNOSIS";
					break;

				case STATE_HEAL:
					state = "HEAL";
					break;

				case STATE_REANIMATE_START:
				case STATE_REANIMATE:
					state = "REANIMATE";
					break;

				case STATE_CLOSE_MEDIKIT:
				case STATE_GET_UP:
					// Nothing to do in this case
					break;
			}

			// Draw the state
			mDebugDrawProxy.registerAt(getMap().getDebugDrawManager());
			mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest(state, transformComponent->getPosition()));
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

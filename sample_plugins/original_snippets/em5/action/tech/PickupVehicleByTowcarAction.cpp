// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/tech/PickupVehicleByTowcarAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/base/ContainerCategory.h"
#include "em5/command/tech/PickupVehicleByTowcarCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/command/move/ReturnToBaseCommand.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/component/vehicle/parts/VehicleCraneComponent.h"
#include "em5/component/vehicle/parts/SupportLegsComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/component/event/EventTagComponent.h>
#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/environment/weather/SnowComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf_ai/navigation/NavigationComponent.h>
#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/math/Math.h>
#include <qsf/math/EulerAngles.h>
#include <qsf/map/Map.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/serialization/binary/GlmTypeSerialization.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Locally used helper functions                         ]
	//[-------------------------------------------------------]
	namespace
	{
		glm::quat getPlacementRotation(const qsf::TransformComponent& wreckTransformComponent, const qsf::TransformComponent& baseTransformComponent)
		{
			float diffAngle = acos((wreckTransformComponent.getRotation() * glm::inverse(baseTransformComponent.getRotation())).w) * 2.0f;
			diffAngle = (diffAngle > glm::pi<float>()) ? (diffAngle - glm::pi<float>() * 2.0f) : diffAngle;

			// Adjust local rotation if needed
			glm::quat rotation = qsf::Math::GLM_QUAT_IDENTITY;
			if (glm::abs(diffAngle) > glm::half_pi<float>())
			{
				rotation = qsf::EulerAngles::eulerToQuaternion(glm::pi<float>(), 0.0f, 0.0f);
			}

			return rotation;
		}
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PickupVehicleByTowcarAction::ACTION_ID = "em5::PickupVehicleByTowcarAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PickupVehicleByTowcarAction::PickupVehicleByTowcarAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCoverEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT),
		mUseCover(false),
		mSecondsPassed()
	{
		// Nothing here
	}

	PickupVehicleByTowcarAction::~PickupVehicleByTowcarAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
	}

	void PickupVehicleByTowcarAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
		mCoverEntityId = getCoverEntityId();

		// To avoid multiple actors at the same target, we reserve the target here.
		// It is only checked in command for automatism. If the action starts, the logic is changed. Only reserve if no other reserve the target (its not bad if a moveTo reservation is missing)
		// Reserve the entity
		if (ReserveLogic::checkReservation(targetEntity, getEntityId()))
		{
			ReserveLogic::createReservation<ReserveMoveToTargetLogic>(targetEntity, getEntityId());
		}
	}

	uint64 PickupVehicleByTowcarAction::getTargetId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PickupVehicleByTowcarAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serialize(mCoverEntityId);
		serializer.serializeAs<uint8>(mState);
		serializer.serialize(mDefaultRotation);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PickupVehicleByTowcarAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Check distance
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, getEntity());
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getTowcarDistance();
		if (currentDistance >= maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" && "checkContext" of the commando for validation
		PickupVehicleByTowcarCommand* pickupVehicleByTowcarCommand = static_cast<PickupVehicleByTowcarCommand*>(QSFGAME_COMMAND.getCommandManager().getById<PickupVehicleByTowcarCommand>(PickupVehicleByTowcarCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != pickupVehicleByTowcarCommand, "PickupVehicleByTowcarAction::onStartup(): Could not find place pickup vehicle by towcar command instance", return false);
		if (!(pickupVehicleByTowcarCommand->checkContext(commandContext)))
			return false;

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(*targetEntity, getEntityId()))
			return false;

		// Try to get event id component lock
		if (!EventIdComponent::tryLockForTeam(*targetEntity, callerEntity))
		{
			return false;
		}

		return true;
	}

	qsf::action::Result PickupVehicleByTowcarAction::updateAction(const qsf::Clock& clock)
	{
		VehicleCraneComponent* vehicleCraneComponent = getEntity().getComponent<VehicleCraneComponent>();
		SupportLegsComponent* supportLegsComponent = getEntity().getComponent<SupportLegsComponent>();
		qsf::Entity* wreckEntity = getMap().getEntityById(mTargetEntityId);
		qsf::Entity* coverEntity = getMap().getEntityById(mCoverEntityId);

		if (nullptr != wreckEntity && nullptr != coverEntity && nullptr != supportLegsComponent && nullptr != vehicleCraneComponent)
		{
			qsf::TransformComponent& towcarTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
			qsf::TransformComponent& wreckTransformComponent = wreckEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			qsf::TransformComponent& coverTransformComponent = coverEntity->getOrCreateComponentSafe<qsf::TransformComponent>();

			switch (mState)
			{
				case STATE_INIT:
				{
					const float tolerance = glm::pi<float>() / 8.0f;
					const float yaw = towcarToWreckYaw(towcarTransformComponent.getTransform(), wreckTransformComponent.getTransform());

					// Rotate towcar to stand parallel and with its heck towards the wreck
					const glm::vec3 towcarEuler = qsf::EulerAngles::quaternionToEuler(towcarTransformComponent.getRotation());
					const glm::vec3 wreckEuler = qsf::EulerAngles::quaternionToEuler(wreckTransformComponent.getRotation());

					const glm::quat rotationForward = qsf::EulerAngles::eulerToQuaternion(wreckEuler.x, towcarEuler.y, towcarEuler.z);
					const glm::quat rotationBackward = rotationForward * qsf::EulerAngles::eulerToQuaternion(glm::pi<float>(), 0.0f, 0.0f);
					const glm::quat rotation = (yaw < glm::half_pi<float>() && yaw > -glm::half_pi<float>()) ? rotationBackward : rotationForward;

					float diffAngleForward = acos((rotationForward * glm::inverse(towcarTransformComponent.getRotation())).w) * 2.0f;
					diffAngleForward = diffAngleForward > glm::pi<float>() ? diffAngleForward - glm::pi<float>() * 2.0f : diffAngleForward;

					float diffAngleBackward = acos((rotationBackward * glm::inverse(towcarTransformComponent.getRotation())).w) * 2.0f;
					diffAngleBackward = diffAngleBackward > glm::pi<float>() ? diffAngleBackward - glm::pi<float>() * 2.0f : diffAngleBackward;

					// The towcar is on the left or right side of the wreck
					const bool isOnSide = glm::abs(yaw) > glm::half_pi<float>() - tolerance && glm::abs(yaw) < glm::half_pi<float>() + tolerance;

					// The towcar is parallel to the wreck
					const bool isParallel = glm::abs(diffAngleForward) < tolerance || glm::abs(diffAngleBackward) < tolerance;

					// We only use the new rotation we are not the side
					if (!(isOnSide && isParallel))
					{
						towcarTransformComponent.setRotation(rotation);
					}

					EntityHelper(getEntity()).applyPlacement();

					// Change reserve logic from "moveto" to "towcar" (has higher priority)
					ReserveLogic::deleteOwnReservation(getMap(), mTargetEntityId, getEntityId());
					ReserveLogic::createReservation<ReserveTowcarLogic>(*wreckEntity, getEntityId());

					supportLegsComponent->startExtending();

					// Make target immune to fire
					wreckEntity->destroyComponent<FireComponent>();

					StatusOverlayComponent::hideIcon(*wreckEntity, StatusOverlayComponent::STATUS_ICON_DEFECT_VEHICLE);

					mState = STATE_SUPPORTLEGS_EXPAND;

					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_SUPPORTLEGS_EXPAND:
				{
					if (!supportLegsComponent->isAnimationPlaying())
					{
						exchangeTargetWithCover(*wreckEntity, *coverEntity); // If wreck is too big

						qsf::TransformComponent& targetTransformComponent = mUseCover ? coverTransformComponent : wreckTransformComponent;
						glm::vec3 position = targetTransformComponent.getPosition();

						if (mUseCover)
						{
							// The position of the cover isn't the center of the cover mesh -> adjust the position to be in the center
							position += coverTransformComponent.getRotation() * glm::vec3(0.0f, 0.0f, 1.5f);
							position.y += 3.2f;	// TODO(tl): Bounding box seems to be bugged for cover
						}
						else
						{
							glm::vec3 minimum, maximum;
							qsf::EntityHelper(*wreckEntity).getAxisAlignedBoundingBox(minimum, maximum);
							position.y = maximum.y + 0.3f;
						}

						glm::vec3 offset;
						if (vehicleCraneComponent->checkTargetPosition(position, offset))
						{
							// Warp target entity
							targetTransformComponent.setPosition(targetTransformComponent.getPosition() + offset);
						}

						// Save the target position of the crane
						mTargetPosition = position;

						vehicleCraneComponent->setSequenceType(VehicleCraneComponent::SEQUENCE_PICKUP_BY_TOWCAR_ALIGN_ARM);
						vehicleCraneComponent->alignArm(position);

						mState = STATE_CRANE_ALIGN_ARM;
					}
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_CRANE_ALIGN_ARM:
				{
					if (!vehicleCraneComponent->isAnimationPlaying())
					{
						vehicleCraneComponent->setVisibleStrap(true);
						mSecondsPassed = 0.0f;

						AudioHelper::playTowCraneClonk(mAudioProxy, getEntity());
						mAudioProxy.synchronizeWithMultiplay();

						mState = STATE_CRANE_LINK_CARGO;
					}
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_CRANE_LINK_CARGO:
				{
					mSecondsPassed += clock.getTimePassed().getSeconds();
					if (mSecondsPassed > 1.5f)
					{
						linkWreckToCraneEntity(mUseCover ? *coverEntity : *wreckEntity);
						if (!mUseCover)
						{
							EntityHelper::setupEntityAsGhost(*wreckEntity, getEntity());
						}

						// Remove collision box
						qsf::ai::RouterComponent* routerComponent = wreckEntity->getComponent<qsf::ai::RouterComponent>();
						if (nullptr != routerComponent)
							routerComponent->removeCollisionBox();

						// With an active navigation component we are still using ai reservations from this entity, so we need to disable it
						qsf::ai::NavigationComponent* navigationComponent = wreckEntity->getComponent<qsf::ai::NavigationComponent>();
						if (nullptr != navigationComponent)
						{
							navigationComponent->setActive(false);
						}

						vehicleCraneComponent->setSequenceType(VehicleCraneComponent::SEQUENCE_PICKUP_BY_TOWCAR_LOAD_CARGO_ARM);
						vehicleCraneComponent->loadCargoArm();

						mState = STATE_CRANE_LOAD_CARGO;
					}
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_CRANE_LOAD_CARGO:
				{
					// Optical gimmick: In case there's a snow component, ensure it's completely disabled at this point in time (moving objects with snow look like a graphics glitch)
					// -> "STATE_CRANE_WAIT_BEFORE_DROP" state disables it completely, in "STATE_CRANE_LOAD_CARGO" we fade it out, looks nice
					qsf::game::SnowComponent* snowComponent = wreckEntity->getComponent<qsf::game::SnowComponent>();
					if (nullptr != snowComponent)
					{
						snowComponent->setSnowAmount(snowComponent->getSnowAmount() - 0.01f);
					}

					if (!vehicleCraneComponent->isAnimationPlaying())
					{
						mSecondsPassed = 0.0f;

						mState = STATE_CRANE_WAIT_BEFORE_DROP;
					}
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_CRANE_WAIT_BEFORE_DROP:
				{
					mSecondsPassed += clock.getTimePassed().getSeconds();
					if (mSecondsPassed > 1.0f)
					{
						unLinkWreckEntity(mUseCover ? *coverEntity : *wreckEntity);
						addToContainer(*wreckEntity);
						vehicleCraneComponent->stopAllAnimation();
						supportLegsComponent->hideExtending();

						{ // Optical gimmick: In case there's a snow component, ensure it's completely disabled at this point in time (moving objects with snow look like a graphics glitch)
							qsf::game::SnowComponent* snowComponent = wreckEntity->getComponent<qsf::game::SnowComponent>();
							if (nullptr != snowComponent)
							{
								snowComponent->setActive(false);
							}
						}

						AudioHelper::playTowCraneClonk(mAudioProxy, getEntity());
						mAudioProxy.synchronizeWithMultiplay();

						QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_CAR_PICKED_UP_BY_TOWCAR, wreckEntity->getId()));

						mState = STATE_SUPPORTLEGS_HIDE;
					}
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_SUPPORTLEGS_HIDE:
				{
					if (!supportLegsComponent->isAnimationPlaying())
					{
						vehicleCraneComponent->stopAllAnimation();
						mAudioProxy.stop();
						mAudioProxy.synchronizeWithMultiplay();

						mState = STATE_DONE;
					}
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_DONE:
				{
					// Send vehicle back to base. Use the player base as position
					ReturnToBaseCommand::pushReturnToBaseAction(getEntity(), action::COMMAND_STD);

					// TODO(co) No return by intent?
				}
			}
		}
		return qsf::action::RESULT_DONE;
	}

	void PickupVehicleByTowcarAction::onShutdown()
	{
		EventIdComponent::releaseTeamLock(mTargetEntityId, getEntity());

		// Make sure the cover entity is linked again
		//  -> This is especially relevant when vehicle gets destroyed, the cover should stay behind in this case
		//  -> In order for this to work, the action plan has to be cleared before destroying the object ("em5::MapHelper::destroyEntity" does it for exactly this reason)
		if (mUseCover)
		{
			qsf::Entity* coverEntity = getMap().getEntityById(mCoverEntityId);
			if (nullptr != coverEntity)
			{
				qsf::LinkComponent* linkComponent = coverEntity->getComponent<qsf::LinkComponent>();
				if (nullptr != linkComponent && linkComponent->isSimulating() && linkComponent->getParentId() != getEntityId())
				{
					// Link to vehicle again
					linkComponent->linkToParent(getEntity());
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	uint64 PickupVehicleByTowcarAction::getCoverEntityId()
	{
		RoadVehicleComponent* roadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
		if (nullptr == roadVehicleComponent)
			return qsf::getUninitialized<uint64>();

		return roadVehicleComponent->getSpecialEntity();
	}

	void PickupVehicleByTowcarAction::linkWreckToCraneEntity(qsf::Entity& wreckEntity)
	{
		VehicleCraneComponent* vehicleCraneComponent = getEntity().getComponent<VehicleCraneComponent>();

		if (nullptr != vehicleCraneComponent)
		{
			qsf::Entity* jibEntity = getFirstChildEntity(getMap().getEntityById(vehicleCraneComponent->getCraneEntityId()));
			qsf::Entity* stageEntity = getFirstChildEntity(jibEntity);
			qsf::Entity* strapEntity = getFirstChildEntity(stageEntity);

			if (nullptr != strapEntity)
			{
				qsf::TransformComponent& strapTransformComponent = strapEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
				qsf::TransformComponent& wreckTransformComponent = wreckEntity.getOrCreateComponentSafe<qsf::TransformComponent>();

				// Check if the wreck needs to be dropped rotated
				glm::vec3 position = mUseCover ? glm::vec3(0, -3.5f, -1.5f) : glm::vec3(0.0f, -2.5f, 0.15f);	// TODO(fw): ugly hardcoded stuff
				glm::quat rotation = getPlacementRotation(wreckTransformComponent, strapTransformComponent);

				// Activate shine through effect
				// TODO(sw) This doesn't work in multiplayer on client side, there are no actions...
				EntityHelper(wreckEntity).enableShineThroughEffect();

				// Create or reset link
				qsf::LinkComponent& linkComponent = wreckEntity.getOrCreateComponentSafe<qsf::LinkComponent>();
				linkComponent.setParentId(strapEntity->getId());
				linkComponent.setParentLinkType(qsf::LinkComponent::HARD_LINK);
				linkComponent.setLocalPosition(position);
				linkComponent.setLocalRotation(rotation);
			}
		}
	}

	void PickupVehicleByTowcarAction::unLinkWreckEntity(qsf::Entity& wreckEntity)
	{
		qsf::LinkComponent* linkComponent = wreckEntity.getComponent<qsf::LinkComponent>();
		if (nullptr == linkComponent)
			return;

		qsf::TransformComponent& wreckTransformComponent = wreckEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
		qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();

		// Check if the wreck needs to be dropped rotated
		glm::vec3 position = glm::vec3(0.044f, 0.963f, 1.330f); //TODO(tl): ugly hardcoded stuff
		glm::quat rotation = getPlacementRotation(wreckTransformComponent, transformComponent);

		if (mUseCover)
		{
			// Special handling for the cover
			position = glm::vec3(0.0f, 0.0f, 0.0f);
		}

		// Link to transporter
		linkComponent->linkToParent(getEntity());
		linkComponent->setLocalRotation(rotation);
		linkComponent->setLocalPosition(position);

		// TODO(tl): Dirty hack to update linkComponent
		// TODO(sw) This Hack doesn't work in multiplay on the client side....
		transformComponent.setPosition(transformComponent.getPosition() + glm::vec3(0.0f, -0.01f, 0.0f));

		qsf::ai::NavigationComponent* navigationComponent = wreckEntity.getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			navigationComponent->setActive(false);
		}

		// TODO(sw) This doesn't work in multiplayer on client side, there are no actions...
		wreckEntity.setHidden(false);	// Ensure the wreck isn't invisible
	}

	qsf::Entity* PickupVehicleByTowcarAction::getFirstChildEntity(qsf::Entity* entity)
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

	void PickupVehicleByTowcarAction::exchangeTargetWithCover(qsf::Entity& wreckEntity, qsf::Entity& coverEntity)
	{
		RoadVehicleComponent* wreckRoadVehicleComponent = wreckEntity.getComponent<RoadVehicleComponent>();
		if (nullptr != wreckRoadVehicleComponent)
		{
			mUseCover = wreckRoadVehicleComponent->getTooBigToTow();
		}

		HelicopterComponent* helicopterComponent = wreckEntity.getComponent<HelicopterComponent>();
		if (nullptr != helicopterComponent)
		{
			// Always use cover for helicopter
			mUseCover = true;
		}

		if (mUseCover)
		{
			qsf::TransformComponent& coverTransformComponent = coverEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
			qsf::LinkComponent& coverLinkComponent = coverEntity.getOrCreateComponentSafe<qsf::LinkComponent>();
			qsf::TransformComponent& wreckTransformComponent = wreckEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
			qsf::MeshComponent& wreckMeshComponent = wreckEntity.getOrCreateComponentSafe<qsf::MeshComponent>();

			// Fade out the target vehicle
			wreckEntity.getOrCreateComponentSafe<FadeEffectComponent>().fadeOut();
			wreckMeshComponent.setCastShadows(false);

			// Unlink cover
			coverLinkComponent.setParentId(qsf::getUninitialized<uint64>());

			// Move the cover to target, and let it fall to the ground
			glm::vec3 position = wreckTransformComponent.getPosition();
			qsf::Map& map = getMap();

			// TODO(sw) Uses deprecated ground map query constructor
			qsf::GroundMapQuery(getMap(), nullptr, GroundMaps::FILTER_HELICOPTER_GROUND).getHeightAtXZPosition(position.x, position.z, position.y);

			// Special handling for ME01 Truck
			qsf::game::EventTagComponent* eventTagComponent = wreckEntity.getComponent<qsf::game::EventTagComponent>();
			if (nullptr != eventTagComponent && eventTagComponent->hasTag(qsf::StringHash("MUNICH_ME01_TRUCK")))
			{
				coverTransformComponent.setPosition(position + coverTransformComponent.getRotation() * glm::vec3(1.5f, -1.0f, -1.5f));
			}
			else
			{
				coverTransformComponent.setPosition(position + coverTransformComponent.getRotation() * glm::vec3(0.0f, -1.0f, -1.5f));
			}

			// Link cover with the towcar
			qsf::LinkComponent& wreckLinkComponent = wreckEntity.getOrCreateComponentSafe<qsf::LinkComponent>();
			wreckLinkComponent.linkToParent(getEntity());

			// Delete all wrecker childs, like doors or environment parts (remember that we have pre edited vehicle accidents with obstacles)
			std::vector<uint64> childIds;
			for (const qsf::LinkComponent* childComponent : wreckLinkComponent.getChildLinks())
			{
				VehicleComponent* vehicleComponnent = childComponent->getEntity().getComponent<VehicleComponent>();
				if (nullptr != vehicleComponnent)
				{
					// Linked vehicles (eg motorboat with motorboat transporter) should enter the proxy container from the parent
					qsf::game::HiddenComponent::setHidden(childComponent->getEntity());
				}
				else
				{
					childIds.push_back(childComponent->getEntityId());
				}
			}

			for (uint64 id : childIds)
			{
				// Delete child entity with possible child entities
				map.destroyObjectById(id);
			}

			// Fade in the cover
			// TODO(sw) This doesn't work in multiplay on the client side...
			coverEntity.setActive(true);
			coverEntity.setHidden(true);
			coverEntity.getOrCreateComponentSafe<FadeEffectComponent>().fadeIn(qsf::Time::fromSeconds(1.5f));
		}
	}

	void PickupVehicleByTowcarAction::addToContainer(qsf::Entity& wreckEntity)
	{
		RoadVehicleComponent* roadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();
		if (nullptr != roadVehicleComponent)
		{
			// Collect all vehicle childs
			addChildsSafeToContainer(wreckEntity);

			// Collect the first one
			roadVehicleComponent->addToContainer(wreckEntity, container::CONTAINERTYPE_PASSENGER);
		}
	}

	void PickupVehicleByTowcarAction::addChildsSafeToContainer(qsf::Entity& mainChildEntity)
	{
		// This function is designed to work recursive, but currently its not needed
		RoadVehicleComponent* parentRoadVehicleComponent = getEntity().getComponent<RoadVehicleComponent>();

		qsf::EntityVectorScratchBuffer childVehicleEntities;

		// Call for all linked childs (to support eg the tow car, carrying another tow car with an motorboat transporter...)
		VehicleComponent* vehicleComponent = mainChildEntity.getComponent<VehicleComponent>();
		if (nullptr != vehicleComponent)
		{
			for (auto& iterator : vehicleComponent->getLinks())
			{
				const qsf::ContainerLink& containerLink = *iterator.second;
				qsf::Entity& linkedEntity = containerLink.getTargetEntity();

				// Here take the vehicle component to support boats/helicopters
				VehicleComponent* childVehicleComponent = linkedEntity.getComponent<VehicleComponent>();
				if (nullptr != childVehicleComponent)
				{
					childVehicleEntities.push_back(&linkedEntity);
				}
			}
		}

		// Destroy all old connections
		for (qsf::Entity* vehicleEntity : childVehicleEntities)
		{
			vehicleComponent = vehicleEntity->getComponent<VehicleComponent>();
			if (nullptr != vehicleComponent)
			{
				vehicleComponent->removeFromContainer(parentRoadVehicleComponent->getPrototype());
			}
			qsf::LinkComponent* linkComponent = vehicleEntity->getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent)
			{
				linkComponent->unlinkFromParent();
			}
		}

		// Enter the tow car how holds the action, rebuild connections
		for (qsf::Entity* vehicleEntity : childVehicleEntities)
		{
			parentRoadVehicleComponent->addToContainer(*vehicleEntity, container::CONTAINERTYPE_PASSENGER);
			vehicleEntity->getOrCreateComponentSafe<qsf::LinkComponent>().linkToParent(getEntity());
		}
	}

	float PickupVehicleByTowcarAction::towcarToWreckYaw(const qsf::Transform& towcarTransform, const qsf::Transform& wreckTransform)
	{
		const glm::vec3 direction = glm::normalize(wreckTransform.getPosition() - towcarTransform.getPosition());
		const glm::vec3 localDirection = wreckTransform.vec3DirectionWorldToLocal(direction);

		return qsf::EulerAngles::quaternionToEuler(qsf::Math::getLookAtQuaternion(localDirection, qsf::CoordinateSystem::getUp())).x;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

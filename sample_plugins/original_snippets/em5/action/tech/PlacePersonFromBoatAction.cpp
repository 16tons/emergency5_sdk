// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/tech/PlacePersonFromBoatAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/command/tech/PlacePersonFromBoatCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/component/objects/BoatDropPointComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/game/collision/CollisionTypes.h"
#include "em5/health/injury/Injuries.h"
#include "em5/health/HealthComponent.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/health/HealthHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/component/base/HiddenComponent.h>
#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>
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
	const qsf::NamedIdentifier PlacePersonFromBoatAction::ACTION_ID = "em5::PlacePersonFromBoatAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlacePersonFromBoatAction::PlacePersonFromBoatAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	PlacePersonFromBoatAction::~PlacePersonFromBoatAction()
	{
		// Nothing here
	}

	void PlacePersonFromBoatAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}

	uint64 PlacePersonFromBoatAction::getTargetId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PlacePersonFromBoatAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint8>(mState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PlacePersonFromBoatAction::onStartup()
	{
		// TODO(mk) why is here nearly the same code as in the checkActionCondition() ?
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" && "checkContext" of the command for validation
		PlacePersonFromBoatCommand* placePersonFromBoatCommand = static_cast<PlacePersonFromBoatCommand*>(QSFGAME_COMMAND.getCommandManager().getById<PlacePersonFromBoatCommand>(PlacePersonFromBoatCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != placePersonFromBoatCommand, "PlacePersonFromBoatAction::onStartup(): Could not lay down person from boat command instance", return false);
		if (!(placePersonFromBoatCommand->checkCallerWithoutPriority(*commandContext.mCaller) && placePersonFromBoatCommand->checkContext(commandContext)))
			return false;

		return true;
	}

	qsf::action::Result PlacePersonFromBoatAction::updateAction(const qsf::Clock&)
	{
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return qsf::action::RESULT_DONE;	// Target entity is not existing, error

		switch (mState)
		{
			case STATE_INIT:
			{
				if (!checkActionCondition(*targetEntity))
				{
					return qsf::action::RESULT_DONE;
				}

				mState = STATE_PLAY_ANIMATION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_PLAY_ANIMATION:
			{
				// Link person to boat.
				unLinkTargetEntity(*targetEntity);

				// Send message to update gui
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION));

				mState = STATE_FINISH;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FINISH:
				// Nothing to do in this case
				break;
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PlacePersonFromBoatAction::unLinkTargetEntity(qsf::Entity& targetEntity)
	{
		// Is the target an unload spot?
		BoatDropPointComponent* boatDropPointComponent = targetEntity.getComponent<BoatDropPointComponent>();
		if (nullptr != boatDropPointComponent)
		{
			// Is this entity a boat?
			BoatComponent* boatComponent = getEntity().getComponent<BoatComponent>();
			if (nullptr != boatComponent)
			{
				BoatDropPointComponent::EntityIdArrayInterface dropMarker = boatDropPointComponent->DropMarkerArray;

				qsf::EntityVectorScratchBuffer entitiesToExit;
				for (auto& iterator : boatComponent->getLinks())
				{
					// Get the person in the vehicle
					qsf::ContainerLink& containerLink = *iterator.second;
					entitiesToExit.push_back(&containerLink.getTargetEntity());
				}

				// Release all persons
				for (auto& iterator : entitiesToExit)
				{
					// Get the person in the vehicle
					qsf::Entity& personEntity = *iterator;

					// Exit the vehicle now
					EntityHelper(personEntity).leaveContainer();

					// Squad persons usually are not drowning, so this must be a diver
					const bool isDiver = EntityHelper(personEntity).isSquadPerson();
					if (!isDiver)
					{
						applyInjuryToTransportedPerson(personEntity);
					}

					// Stop old animations
					AnimationHelper(personEntity).clearAnimation();

					// Fade in person
					personEntity.getOrCreateComponentSafe<FadeEffectComponent>().fadeIn();

					// Set position to drop point
					qsf::TransformComponent& personTransformComponent = personEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
					qsf::TransformComponent& targetTransformComponent = targetEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
					personTransformComponent.setRotation(qsf::Math::GLM_QUAT_IDENTITY);	// Overwrite the person rotation to assume that everybody is in the right place

					glm::vec3 position = targetTransformComponent.getPosition();
					int iterateNumber = 0;

					do
					{
						size_t dropMarkerIndex = boatDropPointComponent->getDropMarkerIndex();
						if (dropMarker.size() > 0)
						{
							// Only access dropMarker list when the list contains elements
							qsf::Entity* dropMarkerEntity = getMap().getEntityById(dropMarker.get(dropMarkerIndex));
							if (nullptr != dropMarkerEntity)
							{
								qsf::TransformComponent& dropMarkerTransformComponent = dropMarkerEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
								position = dropMarkerTransformComponent.getPosition();
							}
						}

						personTransformComponent.setPosition(position);

						// Place the person on the ground
						EntityHelper(personEntity).applyPlacement();

						// Use next drop marker index for the next victim unloaded
						++dropMarkerIndex;
						boatDropPointComponent->setDropMarkerIndex(dropMarkerIndex < dropMarker.size() ? dropMarkerIndex : 0);
						++iterateNumber;

					// Check collision for up to 20 drop marker (actual number is lower but shouldn't be a problem)
					} while (!CollisionHelper(getMap()).canBeAddedWithoutCollision(personEntity) && iterateNumber < 20);


					// Make person mortal
					HealthHelper(personEntity).setInvincible(false);

					if (!isDiver)
					{
						// Report that the action has been done
						QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_PLACE_PERSON_FROM_BOAT_ACTION, mTargetEntityId, getEntityId(), personEntity.getId()));
					}
				}
			}
		}
	}

	bool PlacePersonFromBoatAction::checkActionCondition(qsf::Entity& targetEntity)
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = &targetEntity;

		// Call "checkCaller" && "checkContext" of the commando for validation
		PlacePersonFromBoatCommand* placePersonFromBoatCommand = static_cast<PlacePersonFromBoatCommand*>(QSFGAME_COMMAND.getCommandManager().getById<PlacePersonFromBoatCommand>(PlacePersonFromBoatCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != placePersonFromBoatCommand, "PlacePersonFromBoatAction::checkActionCondition(): Could not lay down person from boat command instance", return false);
		if (!(placePersonFromBoatCommand->checkCallerWithoutPriority(*commandContext.mCaller) && placePersonFromBoatCommand->checkContext(commandContext)))
			return false;

		// Make the distance check
		float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getEnterVehicleDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(targetEntity, callerEntity);

		if (currentDistance > (maxDistance*2))
			return false;

		// Every check is valid, return true
		return true;
	}

	void PlacePersonFromBoatAction::applyInjuryToTransportedPerson(qsf::Entity& targetEntity)
	{
		HealthComponent* healthComponent = targetEntity.getComponent<HealthComponent>();
		if (nullptr != healthComponent)
		{
			if (healthComponent->getIsDrowning())
			{
				// Higher than 10% life
				uint32 injuryNameHash;
				if (healthComponent->getLifeEnergy() > healthComponent->getMaximumLifeEnergy() * 0.1f)
				{
					int index = qsf::Random::getRandomInt(0, 1);
					if (index == 0)
					{
						injuryNameHash = injury::WEAKNESS;
					}
					else
					{
						injuryNameHash = injury::HYPOTHERMIA;
					}
				}
				else
				{
					injuryNameHash = injury::WATER_FILLED_LUNG;
				}

				// Get the current freeplay event from the target
				FreeplayEvent* currentEvent = nullptr;
				EventIdComponent* eventIdComponent = targetEntity.getComponent<EventIdComponent>();
				if (nullptr != eventIdComponent)
				{
					currentEvent = eventIdComponent->getFreeplayEvent();
				}
				healthComponent->applyInjuryById(injuryNameHash, currentEvent, false);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

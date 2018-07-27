// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/RescuePersonFromBuildingAction.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/overlay/BuildingOverlayComponent.h"
#include "em5/command/fire/RescuePersonFromBuildingCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/logic/local/base/PersonInBuildingLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/map/Map.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Math.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/base/TransformComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier RescuePersonFromBuildingAction::ACTION_ID = "em5::RescuePersonFromBuildingAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RescuePersonFromBuildingAction::RescuePersonFromBuildingAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT),
		mNeedsToHideOverlay(false)
	{
		// Nothing here
	}

	RescuePersonFromBuildingAction::~RescuePersonFromBuildingAction()
	{
		if (mNeedsToHideOverlay)
		{
			qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
			if (targetEntity != nullptr)
			{
				BuildingOverlayComponent::hideOverlay(*targetEntity, getEntity());
			}
		}
	}

	void RescuePersonFromBuildingAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void RescuePersonFromBuildingAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool RescuePersonFromBuildingAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		glm::vec3 targetPosition = targetEntity->getComponentSafe<qsf::TransformComponent>().getPosition();
		const BuildingComponent& buildingComponent = targetEntity->getComponentSafe<BuildingComponent>();
		if (buildingComponent.hasDoor())
			targetPosition = buildingComponent.getDoorPosition();

		// Make the distance check
		const glm::vec3& callerPosition = callerEntity.getComponentSafe<qsf::TransformComponent>().getPosition();
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getEnterBuildingDistance();
		const float currentSquaredDistance = glm::distance2(targetPosition, callerPosition);
		if (currentSquaredDistance > maxDistance * maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" and "checkContext" of the commando for validation
		RescuePersonFromBuildingCommand* rescuePersonFromBuildingCommand = static_cast<RescuePersonFromBuildingCommand*>(QSFGAME_COMMAND.getCommandManager().getById<RescuePersonFromBuildingCommand>(RescuePersonFromBuildingCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != rescuePersonFromBuildingCommand, "RescuePersonFromBuildingAction::onStartup(): Could not find rescue person from building command instance", return false);
		if (!(rescuePersonFromBuildingCommand->checkCallerWithoutPriority(callerEntity) && rescuePersonFromBuildingCommand->checkContext(commandContext)))
			return false;

		return true;
	}

	qsf::action::Result RescuePersonFromBuildingAction::updateAction(const qsf::Clock& jobArguments)
	{
		// Get the most important objects first
		qsf::Entity* buildingEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == buildingEntity)
			return qsf::action::RESULT_DONE;

		BuildingComponent* buildingComponent = buildingEntity->getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return qsf::action::RESULT_DONE;

		// Person in building logic is currently optional
		PersonInBuildingLogic* personInBuildingLogic = EntityHelper(*buildingEntity).getGameLogic<PersonInBuildingLogic>();

		// Handle the current state
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Check if building is already "full", i.e. enough units inside
				if (nullptr != personInBuildingLogic && personInBuildingLogic->isTeamComplete())
				{
					return qsf::action::RESULT_DONE;
				}

				EntityHelper entityHelper(getEntity());
				mOldTransform = entityHelper.getTransform();
				entityHelper.enterContainer(*buildingEntity, container::ContainerTypes::CONTAINERTYPE_PROGRESS);

				mCurrentTime = qsf::Time::ZERO;
				mTotalTime = buildingComponent->getFirefighterRescueTime();

				// Make building invincible for the progress
				EntityHelper(*buildingEntity).setInvincible(true);

				if (nullptr != personInBuildingLogic)
				{
					personInBuildingLogic->playEvacuateAudio();
				}

				mCurrentState = STATE_WAIT_FOR_COMPLETE_TEAM;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_WAIT_FOR_COMPLETE_TEAM:
			{
				if (nullptr != personInBuildingLogic)
				{
					BuildingOverlayComponent::showProgress(*buildingEntity, personInBuildingLogic->getNumberValidTeamUnitsNeeded(), getEntity(), 0);
					mNeedsToHideOverlay = true;
					if (!personInBuildingLogic->isTeamComplete())
					{
						// Wait till the team is complete
						return qsf::action::RESULT_CONTINUE;
					}
				}

				mCurrentState = STATE_ENTER;
				// Immediately go to the next state; no break by intent
			}

			case STATE_ENTER:
			{
				if (mCurrentTime < mTotalTime)
				{
					mCurrentTime += jobArguments.getTimePassed();

					int numberNeededPersons = 1;
					if (nullptr != personInBuildingLogic)
					{
						numberNeededPersons = personInBuildingLogic->getNumberValidTeamUnitsNeeded();
					}

					// Show building overlay
					BuildingOverlayComponent::showProgress(*buildingEntity, numberNeededPersons, getEntity(), mCurrentTime / mTotalTime);
					mNeedsToHideOverlay = true;
					return qsf::action::RESULT_CONTINUE;
				}

				if (nullptr != personInBuildingLogic)
				{
					personInBuildingLogic->startFreeingCivil();
				}

				mCurrentState = STATE_RESCUE_PERSONS;
				// Immediately go to the next state; no break by intent
			}

			case STATE_RESCUE_PERSONS:
			{
				if (nullptr != personInBuildingLogic)
				{
					if (!personInBuildingLogic->isBuildingEmpty())
					{
						// Wait till all persons are rescued
						return qsf::action::RESULT_CONTINUE;
					}
				}

				// Hide building overlay
				BuildingOverlayComponent::hideOverlay(*buildingEntity, getEntity());
				mNeedsToHideOverlay = false;

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_DONE;
			}

			case STATE_DONE:
			{
				// Done
				EntityHelper(getEntity()).leaveContainer();

				// Test the place where we want place the unit (position is out parameter)
				glm::vec3 position = mOldTransform.getPosition();
				CollisionHelper(getMap()).findFreePositionEM3(getEntity(), mOldTransform, 2.0f, 0.1f, position);
				qsf::MovableComponent* personMovableComponent = getEntity().getOrCreateComponent<qsf::MovableComponent>();
				if (nullptr != personMovableComponent)
				{
					// Turn the firefighter for 180°
					glm::quat turnedRotation = mOldTransform.getRotation() * qsf::Math::getRotationAroundAxis(qsf::CoordinateSystem::getUp(), glm::pi<float>());
					personMovableComponent->warpToPositionAndRotation(position, turnedRotation);
				}

				buildingComponent->untrapPerson_SingleInjured();

				// Make building normal
				EntityHelper(*buildingEntity).setInvincible(false);

				// Finish the action
				return qsf::action::RESULT_DONE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

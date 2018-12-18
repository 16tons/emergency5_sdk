// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/EvacuateBuildingAction.h"
#include "em5/base/ContainerCategory.h"
#include "em5/action/base/ExitBuildingAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MovementModes.h"
#include "em5/audio/AudioHelper.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/police/EvacuateBuildingCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/overlay/BuildingOverlayComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/map/Map.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
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
	const qsf::NamedIdentifier EvacuateBuildingAction::ACTION_ID = "em5::EvacuateBuildingAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	EvacuateBuildingAction::EvacuateBuildingAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT)
	{
		// Nothing here
	}

	EvacuateBuildingAction::~EvacuateBuildingAction()
	{
		if (mTargetEntity.valid())
		{
			BuildingOverlayComponent::hideOverlay(mTargetEntity.getSafe(), getEntity());
		}
	}

	void EvacuateBuildingAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntity = targetEntity;
	}

	uint64 EvacuateBuildingAction::getTargetEntityId() const
	{
		return (mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void EvacuateBuildingAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool EvacuateBuildingAction::onStartup()
	{
		if (!validateUserAndTarget())
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &getEntity();
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		EvacuateBuildingCommand* evacuateBuildingCommand = static_cast<EvacuateBuildingCommand*>(QSFGAME_COMMAND.getCommandManager().getById<EvacuateBuildingCommand>(EvacuateBuildingCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != evacuateBuildingCommand, "EvacuateBuildingAction::onStartup(): Could not find evacuate building command instance", return false);
		if (!(evacuateBuildingCommand->checkCallerWithoutPriority(getEntity()) && evacuateBuildingCommand->checkContext(commandContext)))
			return false;

		return true;
	}

	qsf::action::Result EvacuateBuildingAction::updateAction(const qsf::Clock& clock)
	{
		if (validateUserAndTarget())
		{
			EntityHelper entityHelper(getEntity());

			switch (mCurrentState)
			{
				case STATE_INIT:
				{
					entityHelper.enterContainer(mTargetEntity.getSafe(), container::CONTAINERTYPE_PROGRESS);

					// Currently the length (10sec) fits with the length of the audio file.
					mProgressTime = qsf::Time::fromSeconds(10.0f);
					AudioHelper::playBuildingEvacuateSound(mAudioProxy, getEntity());
					mCurrentState = STATE_EVACUATING;
					// Fall through by design
				}

				case STATE_EVACUATING:
				{
					// Build timer for the progress
					if (mProgressTime > qsf::Time::ZERO)
					{
						mProgressTime -= clock.getTimePassed();
						BuildingOverlayComponent::showProgress(mTargetEntity.getSafe(), 1, getEntity(), 1.0f - (mProgressTime.getSeconds() / 10.0f));

						// Wait till time is over
						return qsf::action::RESULT_CONTINUE;
					}

					BuildingOverlayComponent::hideOverlay(mTargetEntity.getSafe(), getEntity());

					evacuatePersons();
					entityHelper.leaveContainer();

					mCurrentState = STATE_DONE;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_DONE:
				{
					entityHelper.placeEntityAtBuildingDoor(mTargetEntity.getSafe(), false);
					entityHelper.fadeIn();

					// Update minimap icons
					BuildingComponent* buildingComponent = mTargetEntity->getComponent<BuildingComponent>();
					if (buildingComponent)
					{
						buildingComponent->updateIcons();
					}

					break;
				}
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool EvacuateBuildingAction::validateUserAndTarget()
	{
		if (!mTargetEntity.valid())
			return false;

		BuildingComponent* buildingComponent = mTargetEntity->getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return false;

		return true;
	}

	void EvacuateBuildingAction::evacuatePersons()
	{
		// Get the building
		BuildingComponent* buildingComponent = mTargetEntity->getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return;

		// Get the persons
		std::vector<qsf::Entity*> entitiesInBuilding = buildingComponent->getAllLinksByContainerType(container::CONTAINERTYPE_TRAPPED);
		std::vector<qsf::Entity*> hostageInBuilding = buildingComponent->getAllLinksByContainerType(container::CONTAINERTYPE_HOSTAGE);
		entitiesInBuilding.insert(entitiesInBuilding.end(), hostageInBuilding.begin(), hostageInBuilding.end());

		qsf::Time fleeWaitTime(qsf::Time::ZERO);

		bool injuredPersonsInBuilding = false;
		// Exit the entities
		for (qsf::Entity* containedEntity : entitiesInBuilding)
		{
			EntityHelper containedEntityHelper(*containedEntity);

			// Check if the entity is a person
			if (containedEntityHelper.isEntityPerson())
			{
				if (containedEntityHelper.isPersonInjured()) // Do nothing if the person is already injured!
				{
					injuredPersonsInBuilding = true;
				}
				else
				{
					fleeWaitTime += qsf::Time::fromSeconds(1.0f);

					// This one can flee so, let it run for its life
					qsf::ActionComponent& actionComponent = containedEntity->getOrCreateComponentSafe<qsf::ActionComponent>();
					actionComponent.clearPlan();
					actionComponent.pushAction<qsf::WaitAction>(action::BLOCKING).init(fleeWaitTime);
					actionComponent.pushAction<ExitBuildingAction>(action::DANGER_URGENT).init(true);

					// Send message
					if (containedEntityHelper.isInContainer(*mTargetEntity, container::CONTAINERTYPE_HOSTAGE))
					{
						// For hostages
						qsf::MessageParameters messageParameters;
						messageParameters.setParameter("buildingId", mTargetEntity->getId());
						QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_FREE_HOSTAGE_FROM_BUILDING, containedEntity->getId()), messageParameters);
					}
					else
					{
						// For trapped persons
						qsf::MessageParameters messageParameters;
						messageParameters.setParameter("buildingId", mTargetEntity->getId());
						QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_FREE_PERSON_FROM_BUILDING, containedEntity->getId()), messageParameters);
					}
				}
			}
		}

		sendUnitHint(injuredPersonsInBuilding);
	}

	void EvacuateBuildingAction::sendUnitHint(bool injuredPersonsInBuilding)
	{
		CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			std::string hintId = (injuredPersonsInBuilding ? "EM5X_BIGFIRE_UNITHINT01" : "EM5X_BIGFIRE_UNITHINT02");
			uint32 playerIndex = commandableComponent->getPlayerId();
			HintHelper::showHint(hintId, playerIndex, qsf::Time::fromSeconds(30.0f));
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/AttackBuildingAction.h"
#include "em5/action/base/GuidePersonAction.h"
#include "em5/action/police/ArrestGangsterAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/base/ContainerCategory.h"
#include "em5/command/police/AttackBuildingCommand.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/overlay/BuildingOverlayComponent.h"
#include "em5/logic/local/police/GangsterInBuildingLogic.h"
#include "em5/logic/HintHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier AttackBuildingAction::ACTION_ID = "em5::AttackBuildingAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	AttackBuildingAction::AttackBuildingAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT),
		mWaitForFullGroupTime(qsf::Time::ZERO)
	{
		// Nothing here
	}

	AttackBuildingAction::~AttackBuildingAction()
	{
		if (mTargetEntity.valid() && mCurrentState != STATE_INIT)
		{
			BuildingOverlayComponent::hideOverlay(mTargetEntity.getSafe(), getEntity());
		}
	}

	void AttackBuildingAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntity = targetEntity;
	}

	uint64 AttackBuildingAction::getTargetEntityId() const
	{
		return (mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>());
	}

	bool AttackBuildingAction::getIsHalfProcessDone()
	{
		return (mCurrentState == STATE_DO_PROGRESS && mProgressTime <= qsf::Time::fromSeconds(5.0f));
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void AttackBuildingAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool AttackBuildingAction::onStartup()
	{
		if (!validateUserAndTarget())
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &getEntity();
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		AttackBuildingCommand* attackBuildingCommand = static_cast<AttackBuildingCommand*>(QSFGAME_COMMAND.getCommandManager().getById<AttackBuildingCommand>(AttackBuildingCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != attackBuildingCommand, "AttackBuildingAction::onStartup(): Could not find arrest gangster command instance", return false);
		if (!attackBuildingCommand->checkContext(commandContext))
			return false;

		return true;
	}

	qsf::action::Result AttackBuildingAction::updateAction(const qsf::Clock& clock)
	{
		if (validateUserAndTarget())
		{
			switch (mCurrentState)
			{
				case STATE_INIT:
				{
					EntityHelper entityHelper(getEntity());
					entityHelper.storeActiveEquipmentInInventory();
					entityHelper.enterContainer(mTargetEntity.getSafe(), container::CONTAINERTYPE_PROGRESS);
					mWaitForFullGroupTime = qsf::Time::fromSeconds(15.0f);

					mCurrentState = STATE_WAIT_FOR_FULL_GROUP;
					// Fall through by design
				}

				case STATE_WAIT_FOR_FULL_GROUP:
				{
					GangsterInBuildingLogic* gangsterInBuildingLogic = EntityHelper(*mTargetEntity).getGameLogic<GangsterInBuildingLogic>();
					if (nullptr != gangsterInBuildingLogic)
					{
						if (!gangsterInBuildingLogic->isPoliceTeamComplete())
						{
							// Team is not complete, wait
							BuildingOverlayComponent::showProgress(mTargetEntity.getSafe(), 3, getEntity(), 0);

							if (mWaitForFullGroupTime > qsf::Time::ZERO)
							{
								mWaitForFullGroupTime -= clock.getTimePassed();
								if (mWaitForFullGroupTime <= qsf::Time::ZERO)
								{
									CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
									if (nullptr != commandableComponent && !gangsterInBuildingLogic->getAllowNormalPolice())
									{
										bool hasHostages = false;
										{
											BuildingComponent* buildingComponent = mTargetEntity->getComponent<BuildingComponent>();
											if (nullptr != buildingComponent)
											{
												hasHostages = !buildingComponent->getAllLinksByContainerType(container::CONTAINERTYPE_HOSTAGE).empty();
											}
										}

										const char* hintId = hasHostages ? "EM5X_HOSTAGE_UNITHINT05" : "EM5X_HOSTAGE_UNITHINT06";
										HintHelper::showHint(hintId, commandableComponent->getPlayerId(), qsf::Time::fromSeconds(30.0f));
									}
								}
							}

							return qsf::action::RESULT_CONTINUE;
						}

						// Play sound, depending on the kind of units entering the building
						if (gangsterInBuildingLogic->getAllowNormalPolice())
						{
							AudioHelper::playBuildingAttackingSound1(mAudioProxy, getEntity());
							mAudioProxy.synchronizeWithMultiplay();
						}
						else
						{
							// The sound for the sek is more violent
							AudioHelper::playBuildingAttackingSound2(mAudioProxy, getEntity());
							mAudioProxy.synchronizeWithMultiplay();
						}
					}

					mProgressTime = qsf::Time::fromSeconds(10.0f);
					mCurrentState = STATE_DO_PROGRESS;
					// Fall through by design
				}

				case STATE_DO_PROGRESS:
				{
					if (mProgressTime > qsf::Time::ZERO)
					{
						mProgressTime -= clock.getTimePassed();
						BuildingOverlayComponent::showProgress(mTargetEntity.getSafe(), 3, getEntity(), 1.0f - (mProgressTime.getSeconds() / 10.0f));

						// Wait till time is over
						return qsf::action::RESULT_CONTINUE;
					}

					BuildingOverlayComponent::hideOverlay(mTargetEntity.getSafe(), getEntity());
					mCurrentState = STATE_FINISH_FIGHT;
					// Fall through by design
				}

				case STATE_FINISH_FIGHT:
				{
					EntityHelper entityHelper(getEntity());
					entityHelper.leaveContainer();

					// We need time to create the collision before setting the entity to the final place
					mCurrentState = STATE_DONE;
					return qsf::action::RESULT_CONTINUE;
				}

				case STATE_DONE:
				{
					EntityHelper entityHelper(getEntity());
					entityHelper.placeEntityAtBuildingDoor(mTargetEntity.getSafe(), false);
					entityHelper.fadeIn();
					arrestTarget();

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
	bool AttackBuildingAction::validateUserAndTarget()
	{
		if (!mTargetEntity.valid())
			return false;

		return true;
	}

	void AttackBuildingAction::arrestTarget()
	{
		BuildingComponent* buildingComponent = mTargetEntity->getComponent<BuildingComponent>();
		if (nullptr != buildingComponent)
		{
			// Building contains gangsters, civil and police units
			for (auto iterator : buildingComponent->getLinks())
			{
				// Find the target
				qsf::Entity& containedEntity = iterator.second->getTargetEntity();
				PersonComponent* targetPersonComponent = containedEntity.getComponent<PersonComponent>();
				if (nullptr != targetPersonComponent && targetPersonComponent->getGangsterFlag())
				{
					// Destroy the linkage between building and gangster
					EntityHelper entityHelper(containedEntity);
					entityHelper.leaveContainer();

					// Set caller to door position
					entityHelper.setPosition(buildingComponent->getDoorPosition());

					ArrestGangsterAction::setArrestedParameters(containedEntity, true);

					getComponent().pushAction<GuidePersonAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(containedEntity.getId(), GuidePersonAction::ARREST, true);

					// Send message
					qsf::MessageParameters parameters;
					parameters.setParameter("PolicemanId", getEntityId());
					parameters.setParameter("GangsterId", containedEntity.getId());
					parameters.setParameter("BuildingId", getTargetEntityId());
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_ATTACK_BUILDING_FINISH, getTargetEntityId()), parameters);

					// Handling one gangster enough
					break;
				}
			}

			// TODO(mk) for fake objects we can add a injured civil person

			qsf::GameLogicComponent* gameLogicComponent = mTargetEntity->getComponent<qsf::GameLogicComponent>();
			if (nullptr != gameLogicComponent)
			{
				GangsterInBuildingLogic* gangsterInBuildingLogic = gameLogicComponent->getGameLogic<GangsterInBuildingLogic>();
				if (nullptr != gangsterInBuildingLogic)
				{
					if (gangsterInBuildingLogic->getFakeObject())
					{
						qsf::MessageParameters parameters;
						parameters.setParameter("PolicemanId", getEntityId());
						parameters.setParameter("BuildingId", getTargetEntityId());
						QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_ATTACK_FAKEBUILDING_FINISH, getTargetEntityId()), parameters);
					}

					// Function only updates the objectives if necessary
					gangsterInBuildingLogic->onPoliceAttackedBuilding();
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

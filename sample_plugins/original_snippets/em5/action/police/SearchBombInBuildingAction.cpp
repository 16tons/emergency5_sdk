// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/SearchBombInBuildingAction.h"
#include "em5/action/base/GuidePersonAction.h"
#include "em5/action/police/ArrestGangsterAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/ContainerCategory.h"
#include "em5/command/police/SearchBombInBuildingCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/objects/PoliceDogTargetComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/overlay/BuildingOverlayComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/link/link/prototype/ContainerLink.h>
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
	const qsf::NamedIdentifier SearchBombInBuildingAction::ACTION_ID = "em5::SearchBombInBuildingAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SearchBombInBuildingAction::SearchBombInBuildingAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT),
		mProgressTime(qsf::Time::ZERO)
	{
		// Nothing here
	}

	SearchBombInBuildingAction::~SearchBombInBuildingAction()
	{
		if (mTargetEntity.valid() && mCurrentState != STATE_INIT)
		{
			BuildingOverlayComponent::hideOverlay(mTargetEntity.getSafe(), getEntity());
		}
	}

	void SearchBombInBuildingAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntity = targetEntity;
	}

	uint64 SearchBombInBuildingAction::getTargetEntityId() const
	{
		return (mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void SearchBombInBuildingAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool SearchBombInBuildingAction::onStartup()
	{
		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &getEntity();
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		SearchBombInBuildingCommand* searchBombInBuildingCommand = static_cast<SearchBombInBuildingCommand*>(QSFGAME_COMMAND.getCommandManager().getById<SearchBombInBuildingCommand>(SearchBombInBuildingCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != searchBombInBuildingCommand, "SearchBombInBuildingAction::onStartup(): Could not find search bomb in building command instance", return false);
		if (!searchBombInBuildingCommand->checkCallerWithoutPriority(getEntity()) || !searchBombInBuildingCommand->checkContext(commandContext))
			return false;

		return true;
	}

	qsf::action::Result SearchBombInBuildingAction::updateAction(const qsf::Clock& clock)
	{
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				EntityHelper entityHelper(getEntity());
				entityHelper.enterContainer(mTargetEntity.getSafe(), container::CONTAINERTYPE_PROGRESS);

				mProgressTime = qsf::Time::fromSeconds(15.0f);

				mTargetEntity->destroyComponent<PoliceDogTargetComponent>();
				mCurrentState = STATE_DO_PROGRESS;
				// Fall through by design
			}

			case STATE_DO_PROGRESS:
			{
				if (mProgressTime > qsf::Time::ZERO)
				{
					mProgressTime -= clock.getTimePassed();
					BuildingOverlayComponent::showProgress(mTargetEntity.getSafe(), 1, getEntity(), 1.0f - (mProgressTime.getSeconds() / 15.0f));

					// Wait till time is over
					return qsf::action::RESULT_CONTINUE;
				}

				BuildingOverlayComponent::hideOverlay(mTargetEntity.getSafe(), getEntity());

				mCurrentState = STATE_FINISH_SEARCH;
				// Fall through by design
			}

			case STATE_FINISH_SEARCH:
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

				// Send message
				qsf::MessageParameters parameters;
				parameters.setParameter("PolicemanId", getEntityId());
				parameters.setParameter("BuildingId", getTargetEntityId());
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_SEARCH_BOMB_IN_BUILDING_FINISH, getTargetEntityId()), parameters);

				// Update minimap icons
				BuildingComponent* buildingComponent = mTargetEntity->getComponent<BuildingComponent>();
				if (buildingComponent)
				{
					buildingComponent->updateIcons();
				}

				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/SpeakToHostageTakerAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/ContainerCategory.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/police/SpeakToHostageTakerCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/logic/local/police/GangsterInBuildingLogic.h"
#include "em5/logic/HintHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/Game.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{

	namespace detail
	{
		// TODO(mk) Read the funny variable name from specs file
		static const qsf::Time TIME_TO_SPEAK = qsf::Time::fromSeconds(10.0f);
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier SpeakToHostageTakerAction::ACTION_ID = "em5::SpeakToHostageTakerAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SpeakToHostageTakerAction::SpeakToHostageTakerAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT)
	{
		// Nothing here
	}

	SpeakToHostageTakerAction::~SpeakToHostageTakerAction()
	{
		if (mCurrentState == STATE_USE_MEGAPHONE)
		{
			// Hide status overlay bar
			StatusOverlayComponent::hideBarOverlay(*mTargetEntity, StatusOverlayComponent::BAR_TYPE_PROGRESS);
		}
	}

	void SpeakToHostageTakerAction::init(qsf::Entity& buildingEntity)
	{
		mTargetEntity = buildingEntity;
	}

	uint64 SpeakToHostageTakerAction::getTargetEntityId() const
	{
		return (mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void SpeakToHostageTakerAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) whats up with this useless functions
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool SpeakToHostageTakerAction::onStartup()
	{
		qsf::Entity& callerEntity = getEntity();
		if (!mTargetEntity.valid())
			return false;

		// Get the building component
		BuildingComponent* buildingComponent = mTargetEntity->getComponent<BuildingComponent>();
		if (buildingComponent == nullptr)
			return false;

		// Get main door entity
		const qsf::Entity* targetDoorEntity = getMap().getEntityById(buildingComponent->getMainDoorId());
		if (nullptr == targetDoorEntity)
			return false;

		// Make the distance check
		const float maxDistance = 10.f; // Here be nice to the speaker, the real distance is not so important
		const float currentDistance = qsf::game::DistanceHelper::getDistance(buildingComponent->getDoorPosition(), callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		SpeakToHostageTakerCommand* speakToHostageTakerCommand = static_cast<SpeakToHostageTakerCommand*>(QSFGAME_COMMAND.getCommandManager().getById<SpeakToHostageTakerCommand>(SpeakToHostageTakerCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != speakToHostageTakerCommand, "SpeakToHostageTakerAction::onStartup(): Could not find speak to hostage command instance", return false);
		if (!(speakToHostageTakerCommand->checkCaller(*commandContext.mCaller) && speakToHostageTakerCommand->checkContext(commandContext)))
			return false;

		return true;
	}

	qsf::action::Result SpeakToHostageTakerAction::updateAction(const qsf::Clock& jobArguments)
	{
		if (!mTargetEntity.valid())
			return qsf::action::RESULT_DONE;

		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Set equipment
				EntityHelper entityHelper(getEntity());
				entityHelper.showEquipment(equipment::POLICE_MEGAPHONE);

				// Set animation
				AnimationHelper animationHelper(getEntity());
				animationHelper.playAnimation(animationHelper.getAnimationPoliceMegaphoneIdleLoop(), true);

				getComponent().pushAction<TurnToAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(mTargetEntity->getId());

				// Don't allow additional speaking with hostage
				qsf::GameLogicComponent& gameLogicComponent = mTargetEntity->getOrCreateComponentSafe<qsf::GameLogicComponent>();
				GangsterInBuildingLogic& gangsterInBuildingLogic = gameLogicComponent.getOrCreateGameLogicSafe<GangsterInBuildingLogic>();
				gangsterInBuildingLogic.setInUseFromSpeaker(true);

				mCurrentTime = detail::TIME_TO_SPEAK;

				mCurrentState = STATE_USE_MEGAPHONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_USE_MEGAPHONE:
			{
				if (mCurrentTime > qsf::Time::ZERO)
				{
					mCurrentTime -= jobArguments.getTimePassed();

					// Show status overlay bar
					StatusOverlayComponent::showBarOverlay(*mTargetEntity, StatusOverlayComponent::BAR_TYPE_PROGRESS, 1.0f - (mCurrentTime.getSeconds() / detail::TIME_TO_SPEAK.getSeconds()));

					return qsf::action::RESULT_CONTINUE;
				}

				// Hide status overlay bar
				StatusOverlayComponent::hideBarOverlay(*mTargetEntity, StatusOverlayComponent::BAR_TYPE_PROGRESS);

				// Don't allow additional speaking with hostage
				qsf::GameLogicComponent& gameLogicComponent = mTargetEntity->getOrCreateComponentSafe<qsf::GameLogicComponent>();
				GangsterInBuildingLogic& gangsterInBuildingLogic = gameLogicComponent.getOrCreateGameLogicSafe<GangsterInBuildingLogic>();
				gangsterInBuildingLogic.setCanSpeakWithHostageTaker(false);
				gangsterInBuildingLogic.setInUseFromSpeaker(false);

				EntityHelper(getEntity()).deleteActiveEquipment();

				handleHostageTakerGiveUp();

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_DONE;
			}

			case STATE_DONE:
			{
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
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SpeakToHostageTakerAction::handleHostageTakerGiveUp()
	{
		qsf::GameLogicComponent* gameLogicComponent = mTargetEntity->getComponent<qsf::GameLogicComponent>();
		if (nullptr == gameLogicComponent)
			return;

		GangsterInBuildingLogic* gangsterInBuildingLogic = gameLogicComponent->getGameLogic<GangsterInBuildingLogic>();
		if (nullptr == gangsterInBuildingLogic)
			return;

		gangsterInBuildingLogic->onPoliceSpokeToHostageTaker();

		sendUnitHint(gangsterInBuildingLogic->getGangsterGiveUp());
	}

	void SpeakToHostageTakerAction::sendUnitHint(bool gangsterGiveUp)
	{
		CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
		if (nullptr != commandableComponent)
		{
			std::string hintId = (gangsterGiveUp ? "EM5X_HOSTAGE_UNITHINT01" : "EM5X_HOSTAGE_UNITHINT02");
			uint32 playerIndex = commandableComponent->getPlayerId();
			HintHelper::showHint(hintId, playerIndex, qsf::Time::fromSeconds(30.0f));
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

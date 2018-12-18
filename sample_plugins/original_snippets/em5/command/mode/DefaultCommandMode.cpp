// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/DefaultCommandMode.h"
#include "em5/application/Application.h"
#include "em5/game/Game.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/component/effect/HighlightComponent.h"
#include "em5/component/objects/BuoyComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/objects/BarrierTapeComponent.h"
#include "em5/component/person/simplebarriertape/SimpleBarrierTapeComponent.h"
#include "em5/command/CommandContext.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/police/UseStunGrenadeCommand.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/map/query/RayMapQuery.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/command/Command.h>
#include <qsf_game/command/CommandDelegate.h>
#include <qsf_game/command/component/CommandableComponent.h>
#include <qsf_game/command/evaluation/CommandEvaluation.h>
#include <qsf_game/component/base/SelectableComponent.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/component/link/LinkComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/nodes/PathMeshComponent.h>
#include <qsf/component/polygon/LiquidPolygonComponent.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/gui/GuiContext.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Ray.h>
#include <qsf/math/Math.h>
#include <qsf/renderer/particles/ParticlesComponent.h>
#include <qsf/time/clock/Clock.h>
#include <qsf/QsfHelper.h>

// TODO(fw): Remove these, when real feedback exists, they are needed only for test output right now
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/debug/request/CircleDebugDrawRequest.h>


//[-------------------------------------------------------]
//[ Detail implementation                                 ]
//[-------------------------------------------------------]
namespace
{
	namespace detail
	{
		static bool isPrototypeSelectable(const qsf::Prototype& prototype)
		{
			// Entity must have a selectable component
			return (nullptr != prototype.getComponent<qsf::game::SelectableComponent>());
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DefaultCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::DefaultCommandMode");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	void DefaultCommandMode::showVisualCommandFeedback(const qsf::game::CommandContext& context)
	{
		const CommandContext* em5CommandContext = dynamic_cast<const CommandContext*>(&context);
		if (nullptr != em5CommandContext && nullptr != em5CommandContext->mPlayer && em5CommandContext->mPlayer->isPlayerLocal())
		{
			// Use particle effect as visual feedback
			static const glm::vec3 offset(0.0f, 0.3f, 0.0f);
			static const qsf::StringHash PARTICLE_CLICK_MARKER("em5/prefab/default/click_target_marker");
			qsf::ParticlesComponent* particleComponent = MapHelper(QSF_MAINMAP).spawnParticle(PARTICLE_CLICK_MARKER, 1.0f, *context.mTargetPosition + offset, qsf::Math::GLM_VEC3_UNIT_XYZ, qsf::Math::GLM_QUAT_IDENTITY, false);
			if (nullptr != particleComponent)
			{
				particleComponent->setEmitterIntensity(-1.0f);	// Don't apply particles quality
				particleComponent->setRealTimeParticles(true);
				particleComponent->setMaximumRenderingDistance(120.0f);
			}
		}
	}

	bool DefaultCommandMode::isUnitAllowedForSelection(const CommandableComponent& commandableComponent)
	{
		const Player* localPlayer = PlayerManager::getLocalPlayer();
		if (nullptr != localPlayer)
		{
			EntityHelper entityHelper(commandableComponent.getEntity());

			return (entityHelper.isPersonInjured() || entityHelper.isEntityDamaged() || localPlayer->getPlayerIndex() == commandableComponent.getPlayerId());
		}
		return true;
	}

	bool DefaultCommandMode::isUnitAllowedForSelection(const qsf::Entity& entity)
	{
		CommandableComponent* commandableComponent = entity.getComponent<CommandableComponent>();

		// As fallback, ask the parent for top-level entity. Some player units have child entities. The paramedic team for eg. is a meta entity with childs.
		if (nullptr == commandableComponent)
		{
			qsf::LinkComponent* linkComponent = entity.getComponent<qsf::LinkComponent>();
			if (nullptr != linkComponent)
			{
				commandableComponent = linkComponent->getComponentFromEntityOrLinkedParent<CommandableComponent>();
			}
		}

		if (nullptr != commandableComponent)
		{
			const Player* localPlayer = PlayerManager::getLocalPlayer();
			if (nullptr != localPlayer)
			{
				return (localPlayer->getPlayerIndex() == commandableComponent->getPlayerId());
			}
		}
		return false;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DefaultCommandMode::DefaultCommandMode() :
		qsf::game::CommandMode(PLUGINABLE_ID),
		mWaitTimeBetweenChecks(qsf::Time::fromSeconds(0.25f)),
		mMouseoverHighlightedEntityId(qsf::getUninitialized<uint64>()),
		mCommandTargetHighlightedEntityId(qsf::getUninitialized<uint64>()),
		mElapsedWaitTime(qsf::Time::ZERO),
		mLastEntityUnderMouse(nullptr),
		mEntityForCommandHighlighting(nullptr),
		mCurrentMouseCursorIcon(MOUSECURSORICON_DEFAULT)
	{
		// Nothing here
	}

	DefaultCommandMode::~DefaultCommandMode()
	{
		// Nothing here
	}

	qsf::Time DefaultCommandMode::getWaitTimeBetweenChecks() const
	{
		return mWaitTimeBetweenChecks;
	}

	void DefaultCommandMode::setWaitTimeBetweenChecks(const qsf::Time& waitTimeBetweenChecks)
	{
		mWaitTimeBetweenChecks = waitTimeBetweenChecks;
	}

	void DefaultCommandMode::onRayMapQueryResponse(const RayMapQueryResponse& rayMapQueryResponse)
	{
		mLastEntityUnderMouse = rayMapQueryResponse.entity.get();

		// If selection input is disabled don't do entity selectable highlighting
		const GameSettingsGroup& gameSettingsGroup = GameSettingsGroup::getInstanceSafe();
		if (gameSettingsGroup.getSelectionBlocked())
		{
			resetSelectableHighlighting();
		}
		else
		{
			highlightEntityUnderMouse(mLastEntityUnderMouse);
		}

		// If command input is blocked or nothing selected don't do command highlighting
		if (gameSettingsGroup.getCommandsBlocked() || EM5_GAME.getSelectionManager().getSelectionSize() == 0)
		{
			resetToDefault();
		}
		else
		{
			if (nullptr == mEntityForCommandHighlighting || mEntityForCommandHighlighting == mLastEntityUnderMouse)
			{
				// Displays the name of the command which will be executed on "execute command" request
				showCommandWhichCanBeExecuted(mLastEntityUnderMouse, rayMapQueryResponse.position);
			}
			else
			{
				resetToDefault();
			}
		}
	}

	uint64 DefaultCommandMode::getMouseoverHighlightedEntityId() const
	{
		return mMouseoverHighlightedEntityId;
	}

	void DefaultCommandMode::setOnlyCommandHighlightingEntity(qsf::Entity* commandHighlightOnlyThisEntity)
	{
		mEntityForCommandHighlighting = commandHighlightOnlyThisEntity;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void DefaultCommandMode::startup()
	{
		// Reset mouse cursor
		EM5_APPLICATION.getGui().resetMouseCursor();
	}

	void DefaultCommandMode::shutdown()
	{
		mDebugDrawProxy.unregister();
	}

	bool DefaultCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds)
	{
		// Call default implementation
		return qsf::game::CommandMode::executeCommandContext(commandContext, callerIds);
	}

	void DefaultCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		const GameSettingsGroup& gameSettingsGroup = GameSettingsGroup::getInstanceSafe();
		const bool commandsBlocked = gameSettingsGroup.getCommandsBlocked();

		// Do nothing, when global input is blocked, selection and command inputs are blocked or when the mouse cursor is over an HUD element
		if (gameSettingsGroup.getInputBlocked() || (gameSettingsGroup.getSelectionBlocked() && commandsBlocked) || gameSettingsGroup.getMouseOverHudElement())
		{
			resetToDefault();
			resetSelectableHighlighting();
			return;
		}

		mElapsedWaitTime += clock.getTimePassed();

		// Don't do the checks on each update tick
		if (qsf::Time::MAX != mWaitTimeBetweenChecks && mElapsedWaitTime >= mWaitTimeBetweenChecks)
		{
			mElapsedWaitTime = qsf::Time::ZERO;

			// Get entity under current mouse cursor position
			const glm::vec2 normalizedMousePosition = EM5_GUI.getGuiContext().getNormalizedMouseCursorPosition();

			// When nothing is selected we are only interested in selectable entities (no terrain)
			// -> Don't dare to perform this heavy work synchronous inside the main thread, this can and will result in nasty runtime hiccups
			RayMapQueryResponse rayMapQueryResponse(RayMapQueryResponse::ASYNCHRONOUS_RESPONSE | RayMapQueryResponse::POSITION_RESPONSE);
			if (EM5_GAME.getSelectionManager().getSelectionSize() == 0)
			{
				rayMapQueryResponse.flags.set(RayMapQueryResponse::FOR_SELECTION);
				rayMapQueryResponse.flags.set(RayMapQueryResponse::EXCLUDE_TERRAIN);
			}
			rayMapQueryResponse.callback = boost::bind(&DefaultCommandMode::onRayMapQueryResponse, this, _1);
			RayMapQuery(QSF_MAINMAP).getEntityUnderNormalizedMousePosition(normalizedMousePosition, rayMapQueryResponse);
		}

		// If command input is blocked don't do command highlighting
		else if (commandsBlocked)
		{
			resetToDefault();
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DefaultCommandMode::highlightEntityUnderMouse(qsf::Entity* entityUnderMouse)
	{
		qsf::Entity* currentEntity = entityUnderMouse;
		if (nullptr != currentEntity && !::detail::isPrototypeSelectable(*currentEntity))
		{
			currentEntity = nullptr;
		}

		const uint64 currentEntityId = (nullptr != currentEntity) ? currentEntity->getId() : qsf::getUninitialized<uint64>();

		// Check if the mouse over entity changed
		if (currentEntityId != mMouseoverHighlightedEntityId)
		{
			resetSelectableHighlighting();

			// If there is a new one highlight it... in case it is not selected that is
			if (qsf::isInitialized(currentEntityId) && nullptr != currentEntity)
			{
				// If can be highlighted and it's not already highlighted for some other reason, turn on highlighting
				HighlightComponent* highlightComponent = currentEntity->getOrCreateComponent<HighlightComponent>();
				if (nullptr != highlightComponent)
				{
					highlightComponent->setHighlightFlags(HighlightComponent::FLAG_SELECTABLE, true);

					mMouseoverHighlightedEntityId = currentEntityId;
				}
			}
		}
	}

	void DefaultCommandMode::showCommandWhichCanBeExecuted(qsf::Entity* entityUnderMouse, const glm::vec3& hitPosition)
	{
		// Get selection manager instance
		SelectionManager& selectionManager = EM5_GAME.getSelectionManager();

		// Nothing to do if no entity is selected
		if (selectionManager.getSelectionSize() <= 0)
		{
			mLastEntityUnderMouse = nullptr;

			// Nothing to display
			resetToDefault();
			return;
		}

		if (nullptr == entityUnderMouse)
		{
			// Nothing to display
			resetToDefault();
			return;
		}

		Player* player = PlayerManager::getLocalPlayer();
		QSF_CHECK(nullptr != player, "Someone changed the mouse position although there is no local player!", QSF_REACT_THROW);

		// Get command for the entity
		CommandContext commandContext;
		commandContext.mPlayer = player;
		commandContext.mTargetEntity = entityUnderMouse;	// Can be the terrain
		commandContext.mTargetPosition = hitPosition;
		commandContext.mEvaluationReason = CommandContext::EVALUATIONREASON_SHOWCOMMAND_CURSOR;

		qsf::game::CommandDelegate& commandDelegate = QSFGAME_COMMAND.getCommandDelegate();

		// Check whether or not commands may be executed at all in the current situation
		if (!commandDelegate.areCommandsAllowed())
		{
			// Nothing to display
			resetToDefault();
			return;
		}

		// Check if the target or target position is valid, or e.g. outside of the map boundaries
		if (!commandDelegate.isTargetAllowed(commandContext))
		{
			setBlockedMouseCursor();
			return;
		}

		mLastCommandInfo = qsf::game::CommandEvaluationHelper::getCommandForContext(commandContext, selectionManager.getSelectedIdSet(), nullptr);

		updateDisplayOfExecutableCommand();
	}

	void DefaultCommandMode::updateDisplayOfExecutableCommand()
	{
		if (nullptr != mLastEntityUnderMouse && mLastCommandInfo.is_initialized())
		{
			Command* em5_Command = dynamic_cast<em5::Command*>((*mLastCommandInfo).mCommand);
			if (nullptr == em5_Command)
			{
				// Internal error
				resetToDefault();
				return;
			}

			const Command::IconSettings& iconSettings = em5_Command->getIconSettings();
			mCurrentMouseCursorIcon = MOUSECURSORICON_FROM_COMMAND;
			EM5_APPLICATION.getGui().setMouseCursor(iconSettings.mMouseIconName);
#ifndef RETAIL
			// TODO(sw) remove when all commands have an mouse cursor definition set in its IconSettings
			mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);

			// Display command text
			// TODO(sw) remove when all commands have an mouse cursor definition set in its IconSettings
			mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest(em5_Command->getName(), EM5_GUI.getGuiContext().getMouseCursorPosition() + glm::vec2(16, 8)));
#endif
			if (mCommandTargetHighlightedEntityId != mLastEntityUnderMouse->getId())
			{
				resetCommandTargetHighlighting();

				// Only activate target highlighting when the command it allows
				if (em5_Command->highlightTarget())
				{
					mCommandTargetHighlightedEntityId = mLastEntityUnderMouse->getId();

					// Update the highlighting component
					HighlightComponent& highlightComponent = mLastEntityUnderMouse->getOrCreateComponentSafe<HighlightComponent>();
					highlightComponent.setHighlightFlags(HighlightComponent::FLAG_COMMANDTARGET, true);

					// Extra handling for grenade target visualization
					if (em5_Command->getId() == UseStunGrenadeCommand::PLUGINABLE_ID)
					{
						const float stunGrenadeRadius = ActionRangeSpecsGroup::getInstanceSafe().getStunGrenadeRadius();
						highlightComponent.setGroundSpotLightRadius(stunGrenadeRadius);
					}
				}
			}
		}
		else
		{
			if (mLastEntityUnderMouse && nullptr != mLastEntityUnderMouse->getComponent<qsf::game::SelectableComponent>())
			{
				// Mouse is under selectable entity
				resetToDefault();
			}
			else
			{
				setBlockedMouseCursor();
			}
		}
	}

	void DefaultCommandMode::resetToDefault()
	{
		if (mCurrentMouseCursorIcon != MOUSECURSORICON_DEFAULT)
		{
			EM5_APPLICATION.getGui().resetMouseCursor();
			mCurrentMouseCursorIcon = MOUSECURSORICON_DEFAULT;
		}
#ifndef RETAIL
		// TODO(sw) remove when all commands have an mouse cursor definition set in its IconSettings
		mDebugDrawProxy.clearRequests();
#endif

		resetCommandTargetHighlighting();
	}

	void DefaultCommandMode::setBlockedMouseCursor()
	{
		if (mCurrentMouseCursorIcon != MOUSECURSORICON_BLOCKED)
		{
			EM5_APPLICATION.getGui().setMouseCursor("blocked");
			mCurrentMouseCursorIcon = MOUSECURSORICON_BLOCKED;
		}
#ifndef RETAIL
		// TODO(sw) remove when all commands have an mouse cursor definition set in its IconSettings
		mDebugDrawProxy.clearRequests();
#endif
		resetCommandTargetHighlighting();
	}

	void DefaultCommandMode::resetCommandTargetHighlighting()
	{
		if (qsf::isInitialized(mCommandTargetHighlightedEntityId))
		{
			qsf::Entity* highlightedEntity = QSF_MAINMAP.getEntityById(mCommandTargetHighlightedEntityId);
			if (nullptr != highlightedEntity)
			{
				// If can be highlighted turn off command target highlighting
				HighlightComponent* highlightComponent = highlightedEntity->getComponent<HighlightComponent>();
				if (nullptr != highlightComponent)
				{
					highlightComponent->setHighlightFlags(HighlightComponent::FLAG_COMMANDTARGET, false);

					if (highlightComponent->getGroundSpotLightRadius() > 0.0f)
					{
						highlightComponent->setGroundSpotLightRadius(0.0f);
					}
				}
			}
			mCommandTargetHighlightedEntityId = qsf::getUninitialized<uint64>();
		}
	}

	void DefaultCommandMode::resetSelectableHighlighting()
	{
		// In that case dehighlight the previous one if there was any
		if (qsf::isInitialized(mMouseoverHighlightedEntityId))
		{
			qsf::Entity* previousEntity = QSF_MAINMAP.getEntityById(mMouseoverHighlightedEntityId);
			if (nullptr != previousEntity)
			{
				// If can be highlighted turn off selected highlighting
				HighlightComponent* highlightComponent = previousEntity->getComponent<HighlightComponent>();
				if (nullptr != highlightComponent)
				{
					highlightComponent->setHighlightFlags(HighlightComponent::FLAG_SELECTABLE, false);
				}
			}

			qsf::setUninitialized(mMouseoverHighlightedEntityId);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

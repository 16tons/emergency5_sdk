// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/PlaceBarrierTapeCommandMode.h"
#include "em5/action/ActionPriority.h"
#include "em5/application/Application.h"
#include "em5/command/CommandContext.h"
#include "em5/command/police/BarrierTape/PlaceBarrierTapePole1Command.h"
#include "em5/command/police/BarrierTape/PlaceBarrierTapePole2Command.h"
#include "em5/component/objects/BarrierTapeComponent.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/gui/EmergencyGui.h"
#include "em5/gui/IngameHud.h"
#include "em5/gui/hud/CommandWindow.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/command/evaluation/CommandEvaluationHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/gui/GuiContext.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/RayMapQuery.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PlaceBarrierTapeCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::PlaceBarrierTapeCommandMode");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PlaceBarrierTapeCommandMode::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlaceBarrierTapeCommandMode::PlaceBarrierTapeCommandMode() :
		qsf::game::CommandMode(PLUGINABLE_ID),
		mCaller(nullptr),
		mPreviewPole(0)
	{
		// Nothing here
	}

	PlaceBarrierTapeCommandMode::~PlaceBarrierTapeCommandMode()
	{
		// Nothing here
	}

	void PlaceBarrierTapeCommandMode::initialize(qsf::Entity& caller)
	{
		mCaller = &caller;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void PlaceBarrierTapeCommandMode::startup()
	{
		// We are creating a new entity for the barrier tape
		qsf::Entity* barrierTapeEntity = mCaller->getMap().createEntity();
		QSF_CHECK(nullptr != barrierTapeEntity, "Failed to create barrier tape entity", QSF_REACT_THROW);

		barrierTapeEntity->getOrCreateComponentSafe<qsf::TransformComponent>().setTransform(mCaller->getComponentSafe<qsf::TransformComponent>().getTransform());
		mBarrierTapeComponent = barrierTapeEntity->getOrCreateComponentSafe<BarrierTapeComponent>();

		// Initialize barrier tape component
		mBarrierTapeComponent->setFinishedBuilding(false);

		EM5_GUI.getIngameHud().getCommandWindow().enableAllButtons(false);
	}

	void PlaceBarrierTapeCommandMode::shutdown()
	{
		EM5_GUI.getIngameHud().getCommandWindow().enableAllButtons(true);

		if (mBarrierTapeComponent.valid())
		{
			mCaller->getMap().destroyEntityById(mBarrierTapeComponent->getEntityId());
		}
	}

	bool PlaceBarrierTapeCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds)
	{
		if (!commandContext.hasTargetPosition())
			return true;

		if (callerIds.empty() || callerIds.size() > 1)
			return true;

		if (!mBarrierTapeComponent.valid())
			return false;

		if (mPreviewPole > 1)	// If we already have 2 poles there lets break the command
			return true;

		float transparency = LogicSpecsGroup::getInstanceSafe().getPoliceBarrierTransparency();

		// If we have no preview pole yet create it and move it to the clicked position
		if (nullptr == mBarrierTapeComponent->getPoleEntity(mPreviewPole))
		{
			mBarrierTapeComponent->createPole(mPreviewPole);
			mBarrierTapeComponent->setPoleTransparency(mPreviewPole, transparency);
			mBarrierTapeComponent->movePole(mPreviewPole, commandContext.getTargetPosition());
		}

		if (!mBarrierTapeComponent->isPolePositionValid(mPreviewPole))		// Not a valid place, lets continue searching for a place
			return false;

		// Advance to the next pole
		++mPreviewPole;

		if (mPreviewPole == 2)	// We have finished dropping all poles
		{
			// Since I couldn't figure out how to pass two positions from the client to the host for a single command, I'll just push two consecutive commands instead

			{ // Push the command for the first pole
				qsf::game::CommandContext poleCommandContext = commandContext;

				qsf::Entity* poleEntity = mBarrierTapeComponent->getPoleEntity(0);
				QSF_ASSERT(nullptr != poleEntity, "The barrier tape component doesn't contain the pole we throught we added to it just moments ago", QSF_REACT_THROW);
				poleCommandContext.mTargetPosition = EntityHelper(*poleEntity).getPosition();

				// The command mode will be executed on client side in multiplayer mode, so we need to push the actual actions within another command object to the host for things to work as expected
				PlaceBarrierTapePole1Command& commandObject = QSFGAME_COMMAND.getCommandManager().getSafe<PlaceBarrierTapePole1Command>();
				const qsf::game::CommandEvaluationHelper::EntityArray callers = { mCaller };

				// TODO(ca) Review: Is this the correct way to push a command into the usual command processing?
				qsf::game::CommandEvaluationHelper::executeCommand(commandObject, callers, poleCommandContext);
			}

			{ // Push the command for the second pole
				qsf::game::CommandContext poleCommandContext = commandContext;

				qsf::Entity* poleEntity = mBarrierTapeComponent->getPoleEntity(1);
				QSF_ASSERT(nullptr != poleEntity, "The barrier tape component doesn't contain the pole we throught we added to it just moments ago", QSF_REACT_THROW);
				poleCommandContext.mTargetPosition = EntityHelper(*poleEntity).getPosition();

				// The command mode will be executed on client side in multiplayer mode, so we need to push the actual actions within another command object to the host for things to work as expected
				PlaceBarrierTapePole2Command& commandObject = QSFGAME_COMMAND.getCommandManager().getSafe<PlaceBarrierTapePole2Command>();
				const qsf::game::CommandEvaluationHelper::EntityArray callers = { mCaller };
				// TODO(ca) Review: Is this the correct way to push a command into the usual command processing?
				qsf::game::CommandEvaluationHelper::executeCommand(commandObject, callers, poleCommandContext);
			}

			// End command mode
			return true;
		}

		// Continue command mode
		return false;
	}

	void PlaceBarrierTapeCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		GameSettingsGroup& gameSettingsGroup = GameSettingsGroup::getInstanceSafe();

		// Do nothing, when global input is blocked, or when the mouse cursor is over an HUD element
		if (gameSettingsGroup.getInputBlocked() || gameSettingsGroup.getMouseOverHudElement())
		{
			// TODO(fw): Don't we need to remove the preview?
			return;
		}

		mElapsedWaitTime += clock.getTimePassed();

		// Don't do the checks on each update tick
		static const qsf::Time WAIT_TIME_BETWEEN_CHECKS = qsf::Time::fromSeconds(0.1f);
		if (mElapsedWaitTime >= WAIT_TIME_BETWEEN_CHECKS)
		{
			mElapsedWaitTime = qsf::Time::ZERO;

			if (mBarrierTapeComponent.valid())
			{
				if (mPreviewPole < 2)
				{
					// Get the hit position under current mouse cursor
					const glm::vec2 normalizedMousePosition = EM5_GUI.getGuiContext().getNormalizedMouseCursorPosition();
					glm::vec3 hitPosition;

					if (get3dPositionUnderMouse(normalizedMousePosition, hitPosition, mBarrierTapeComponent.get()))
					{
						float transparency = LogicSpecsGroup::getInstanceSafe().getPoliceBarrierTransparency();

						if (nullptr == mBarrierTapeComponent->getPoleEntity(mPreviewPole))
						{
							mBarrierTapeComponent->createPole(mPreviewPole);
							mBarrierTapeComponent->setPoleTransparency(mPreviewPole, transparency);
						}
						// Place a preview pole
						mBarrierTapeComponent->movePole(mPreviewPole, hitPosition);

						if (mPreviewPole == 1 && !mBarrierTapeComponent->hasTape())
						{
							mBarrierTapeComponent->createTape(BarrierTapeComponent::POLE_TO_POLE);
							mBarrierTapeComponent->setTapeTransparency(transparency);
						}
					}
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool PlaceBarrierTapeCommandMode::get3dPositionUnderMouse(const glm::vec2& normalizedMousePosition, glm::vec3& outPosition, BarrierTapeComponent* barrierTapeComponent)
	{
		// We can't use RayMapQuery(QSF_MAINMAP).getEntityUnderNormalizedMousePosition because that will hit the barrier tape itself once it's placed.

		// Find the first component hit under the mouse cursor
		boost::optional<qsf::Ray> ray = qsf::RayMapQuery::getRayByRenderWindowNormalizedPosition(*EM5_APPLICATION.getRenderWindow(), normalizedMousePosition.x, normalizedMousePosition.y);
		if (!ray.is_initialized())
		{
			return false;
		}

		// Hit everything
		qsf::RayMapQuery::QueryParameters parameters;
		parameters.mInitialClosestDistance = -1;
		parameters.mAccuracy = qsf::RayMapQuery::ACCURACY_MESH; // We want mesh level accuracy
		parameters.mQueryIncludeFlags = qsf::RayMapQuery::INCLUDE_OGRE_ENTITIES | qsf::RayMapQuery::INCLUDE_TERRAIN;
		parameters.mFilterCallback = [barrierTapeComponent](qsf::Component& component, qsf::RayMapQuery::QueryAccuracy& queryAccuracy) -> bool
		{
			// Ignore the barrier poles
			qsf::Entity& entity = component.getEntity();
			return &entity != barrierTapeComponent->getPoleEntity(0) && &entity != barrierTapeComponent->getPoleEntity(1);
		};

		qsf::RayMapQueryResponse rayMapQueryResponse(qsf::RayMapQueryResponse::POSITION_RESPONSE);
		qsf::RayMapQuery(QSF_MAINMAP).getFirstHitAlongRay(*ray, rayMapQueryResponse, parameters);
		outPosition = rayMapQueryResponse.position;

		// Did the ray hit a component?
		return (nullptr != rayMapQueryResponse.component);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

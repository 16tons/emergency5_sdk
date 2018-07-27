// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ambulance/historic/CleanHomeAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/base/ContainerCategory.h"
#include "em5/component/overlay/BuildingOverlayComponent.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/objects/HistoricPlagueBuildingComponent.h"
#include "em5/command/ambulance/historic/CleanHomeCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/Game.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"

#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialManager.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>
#include <qsf/QsfHelper.h>

#include <glm/gtx/norm.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier CleanHomeAction::ACTION_ID = "em5::CleanHomeAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CleanHomeAction::CleanHomeAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	CleanHomeAction::~CleanHomeAction()
	{
		ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());
	}

	void CleanHomeAction::init(qsf::Entity& buildingEntity)
	{
		mTargetEntity = buildingEntity;
	}

	uint64 CleanHomeAction::getTargetEntityId() const
	{
		return (mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void CleanHomeAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool CleanHomeAction::onStartup()
	{
		if (!mTargetEntity.valid())
			return false;

		glm::vec3 targetPosition = mTargetEntity->getComponentSafe<qsf::TransformComponent>().getPosition();
		const BuildingComponent& buildingComponent = mTargetEntity->getComponentSafe<BuildingComponent>();
		if (buildingComponent.hasDoor())
			targetPosition = buildingComponent.getDoorPosition();

		// Make the distance check
		const glm::vec3& callerPosition = getEntity().getComponentSafe<qsf::TransformComponent>().getPosition();
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getEnterBuildingDistance();
		const float currentSquaredDistance = glm::distance2(targetPosition, callerPosition);
		if (currentSquaredDistance > maxDistance * maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &getEntity();
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		CleanHomeCommand* cleanHomeCommand = static_cast<CleanHomeCommand*>(QSFGAME_COMMAND.getCommandManager().getById<CleanHomeCommand>(CleanHomeCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != cleanHomeCommand, "CleanHomeAction::onStartup(): Could not find clean home command instance", return false);

		// Here we skip the caller check, because the states for the pestilence doctor are simple
		if (!(cleanHomeCommand->checkContext(commandContext)))
			return false;

		return true;
	}

	qsf::action::Result CleanHomeAction::updateAction(const qsf::Clock& clock)
	{
		// Get the target entity instance
		QSF_CHECK(mTargetEntity.valid(), "CleanHomeAction::updateAction() unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		EntityHelper entityHelper(getEntity());

		switch (mState)
		{
			case STATE_INIT:
			{
				// Set the can spawn value to false
				HistoricPlagueBuildingComponent::getInstance().setMustClean(*mTargetEntity, false);

				// Reserve the entity
				ReserveLogic::createReservation<ReserveWaitForFinishLogic>(*mTargetEntity, getEntityId());

				entityHelper.fadeOut();

				mState = STATE_FADEOUT;

				// Fall through by design
			}

			case STATE_FADEOUT:
			{
				if (entityHelper.isFading())
				{
					// Wait till entity has faded out
					return qsf::action::RESULT_CONTINUE;
				}

				entityHelper.enterContainer(*mTargetEntity, container::CONTAINERTYPE_PROGRESS);

				mMarkXDecalEntity = HistoricPlagueBuildingComponent::getMarkXDecalFromEntity(*mTargetEntity);
				if (mMarkXDecalEntity.valid())
				{
					setMarkXAlpha(0.05f);
				}

				// Play audio
				AudioHelper::playHistoricDoctorCleanHome(mAudioProxy, getEntity());

				// TODO(mk) Read the timing value from specs file
				mProgressTime = qsf::Time::fromSeconds(5.0f);
				mState = STATE_PROGRESS_INSIDE;

				// Fall through by design
			}

			case STATE_PROGRESS_INSIDE:
			{
				// Build timer for the progress
				if (mProgressTime > qsf::Time::ZERO)
				{
					mProgressTime -= clock.getTimePassed();
					BuildingOverlayComponent::showProgress(mTargetEntity.getSafe(), 1, getEntity(), 1.0f - (mProgressTime.getSeconds() / 5.0f));

					// Wait till time is over
					return qsf::action::RESULT_CONTINUE;
				}

				BuildingOverlayComponent::hideOverlay(mTargetEntity.getSafe(), getEntity());

				cleanHome();
				entityHelper.leaveContainer(false);

				mState = STATE_STARTCLEANING_ANIMATION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_STARTCLEANING_ANIMATION:
			{
				bool lookToDoor = mMarkXDecalEntity.valid();
				entityHelper.placeEntityAtBuildingDoor(mTargetEntity.getSafe(), lookToDoor);
				entityHelper.fadeIn();

				if (mMarkXDecalEntity.valid())
				{
					playDoctorAnimation();
					mProgressTime = qsf::Time::fromSeconds(0.0f);	// Reset and reuse the variable

					mState = STATE_ANIMATION;
				}
				else
				{
					mState = STATE_DONE;
				}

				// Fall through by design
			}

			case STATE_ANIMATION:
			{
				// This timing fits to the animation of the doctor_historic.
				mProgressTime += clock.getTimePassed();
				qsf::Time phase1Ends = qsf::Time::fromSeconds(27.0f / 30.0f);	// Phase 1 is waiting (Animation: Doctor lifts his arm)
				qsf::Time phase2Ends = qsf::Time::fromSeconds(51.0f / 30.0f);	// Phase 2 fading from 0-50%  (Animation: Doctor writes first cross part "\")
				qsf::Time phase3Ends = qsf::Time::fromSeconds(70.0f / 30.0f);	// Phase 3 is waiting (Animation: Doctor moves his arm to the other side, no marking)
				qsf::Time phase4Ends = qsf::Time::fromSeconds(97.0f / 30.0f);	// Phase 4 fading from 50-100% (Animation: Doctor writes second cross part "/")

				AudioHelper::playHistoricDoctorCleanHome2(mAudioProxy, getEntity());
				if (mProgressTime < phase1Ends)
				{
					return qsf::action::RESULT_CONTINUE;
				}
				else if (mProgressTime < phase2Ends)
				{
					activateMarkX();
					float alpha = (mProgressTime.getSeconds() - phase1Ends.getSeconds()) / (phase2Ends.getSeconds() - phase1Ends.getSeconds());
					setMarkXAlpha(alpha / 2.0f);	// Clamp from 0-50%
					return qsf::action::RESULT_CONTINUE;
				}
				else if (mProgressTime < phase3Ends)
				{
					setMarkXAlpha(0.5f);	// Clamp from 0-50%
					return qsf::action::RESULT_CONTINUE;
				}
				else if (mProgressTime < phase4Ends)
				{
					float alpha = (mProgressTime.getSeconds() - phase3Ends.getSeconds()) / (phase4Ends.getSeconds() - phase3Ends.getSeconds());
					alpha = (alpha / 2.0f) + 0.5f;
					setMarkXAlpha(alpha);
					return qsf::action::RESULT_CONTINUE;
				}

				setMarkXAlpha(1.0f);

				HistoricPlagueBuildingComponent::getInstance().showPestIconForBuilding(*mTargetEntity);

				// Fall through by design
			}

			case STATE_DONE:
			{
				// Emit the message for checking the building
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_PLAGUEBUILDING_CHECKED, mTargetEntity->getId()));
				return qsf::action::RESULT_DONE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CleanHomeAction::playDoctorAnimation() const
	{
		getComponent().pushAction<qsf::WaitAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(qsf::Time::fromSeconds(1.0f));
		getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_BEFORE_CURRENT).init(AnimationHelper(getEntity()).getAnimationDoctorHistoricCleanHome());
	}

	void CleanHomeAction::cleanHome()
	{
		// Nothing to do here
	}

	void CleanHomeAction::setMarkXAlpha(float alphaValue)
	{
		if (mMarkXDecalEntity.valid())
		{
			// Invert the alpha
			// -> Setting the alpha value to zero looks wrong for the GFX
			float invertedAlpha = 1.0f - alphaValue;
			invertedAlpha = glm::clamp(invertedAlpha, 0.05f, 1.0f);

			// Tell the material variation manager
			QSF_MATERIAL.getMaterialVariationManager().setEntityMaterialPropertyValue(mMarkXDecalEntity.getSafe(), "AlphaReference", qsf::MaterialPropertyValue::fromFloat(invertedAlpha));
		}
	}

	void CleanHomeAction::activateMarkX()
	{
		if (mMarkXDecalEntity.valid())
		{
			qsf::MeshComponent& meshComponent = mMarkXDecalEntity.getSafe().getComponentSafe<qsf::MeshComponent>();
			if (!meshComponent.isActive())
			{
				{ // Ensure the alpha reference value of the X-mark material is set to 1 (fully invisible) by default
				  // -> If we don't do this, we risk seeing a graphics glitch
				  // -> X-mark material: Global asset ID 651874 - Local asset name "em5/material/default/decal_x" - Local asset ID 1046360747
					static qsf::MaterialManager& materialManager = QSF_MATERIAL.getMaterialManager();
					qsf::Material* material = materialManager.findElement(qsf::StringHash("651874"));
					if (nullptr != material)
					{
						material->setPropertyById("AlphaReference", qsf::MaterialPropertyValue::fromFloat(1.0f));
					}
				}

				// Activate the mesh component
				meshComponent.setActive(true);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

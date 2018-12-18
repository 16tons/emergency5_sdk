// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ambulance/PlagueQuickTestAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/activity/ActivityComponent.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/command/ambulance/PlagueQuickTestCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/health/HealthHelper.h"
#include "em5/health/HealthComponent.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/specs/HealthSpecsGroup.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/units/UnitHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier PlagueQuickTestAction::ACTION_ID = "em5::PlagueQuickTestAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PlagueQuickTestAction::PlagueQuickTestAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mIsDoctor(false)
	{
		// Nothing here
	}

	PlagueQuickTestAction::~PlagueQuickTestAction()
	{
		// Remove reservation again
		ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());

		// Remove invincibility
		HealthHelper(getEntity()).setInvincible(false);
	}

	void PlagueQuickTestAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntity = &targetEntity;
		mTargetHealthComponent = mTargetEntity->getComponent<HealthComponent>();

		// To avoid multiple doctors at the same target, we reserve the target here.
		// It is only checked in command for automatism. If the doctor starts treatment, the logic is changed. Only reserve if no other reserve the target (its not bad if a moveTo reservation is missing).
		// Reserve the entity
		if (ReserveLogic::checkReservation(targetEntity, getEntityId()))
		{
			ReserveLogic::createReservation<ReserveMoveToTargetLogic>(targetEntity, getEntityId());
		}
	}

	uint64 PlagueQuickTestAction::getTargetEntityId() const
	{
		return mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void PlagueQuickTestAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialization
		// serializer.serialize(mTargetEntity);
		serializer.serializeAs<uint16>(mState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool PlagueQuickTestAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		if (!mTargetEntity.valid())
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getTreatPersonDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(mTargetEntity.getSafe(), callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = mTargetEntity.get();

		// Call "checkCaller" && "checkContext" of the commando for validation
		PlagueQuickTestCommand* plagueQuickTestCommand = static_cast<PlagueQuickTestCommand*>(QSFGAME_COMMAND.getCommandManager().getById<PlagueQuickTestCommand>(PlagueQuickTestCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != plagueQuickTestCommand, "PlagueQuickTestAction::onStartup(): Could not find plague quick test command instance", return false);
		if (!(plagueQuickTestCommand->checkCallerWithoutPriority(*commandContext.mCaller) && plagueQuickTestCommand->checkContext(commandContext)))
			return false;

		// Check for the reserve logic
		if (!ReserveLogic::checkReservation(mTargetEntity.getSafe(), getEntityId()))
			return false;

		// TODO(mk) Add this code for multiplayer support, don't know if this is enough or wrong, pls review this
		// Try to get event id component lock
		//if (!EventIdComponent::tryLockForTeam(mTargetEntity.getSafe(), getEntity()))
		//{
		//	return false;
		//}

		mIsDoctor = UnitHelper(getEntity()).isAmbulanceDoctor();

		// Mark this patient as being treated right now
		mTargetEntity->getComponentSafe<HealthComponent>().setTreatmentActive(true);

		return true;
	}

	qsf::action::Result PlagueQuickTestAction::updateAction(const qsf::Clock&)
	{
		qsf::Entity& callerEntity = getEntity();
		switch (mState)
		{
			case STATE_INIT:
			{
				// Change reserve logic from "moveto" to "ReserveDoctorLogic" (has higher priority)
				ReserveLogic::deleteOwnReservation(mTargetEntity, getEntityId());
				ReserveLogic::createReservation<ReserveDoctorLogic>(mTargetEntity.getSafe(), getEntityId());

				// Make patient invincible
				if (mTargetHealthComponent.valid())
				{
					mTargetHealthComponent->setInvincible(true);
				}

				// Hide current equipment
				EntityHelper(callerEntity).storeActiveEquipmentInInventory();

				{ // Stop all activities of the target
					mTargetEntity->destroyComponent<ActivityComponent>();

					// Remove all actions except the blocking ones; especially remove the actions pushed by activity
					mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>().clearPlan();
				}

				getComponent().pushAction<TurnToAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(mTargetEntity->getId());

				mState = STATE_TURNTO;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_TURNTO:
			{
				mMedicationEquipment = MapHelper(callerEntity.getMap()).createObjectByLocalPrefabAssetId(qsf::StringHash("em5/prefab/" + equipment::AMBULANCE_MEDICATION));
				if (mMedicationEquipment.valid())
				{
					EntityHelper equipmentEntityHelper(*mMedicationEquipment);
					equipmentEntityHelper.linkEquipmentToEntity(getEntity());
					equipmentEntityHelper.enableShineThroughEffect();
				}

				// Animation for the target
				AnimationHelper(*mTargetEntity).playAnimation(AnimationHelper(*mTargetEntity).getAnimationGenericLightStanding());
				mTargetEntity->getOrCreateComponentSafe<qsf::ActionComponent>().pushAction<qsf::WaitAction>().init(qsf::Time::fromSeconds(5.0f));	// This line is important to avoid moving a entity with still active move to despawn component (neccesary for the activity system)

				getComponent().pushAction<PlayAnimationAction>(action::BLOCKING, qsf::action::INSERT_AT_FRONT).init(AnimationHelper(getEntity()).getAnimationDoctorPlagueQuickTest());
				mState = STATE_WAIT;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_WAIT:
			{
				if (AnimationHelper(callerEntity).isAnimationPlaying())
				{
					// Wait for animation end
					return qsf::action::RESULT_CONTINUE;
				}

				mState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DONE:
			{
				if (mTargetHealthComponent.valid())
				{
					mTargetHealthComponent->healAllInjuries();
					mTargetHealthComponent->setCanDoContaminationQuickTest(false);

					// Give the target a following injury (much better than the current)
					const bool isInfectedByPlague = qsf::Random::getRandomChance(HealthSpecsGroup::getInstanceSafe().getPlagueQuickTestPositivChance());
					mTargetHealthComponent->applyInjuryById((isInfectedByPlague ? injury::PLAGUE_CONTAMINATION_TEST_POSITIV : injury::PLAGUE_CONTAMINATION_TEST_NEGATIV), EntityHelper(mTargetEntity.getSafe()).getFreeplayEvent());

					if (!isInfectedByPlague)
					{
						mTargetHealthComponent->setImmunityType(HealthComponent::IMMUNITY_CONTAMINATION_PLAGUE, true);
					}

					// Emit a message
					qsf::MessageParameters parameters;
					parameters.setParameter("isInfectedByPlague", isInfectedByPlague);
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_PERSON_PLAGUEQUICKTEST, mTargetEntity->getId()), parameters);

					// Show current equipment
					EntityHelper(callerEntity).storeInventoryEquipmentAsActive();

					mTargetHealthComponent->setInvincible(false);
				}
				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}

	void PlagueQuickTestAction::onShutdown()
	{
		if (mTargetHealthComponent.valid())
		{
			// Abort the treatment
			mTargetHealthComponent->setTreatmentActive(false);
			mTargetHealthComponent->setInvincible(false);
		}

		// Deleting doctor equipment
		if (mMedicationEquipment.valid())
		{
			MapHelper(mMedicationEquipment->getMap()).destroyEntity(mMedicationEquipment.getSafe());
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

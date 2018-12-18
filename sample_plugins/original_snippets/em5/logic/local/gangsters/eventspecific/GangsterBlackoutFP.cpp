// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/eventspecific/GangsterBlackoutFP.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/SignalAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/component/objects/UsableByEngineerComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/targetpoint/GotoObjectFrontsideTargetPointProvider.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/goal/ReachObjectGoal.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterBlackoutFP::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterBlackoutFP");
	const qsf::NamedIdentifier GangsterBlackoutFP::MESSAGE_ON_EBOX_DAMAGE = "GangsterBlackoutFP_onEboxDamage";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterBlackoutFP::GangsterBlackoutFP() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID)
	{
		// We want to receive "onUnitsSpotted" and "onNoUnitsSpotted" callbacks
		mUseOldSpottedUnitsLogic = true;
	}

	void GangsterBlackoutFP::setTargetEboxes(const std::vector<qsf::Entity*>& eboxes)
	{
		mTargetEboxes.clear();
		for (qsf::Entity* ebox : eboxes)
		{
			mTargetEboxes.push_back(ebox);
		}
	}

	void GangsterBlackoutFP::setTimeToDamageEbox(qsf::Time time)
	{
		mTimeToDamageEbox = time;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	void GangsterBlackoutFP::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		// Nothing special to do in here, all gangster behavior is handled in "onIdle"
	}

	void GangsterBlackoutFP::onIdle()
	{
		// Go to next ebox
		if (mTargetEboxes.empty())
		{
			// His work is done, now away from here!
			escape(EscapeType::ESCAPE_FOOT_LONG);
		}
		else
		{
			qsf::Entity& nextTarget = mTargetEboxes.front().getSafe();
			qsf::ActionComponent& actionComponent = mActionComponent.getSafe();

			// Check if he is already there
			if (qsf::game::DistanceHelper::get2dDistance(*getEntity(), nextTarget) > 2.0f)
			{
				actionComponent.pushAction<MoveAction>().init(new qsf::ai::ReachObjectGoal(*getEntity(), nextTarget, GotoObjectFrontsideTargetPointProvider::TARGET_POINT_ID), MovementModes::MOVEMENT_MODE_RUN);
			}
			else if (nullptr == nextTarget.getComponent<UsableByEngineerComponent>())
			{
				// Turn to box and do some animation
				actionComponent.pushAction<TurnToAction>().init(nextTarget.getId());
				actionComponent.pushAction<PlayAnimationAction>().init(AnimationHelper(*getEntity()).getAnimationEngineerRepairStandingLoop(), true, true, false, qsf::Time::ZERO, mTimeToDamageEbox);

				// Signal the event what's going on
				//  -> It will do the actual damaging
				actionComponent.pushAction<SignalAction>().init(qsf::MessageConfiguration(MESSAGE_ON_EBOX_DAMAGE, nextTarget.getId()), qsf::MessageConfiguration(""));
			}
			else
			{
				// Box was just damaged, go on to the next one
				mTargetEboxes.pop_front();
			}
		}
	}

	void GangsterBlackoutFP::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		QSF_CHECK(!sightedPoliceUnits.empty(), "sightedPoliceUnits is empty", return);

		// Attacking uses priority "action::COMMAND_HIGH"
		if (mActionComponent->getCurrentPriority() < action::COMMAND_HIGH)
		{
			// Prefer attacking persons, not vehicles
			for (const SpottedUnit& spottedUnit : sightedPoliceUnits)
			{
				if (nullptr != spottedUnit.mEntity->getComponentById("em5::PersonComponent"))
				{
					attack(*spottedUnit.mEntity);
					break;
				}
			}
		}
	}

	void GangsterBlackoutFP::onNoUnitsSpotted()
	{
		// Nothing here yet
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

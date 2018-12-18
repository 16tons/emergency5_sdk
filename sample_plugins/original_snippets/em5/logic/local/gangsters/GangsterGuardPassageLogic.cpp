// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/GangsterGuardPassageLogic.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/ai/MovementModes.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterGuardPassageLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterGuardPassageLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterGuardPassageLogic::GangsterGuardPassageLogic() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID)
	{
		mEquippedWeaponTypes.set(weapon::PISTOL);
		mEquippedWeaponTypes.set(weapon::BASEBALLBAT);
		mEquippedWeaponTypes.set(weapon::KNIFE);

		// This gangster type always show his weapon
		mAlwaysShowWeapon = true;

		// We want to receive "onUnitsSpotted" and "onNoUnitsSpotted" callbacks
		mUseOldSpottedUnitsLogic = true;
	}

	void GangsterGuardPassageLogic::setGuardedPoint(glm::vec3 const& point)
	{
		mGuardedPoint = point;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	void GangsterGuardPassageLogic::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		qsf::Entity* entity = getEntity();
		QSF_CHECK(nullptr != entity, "Entity does not exist!", return);

		qsf::ActionComponent* ac = entity->getComponent<qsf::ActionComponent>();
		if (ac->getPlan().isEmpty() && (glm::distance2(entity->getComponentSafe<qsf::TransformComponent>().getPosition(), mGuardedPoint) > 5.0f * 5.0f))
		{
			// Nothing to do and too far away from guarded point
			qsf::ai::ReachSinglePointGoal* goal = new qsf::ai::ReachSinglePointGoal(qsf::logic::TargetPoint(mGuardedPoint, false, 4.f, 1.f));
			ac->pushAction<MoveAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT).init(goal, MovementModes::MOVEMENT_MODE_RUN);
		}
	}

	void GangsterGuardPassageLogic::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		for (const SpottedUnit& policeUnit : sightedPoliceUnits)
		{
			if (policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_POLICE
				|| policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_SEK
				|| policeUnit.mEventTag == CommandableComponent::UNITTAG_POLICE_ROBOT)
			{
				// Gangster becomes a "killer taskforce". But depending on what weapons are equipped he behaves differently (refer to GDD: fixed, move)
				getEntity()->getComponentSafe<PersonComponent>().changeGangsterLogic(qsf::StringHash("em5::GangsterKillerTaskforceLogic"));

				// Get us out of here right now
				break;
			}
		}
	}


	bool GangsterGuardPassageLogic::onStartup()
	{
		if (!GangsterBaseLogic::onStartup())
			return false;

		if (nullptr == mTransformComponent.get())
			return false;

		mGuardedPoint = mTransformComponent->getPosition();
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/GangsterGaper.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterGaper::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterGaper");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterGaper::GangsterGaper() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID),
		mCurrentTargetId(qsf::getUninitialized<uint64>())
	{
		mEquippedWeaponTypes.set(weapon::FIST);

		// This gangster type always show his weapon
		mAlwaysShowWeapon = true;
	}

	bool GangsterGaper::hasTarget() const
	{
		return qsf::isInitialized(mCurrentTargetId);
	}

	void GangsterGaper::setTarget(uint64 targetId)
	{
		mCurrentTargetId = targetId;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	void GangsterGaper::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		QSF_CHECK(nullptr != getEntity(), "No entity exists! This must not happen!", return);

		qsf::ActionComponent* actionComponent = getEntity()->getOrCreateComponent<qsf::ActionComponent>();
		if (!actionComponent->getPlan().isEmpty())
		{
			// Gangster got work to do, let him finish that
			return;
		}

		qsf::Entity* currentTarget = nullptr;
		if (qsf::isInitialized(mCurrentTargetId))
		{
			// Stick with the current target as it is still valid
			currentTarget = getMap().getEntityById(mCurrentTargetId);
		}

		if (nullptr == currentTarget)
		{
			qsf::setUninitialized(mCurrentTargetId);
		}
		else
		{
			// The gangster has a registered target
			mCurrentTargetId = currentTarget->getId();

			if (!EntityHelper(*currentTarget).isValidGangsterEventTarget(*getEntity(), false))
			{
				// Reset target -> search new one
				qsf::setUninitialized(mCurrentTargetId);
				return;
			}
		}
	}

	void GangsterGaper::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		// Nothing here (Everything is handled in the onSimulationUpdate() )
	}

	void GangsterGaper::onNoUnitsSpotted()
	{
		// Nothing here (Everything is handled in the onSimulationUpdate() )
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2014 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/gangster/SetBuildingOnFireAction.h"
#include "em5/action/base/GuidePersonAction.h"
#include "em5/action/equipment/Equipment.h"
#include "em5/action/ActionPriority.h"
#include "em5/command/CommandContext.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/logic/local/police/PolicemanLogic.h"
#include "em5/specs/SpecsGroupManager.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/map/EntityHelper.h"
#include "em5/game/Game.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier SetBuildingOnFireAction::ACTION_ID = "em5::SetBuildingOnFireAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SetBuildingOnFireAction::SetBuildingOnFireAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mRelatedEvent(nullptr)
	{
		// Nothing here
	}

	SetBuildingOnFireAction::~SetBuildingOnFireAction()
	{
		// Nothing here
	}

	void SetBuildingOnFireAction::init(const qsf::Entity& target, FreeplayEvent& relatedEvent)
	{
		mTargetEntityId = target.getId();
		mRelatedEvent = &relatedEvent;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void SetBuildingOnFireAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool SetBuildingOnFireAction::onStartup()
	{
		return true;
	}

	qsf::action::Result SetBuildingOnFireAction::updateAction(const qsf::Clock&)
	{
		// Get the target entity instance
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "SetBuildingOnFireAction::updateAction() The EM5 pick set building on fire action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		// Get the person component
		BuildingComponent* building = targetEntity->getComponent<BuildingComponent>();
		QSF_CHECK(nullptr != building, "The EM5 set building on fire action was unable to retrieve the building component", return qsf::action::RESULT_DONE);

		// Set the building on fire!
		EntityHelper(*targetEntity).startFire(mRelatedEvent);

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/gangster/ReachVictimAction.h"
#include "em5/action/base/WaitForEntityAction.h"
#include "em5/command/CommandContext.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ReachVictimAction::ACTION_ID = "em5::ReachVictimAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ReachVictimAction::ReachVictimAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	ReachVictimAction::~ReachVictimAction()
	{
		// Nothing here
	}

	void ReachVictimAction::init(const qsf::Entity& targetEntity)
	{
		mTargetEntityId = targetEntity.getId();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ReachVictimAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool ReachVictimAction::onStartup()
	{
		// Get the caller and target entity
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getPickPocketDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		return true;
	}

	qsf::action::Result ReachVictimAction::updateAction(const qsf::Clock&)
	{
		// Get the target entity instance
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != targetEntity, "ReachVictimAction::updateAction() The EM5 pick pocket action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		qsf::ActionComponent* targetActionComponent = targetEntity->getComponent<qsf::ActionComponent>();
		if (nullptr == targetActionComponent)
			return qsf::action::RESULT_DONE;

		qsf::Action* currentAction = targetActionComponent->getCurrentAction();
		if (nullptr != currentAction && currentAction->getTypeId() == WaitForEntityAction::ACTION_ID)
		{
			WaitForEntityAction* waitAction = static_cast<WaitForEntityAction*>(currentAction);
			if (waitAction->getTargetId() == getEntityId())
				waitAction->finish();
		}
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

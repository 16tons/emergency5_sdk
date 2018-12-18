// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/RobotScanAction.h"
#include "em5/command/police/RobotScanCommand.h"
#include "em5/command/CommandContext.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/QsfGameHelper.h>

#include <qsf/map/Map.h>
#include <qsf/message/MessageSystem.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier RobotScanAction::ACTION_ID	 = "em5::RobotScanAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RobotScanAction::RobotScanAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	RobotScanAction::~RobotScanAction()
	{
		// Nothing here
	}

	void RobotScanAction::init(uint64 targetEntityId)
	{
		mTargetEntityId = targetEntityId;
	}

	uint64 RobotScanAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void RobotScanAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool RobotScanAction::onStartup()
	{
		qsf::Entity& callerEntity = getEntity();
		qsf::Entity* targetEntity = getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getTreatPersonDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		// Build context
		CommandContext commandContext;
		commandContext.mCaller = &callerEntity;
		commandContext.mTargetEntity = targetEntity;

		// Call "checkCaller" && "checkContext" of the commando for validation
		RobotScanCommand* robotScanCommand = static_cast<RobotScanCommand*>(QSFGAME_COMMAND.getCommandManager().getById<RobotScanCommand>(RobotScanCommand::PLUGINABLE_ID));
		QSF_CHECK(nullptr != robotScanCommand, "RobotScanAction::onStartup(): Could not find robot scan command instance", return false);
		if (!(robotScanCommand->checkCaller(*commandContext.mCaller) && robotScanCommand->checkContext(commandContext)))
			return false;

		return true;
	}

	qsf::action::Result RobotScanAction::updateAction(const qsf::Clock&)
	{
		// TODO(mk) This action is just dummy

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

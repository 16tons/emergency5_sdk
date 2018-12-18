// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/police/PoliceVehicleLogic.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/command/CommandContext.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/QsfGameHelper.h>
#include <qsf_game/command/Command.h>
#include <qsf_game/command/CommandSystem.h>
#include <qsf_game/command/CommandManager.h>
#include <qsf_game/command/evaluation/CommandEvaluation.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PoliceVehicleLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::PoliceVehicleLogic");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 PoliceVehicleLogic::ACTION_PRIORITY = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PoliceVehicleLogic::PoliceVehicleLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool PoliceVehicleLogic::onStartup()
	{
		// Register message
		mMessageProxyContents.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_POLICE_VEHICLE_READY_TO_DEPART, getEntityId()), boost::bind(&PoliceVehicleLogic::onReadyToDepart, this, _1));

		return true;
	}

	void PoliceVehicleLogic::onShutdown()
	{
		mMessageProxyContents.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PoliceVehicleLogic::onReadyToDepart(const qsf::MessageParameters& parameters)
	{
		// Trigger self-executed command
		Player* localPlayer = PlayerManager::getLocalPlayer();
		QSF_CHECK(nullptr != localPlayer, "Someone triggered a command although there is no local player!", QSF_REACT_THROW);

		// Check and execute command
		CommandContext context;
		context.mPlayer = localPlayer;

		qsf::game::CommandEvaluation::EntityIdSet entityIdSet;
		entityIdSet.insert(getEntityId());

		const qsf::game::Command* command = QSFGAME_COMMAND.getCommandManager().getById<qsf::game::Command>("em5::ReturnToBaseCommand");
		QSFGAME_COMMAND.getCommandEvaluation().checkAndExecuteCommand(*command, context, entityIdSet);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

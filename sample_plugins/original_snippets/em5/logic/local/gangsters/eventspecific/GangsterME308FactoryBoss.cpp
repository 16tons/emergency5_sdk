// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/eventspecific/GangsterME308FactoryBoss.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/fire/component/FireReceiverComponent.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterME308FactoryBoss::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterME308FactoryBoss");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterME308FactoryBoss::GangsterME308FactoryBoss() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID)
	{
		//mSetGangsterFlagOnStartup = false;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool GangsterME308FactoryBoss::onStartup()
	{
		if (!GangsterBaseLogic::onStartup())
			return false;

		// Make sure he is immune to fire
		getEntitySafe().getOrCreateComponentSafe<FireReceiverComponent>().setActive(false);

		return true;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	void GangsterME308FactoryBoss::onWaypointReached(const Waypoint& waypoint)
	{
		// Turn in the waypoint's direction
		const glm::quat& rotation = waypoint.mEntity.getSafe().getComponentSafe<qsf::TransformComponent>().getRotation();
		qsf::ActionComponent& actionComponent = getEntitySafe().getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.pushAction<TurnToAction>().initWithRotation(rotation);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

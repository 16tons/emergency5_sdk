// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/police/SEKRobotLogic.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"
#include "em5/map/EntityHelper.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/job/JobArguments.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SEKRobotLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::SEKRobotLogic");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 SEKRobotLogic::ACTION_PRIORITY = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SEKRobotLogic::SEKRobotLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID),
		mConnectedTransporterId(qsf::getUninitialized<uint64>()),
		mIsIntact(true)
	{
		// Nothing here
	}

	uint64 SEKRobotLogic::getConnectedTransporterId() const
	{
		return mConnectedTransporterId;
	}

	void SEKRobotLogic::setConnectedTransporterId(uint64 transporter)
	{
		mConnectedTransporterId = transporter;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool SEKRobotLogic::onStartup()
	{
		// Update calls in one second time intervals
		qsf::jobs::JobConfiguration jobConfiguration;
		jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&SEKRobotLogic::updateSimulation, this, _1), jobConfiguration);

		// Listen if the transporter gets destroyed
		mMessageProxyConnectedTransporterDestroyed.registerAt(qsf::MessageConfiguration(Messages::EM5_FIRECOMPONENT_START_BURNING, mConnectedTransporterId), boost::bind(&SEKRobotLogic::onTransporterStartsBurning, this, _1));

		return true;
	}

	void SEKRobotLogic::onShutdown()
	{
		mSimulationJobProxy.unregister();
		mMessageProxyConnectedTransporterDestroyed.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SEKRobotLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// Check if the transporter is still exist
		qsf::Entity* connectedTransporter = getMap().getEntityById(mConnectedTransporterId);
		if (nullptr == connectedTransporter)
		{
			onTransporterStartsBurning(qsf::MessageParameters());
		}

		EntityHelper entityHelper(*getEntity());
		if (mIsIntact)
		{
			if (entityHelper.isBurning() || entityHelper.isBurned())
			{
				// Enable the transporter
				qsf::Entity* transporterEntity = getMap().getEntityById(mConnectedTransporterId);
				if (nullptr != transporterEntity)
				{
					transporterEntity->getOrCreateComponentSafe<qsf::ActionComponent>().clearPlan();
				}

				onTransporterStartsBurning(qsf::MessageParameters());
				mIsIntact = false;
			}
		}
	}

	void SEKRobotLogic::onTransporterStartsBurning(const qsf::MessageParameters& parameters)
	{
		if (nullptr != getEntity())
		{
			FireComponent* fireComponent = getEntity()->getComponent<FireComponent>();
			if (nullptr != fireComponent)
			{
				fireComponent->setActive(false);
			}

			qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();
			actionComponent.clearPlan();

			// To disappear the robot
			actionComponent.pushAction<DisappearAction>(action::BLOCKING);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

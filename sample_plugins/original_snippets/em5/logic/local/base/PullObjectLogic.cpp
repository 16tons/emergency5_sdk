// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/base/PullObjectLogic.h"
#include "em5/component/objects/BuryComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"

#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PullObjectLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::PullObjectLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PullObjectLogic::PullObjectLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	bool PullObjectLogic::isPullingObject() const
	{
		return mPulledEntity.valid();
	}

	qsf::Entity* PullObjectLogic::getPulledObject() const
	{
		return mPulledEntity.get();
	}

	void PullObjectLogic::setPulledObject(qsf::Entity* entity)
	{
		mPulledEntity = entity;

		// Register job update
		if (nullptr != entity)
		{
			mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&PullObjectLogic::updateSimulation, this, _1));

			// Register to catch if our unit person was injured while pulling something
			mMessageProxyInjured.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSINJURED_PERSON, getEntityId()), boost::bind(&PullObjectLogic::onPersonInjured, this, _1));
		}
		else
		{
			mSimulationJobProxy.unregister();
			mMessageProxyInjured.unregister();
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool PullObjectLogic::onStartup()
	{
		// Call base implementation
		return qsf::GameLogic::onStartup();
	}

	void PullObjectLogic::onShutdown()
	{
		setPulledObject(nullptr);

		// Call base implementation
		qsf::GameLogic::onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PullObjectLogic::updateSimulation(const qsf::JobArguments&)
	{
		if (!mPulledEntity.valid() || nullptr == getEntity())
		{
			mSimulationJobProxy.unregister();
			return;
		}

		BuryComponent* buryComponent = mPulledEntity->getComponent<BuryComponent>();
		if (nullptr != buryComponent)
		{
			// TODO(fw): Do this after a few seconds, or when the blocker was moved a certain distance

			buryComponent->unlinkAndShowBuriedEntities();
		}
	}

	void PullObjectLogic::onPersonInjured(const qsf::MessageParameters& parameters)
	{
		// Fireman gets injured while pulling something -> unlink them
		if (getEntity() != nullptr)
		{
			if (mPulledEntity.valid())
			{
				EntityHelper(mPulledEntity.getSafe()).unlinkFromParent();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

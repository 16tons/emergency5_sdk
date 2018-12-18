// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/event/BlockedForEventComponent.h"

#include <qsf/job/JobArguments.h>
#include <qsf/map/Entity.h>
#include <qsf/plugin/QsfJobs.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 BlockedForEventComponent::COMPONENT_ID = qsf::StringHash("em5::BlockedForEventComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BlockedForEventComponent::BlockedForEventComponent(qsf::Prototype* prototype) :
		Component(prototype),
		mBlockedTimer(qsf::Time::fromSeconds(5.f))
	{
		// Nothing here
	}

	qsf::Time BlockedForEventComponent::getBlockedTime() const
	{
		return mBlockedTimer;
	}

	void BlockedForEventComponent::setBlockedTime(qsf::Time blockedTime)
	{
		if (blockedTime < qsf::Time::ZERO)
			blockedTime = qsf::Time::ZERO;	// Clamp to zero

		mBlockedTimer = blockedTime;

		// Is job already active?
		if (!mUpdateJobProxy.isValid() && mBlockedTimer > qsf::Time::ZERO)
		{
			// Activate job by registering via the job proxy
			mUpdateJobProxy.registerAt(qsf::QsfJobs::SIMULATION_GENERAL, boost::bind(&BlockedForEventComponent::updateSimulation, this, _1));
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void BlockedForEventComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Fill function
	}

	void BlockedForEventComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			if (mBlockedTimer > qsf::Time::ZERO)
			{
				// Activate job by registering via the job proxy
				mUpdateJobProxy.registerAt(qsf::QsfJobs::SIMULATION_GENERAL, boost::bind(&BlockedForEventComponent::updateSimulation, this, _1));
			}
		}
	}

	void BlockedForEventComponent::onShutdown()
	{
		mUpdateJobProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BlockedForEventComponent::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		if (mBlockedTimer > qsf::Time::ZERO)
		{
			mBlockedTimer -= jobArguments.getTimePassed();
		}
		else
		{
			// Remove this component, it's not needed any more
			getEntity().destroyComponentById(COMPONENT_ID);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

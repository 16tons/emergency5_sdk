// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/person/InhabitantComponent.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/move/ArrivedAtDespawnAction.h"
#include "em5/plugin/Jobs.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/map/Map.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>


namespace em5
{
	const uint32 InhabitantComponent::COMPONENT_ID = qsf::StringHash("em5::InhabitantComponent");
	const uint32 InhabitantComponent::ACTION_PRIORITY = qsf::StringHash("ActionPriority");
	const uint32 InhabitantComponent::DESTINATION_ENTITY_ID = qsf::StringHash("DestinationEntityId");

	namespace
	{
		const float UPDATE_FREQUENCY_FOR_MODE[InhabitantComponent::UNDEFINED+1]={
			2.0f,	// GOTO_DESTINATION
			10.0f,	// GOTO_HOME
			100.0f, // UNDFINED
		};
	}

	void InhabitantComponent::serialize(qsf::BinarySerializer& serializer)
	{
		qsf::Component::serialize(serializer);
		serializer & mDestinationEntityId;
		serializer & mHomeEntityId;
		serializer & mActionPriority;
		serializer.serializeAs<uint8>(mInhabitantMode);
	}

	void InhabitantComponent::onSetSimulating(bool simulating)
	{
		if (simulating && isActive())
		{
			setUpdateFrequency(qsf::Time::fromSeconds(UPDATE_FREQUENCY_FOR_MODE[mInhabitantMode]));
		}
		else
		{
			mUpdateJobProxy.unregister();
		}
	}

	void InhabitantComponent::setUpdateFrequency(qsf::Time updateInterval)
	{
		qsf::jobs::JobConfiguration jobConfig;
		jobConfig.mTimeBetweenCalls = updateInterval;
		mUpdateJobProxy.registerAt(Jobs::SIMULATION_SPAWN_PERSONS, boost::bind(&InhabitantComponent::updateJob, this, _1), jobConfig);
	}

	void InhabitantComponent::setInhabitantMode(InhabitantMode mode)
	{
		if (mInhabitantMode != mode)
		{
			mInhabitantMode = mode;
			InhabitantComponent::onSetSimulating(isSimulating());
		}
	}

	bool InhabitantComponent::goToEntity(uint64 targetEntityId)
	{
		if (qsf::isInitialized(targetEntityId))
		{
			qsf::ActionComponent* actionComponent = getEntity().getOrCreateComponent<qsf::ActionComponent>();
			if (actionComponent && actionComponent->getCurrentPriority() < mActionPriority)
			{
				const qsf::Entity* target=getEntity().getMap().getEntityById(targetEntityId);
				if (target)
				{
					const qsf::TransformComponent* targetTransformComponent = target->getComponent<qsf::TransformComponent>();
					if (targetTransformComponent)
					{
						qsf::logic::TargetPoint moveTargetPoint;
						moveTargetPoint.mTolerance = 1.0f;	// Don't set the tolerance too low, the target point may be e.g. a bit in the ground
						moveTargetPoint.mPosition = targetTransformComponent->getPosition();
						actionComponent->pushAction<MoveAction>(mActionPriority).init(new qsf::ai::ReachSinglePointGoal(moveTargetPoint));
						actionComponent->pushAction<ArrivedAtDespawnAction>(mActionPriority).init(moveTargetPoint);
						return true;
					}
				}
			}
		}
		return false;
	}

	void InhabitantComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		switch (mInhabitantMode)
		{
			case GOTO_DESTINATION:
				{
					const EntityHelper entityHelper(getEntity());
					if (!(entityHelper.isPersonInjured()))
						if (goToEntity(mDestinationEntityId))
							break;
				}
				setInhabitantMode(GOTO_HOME);
				// TODO(co) There's no break. Is it by intent or was it forgotten?
			case GOTO_HOME:
				goToEntity(mHomeEntityId);
				break;
			default:
				break;
		}
	}

}

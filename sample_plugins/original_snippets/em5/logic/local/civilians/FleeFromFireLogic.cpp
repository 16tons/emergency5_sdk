// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/civilians/FleeFromFireLogic.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/ai/MovementModes.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Jobs.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/math/Random.h>
#include <qsf/job/JobArguments.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 FleeFromFireLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::FleeFromFireLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FleeFromFireLogic::FleeFromFireLogic() :
		GameLogic(GAMELOGIC_TYPE_ID),
		mAmountDamage(0.0f)
	{
		// Nothing here
	}

	void FleeFromFireLogic::receiveFireEnergyFromSource(qsf::Entity& sourceEntity, float amountDamage)
	{
		// TODO(mk) In case we have multiple fire sources, find an way (maybe with direction vectors) to combine them.
		 mSourceEntity = sourceEntity;
		 mAmountDamage += amountDamage;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool FleeFromFireLogic::onStartup()
	{
		qsf::Entity* entity = getEntity();
		if (nullptr == entity)
			return false;

		if (!EntityHelper(*entity).isCivilPerson())
			return false;

		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&FleeFromFireLogic::updateSimulation, this, _1), qsf::jobs::JobConfiguration());
		resetFleeTimer();

		return true;
	}

	void FleeFromFireLogic::onShutdown()
	{
		mSimulationJobProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void FleeFromFireLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// Real update only if the source entity is valid
		if (mSourceEntity.valid())
		{
			mTimeToFlee -= jobArguments.getTimePassed();
			if (mTimeToFlee <= qsf::Time::ZERO)
			{
				// Push flee move action
				qsf::Entity* ownerEntity = getEntity();
				qsf::ActionComponent* actionComponent = ownerEntity->getOrCreateComponent<qsf::ActionComponent>();
				if (nullptr != actionComponent)
				{
					EntityHelper ownerEntityHelper(*ownerEntity);
					if (!ownerEntityHelper.isCivilPersonInValidState())
					{
						removeThis();
						return;
					}

					// In case the entity is not fleeing from fire already or has something better to do:
					// Push a move action, away from the source
					if (actionComponent->getCurrentPriority() < action::DANGER_URGENT)
					{
						// Make sure to remove any pre-edited animation first
						ownerEntityHelper.disableMeshAnimationTestComponent();

						const glm::vec3 sourcePosition = EntityHelper(mSourceEntity.getSafe()).getPosition();
						const glm::vec3 ownerPosition = ownerEntityHelper.getPosition();
						const glm::vec3 distance = ownerPosition - sourcePosition;
						static const float metersToFleeFromSource = 10.f * qsf::Random::getRandomFloat(0.5f, 1.5f);
						const glm::vec3 positionToFlee = ownerPosition + glm::normalize(distance) * metersToFleeFromSource;

						// Push movement action to new point
						qsf::logic::TargetPoint moveTargetConfig;
						moveTargetConfig.mPosition = positionToFlee;

						// For this movement, also use the free area
						actionComponent->pushAction<MoveAction>(action::DANGER_URGENT, qsf::action::REPLACE_WHOLE_PLAN).init(new qsf::ai::ReachSinglePointGoal(moveTargetConfig), MovementModes::MOVEMENT_MODE_FREE_WALK);
						actionComponent->pushAction<qsf::WaitAction>(action::AUTOMATISM_STD, qsf::action::APPEND_TO_BACK).init(qsf::Time::fromSeconds(10.0f));

						// Reset the timer
						resetFleeTimer();
					}
				}
			}
		}
		else
		{
			mTimeWithoutFireDamage -= jobArguments.getTimePassed();
			if (mTimeWithoutFireDamage <= qsf::Time::ZERO)
			{
				// Long time without fire impact, Remove logic
				removeThis();
				return;
			}
		}

		// Reset incoming values
		mSourceEntity = nullptr;
		mAmountDamage = 0.0f;
	}

	void FleeFromFireLogic::resetFleeTimer()
	{
		mTimeToFlee = qsf::Time::fromSeconds(qsf::Random::getRandomFloat(0.5f, 1.0f));
		mTimeWithoutFireDamage = qsf::Time::fromSeconds(5.0f);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

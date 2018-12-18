// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/firefighters/DiverLogic.h"
#include "em5/action/move/MoveAction.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/person/PersonPlacementComponent.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"
#include "em5/game/targetpoint/PullPersonTargetPointProvider.h"
#include "em5/map/EntityHelper.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/job/JobArguments.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DiverLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::DiverLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DiverLogic::DiverLogic() :
		CarryPersonLogic(GAMELOGIC_TYPE_ID),
		mIsSwimming(false),
		mSwimmingOffset(0.0f),
		mWalkSpeed(1.0f),
		mSwimSpeed(2.5f)
	{
		// Nothing here
	}

	bool DiverLogic::isSwimming() const
	{
		return mIsSwimming;
	}

	bool DiverLogic::isCarryingPerson() const
	{
		return qsf::isInitialized(getCarryPerson());
	}

	float DiverLogic::getSwimmingOffset() const
	{
		return mSwimmingOffset;
	}

	void DiverLogic::setWalkSpeed(qsf::UnsignedFloat speed)
	{
		mWalkSpeed = speed;

		if (isRunning())
		{
			updateMovementSpeed();
		}
	}

	void DiverLogic::setSwimSpeed(qsf::UnsignedFloat speed)
	{
		mSwimSpeed = speed;

		if (isRunning())
		{
			updateMovementSpeed();
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool DiverLogic::onStartup()
	{
		// Sanity check: is there an entity?
		qsf::Entity* entity = getEntity();
		QSF_CHECK(nullptr != entity, "Diver logic has no entity", return false);

		// Register job
		//  -> Update is needed in each tick
		qsf::jobs::JobConfiguration jobConfiguration;
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&DiverLogic::updateSimulation, this, _1), jobConfiguration);

		// Get walk and swim speed
		qsf::ai::NavigationComponent* aiNavigationComponent = entity->getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != aiNavigationComponent)
		{
			mWalkSpeed = aiNavigationComponent->getNativeMaxForwardSpeed();
		}
		mSwimSpeed = mWalkSpeed * qsf::UnsignedFloat(LogicSpecsGroup::getInstanceSafe().getDiverInWaterSpeedFactor());

		return true;
	}

	void DiverLogic::onShutdown()
	{
		// Unregister job and message proxies
		mSimulationJobProxy.unregister();

		// Call base implementation
		CarryPersonLogic::onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DiverLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// Sanity check: is there an entity?
		qsf::Entity* entity = getEntity();
		QSF_CHECK(nullptr != entity, "Diver logic has no entity", return);

		// Check if diver must be swimming right now
		//  -> In case he is carrying someone, switch to walking not before only 30cm water depth
		const glm::vec3& position = entity->getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
		const bool mustBeSwimming = isCarryingPerson() ? PersonPlacementComponent::mustBeSwimming(position, 0.3f) : PersonPlacementComponent::mustBeSwimming(position);

		if (mustBeSwimming != mIsSwimming)
		{
			mIsSwimming = mustBeSwimming;

			// Diver has to be faster in water
			qsf::ActionComponent* actionComponent = entity->getComponent<qsf::ActionComponent>();
			if (nullptr != actionComponent)
			{
				// TODO(fw): Why change this only when already moving...?
				qsf::Action* currentAction = actionComponent->getCurrentAction();
				if (nullptr != currentAction && currentAction->getTypeId() == MoveAction::ACTION_ID)
				{
					updateMovementSpeed();
				}
			}

			// Immediately update the animation
			PersonComponent* personComponent = entity->getComponent<PersonComponent>();
			if (nullptr != personComponent)
			{
				personComponent->forceAnimationUpdate();
			}

			if (!mIsSwimming)
			{
				// In the moment both persons are on the same position. The carried person need an offset from the diver to ensure a good optical look for the animation
				giveCarryOffsetToCarriedPerson();
			}
		}
		else
		{
			static const float INTERPOLATION_SECONDS = 0.4f;
			if (mIsSwimming)
			{
				if (mSwimmingOffset < 1.0f)
				{
					mSwimmingOffset = std::min(mSwimmingOffset + jobArguments.getSecondsPassed() / INTERPOLATION_SECONDS, 1.0f);

					// Force placement update anyway (especially important if not moving!)
					EntityHelper(*entity).applyPlacement();
				}
			}
			else
			{
				if (mSwimmingOffset > 0.0f)
				{
					mSwimmingOffset = std::max(mSwimmingOffset - jobArguments.getSecondsPassed() / INTERPOLATION_SECONDS, 0.0f);

					// Force placement update anyway (especially important if not moving!)
					EntityHelper(*entity).applyPlacement();
				}
			}
		}
	}

	void DiverLogic::giveCarryOffsetToCarriedPerson()
	{
		qsf::Entity* carriedPerson = getEntity()->getMap().getEntityById(getCarryPerson());
		if (nullptr != carriedPerson)
		{
			// Give the carried person the needed offset
			qsf::TransformComponent& callerTransformComponent = getEntity()->getOrCreateComponentSafe<qsf::TransformComponent>();
			glm::vec3 position = callerTransformComponent.getPosition();
			const glm::vec3 offsetLocal(PullPersonTargetPointProvider::getOffset());
			const glm::vec3 offsetWorld = callerTransformComponent.getTransform().vec3DirectionLocalToWorld(offsetLocal);
			position -= offsetWorld;
			qsf::TransformComponent& carriedTransformComponent = carriedPerson->getOrCreateComponentSafe<qsf::TransformComponent>();
			carriedTransformComponent.setPositionAndRotation(position, callerTransformComponent.getRotation());
		}
	}

	void DiverLogic::updateMovementSpeed()
	{
		qsf::ai::NavigationComponent* aiNavigationComponent = getEntitySafe().getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != aiNavigationComponent)
		{
			const float newSpeed = mIsSwimming ? *mSwimSpeed : *mWalkSpeed;
			aiNavigationComponent->setNativeMaxForwardSpeed(newSpeed);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/HelicopterFollowAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/vehicle/parts/HelicopterSearchLightComponent.h"
#include "em5/logic/local/civilians/FollowedByHelicopterLogic.h"
#include "em5/logic/local/gangsters/GangsterCivilUnarmedLogic.h"
#include "em5/logic/local/gangsters/GangsterCarHijackerLogic.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Math.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier HelicopterFollowAction::ACTION_ID = "em5::HelicopterFollowAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HelicopterFollowAction::HelicopterFollowAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mTargetEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	HelicopterFollowAction::~HelicopterFollowAction()
	{
		// Nothing here
	}

	void HelicopterFollowAction::init(uint64 targetEntityId)
	{
		mTargetEntity = getEntity().getMap().getEntityById(targetEntityId);
	}

	uint64 HelicopterFollowAction::getTargetEntityId() const
	{
		return mTargetEntity.valid() ? mTargetEntity->getId() : qsf::getUninitialized<uint64>();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	bool HelicopterFollowAction::onStartup()
	{
		// TODO(db) Insert distance check
		// if distance to high -> return false

		if (!checkTarget())
			return false;

		startSearchLightLogic();

		return true;
	}

	void HelicopterFollowAction::onShutdown()
	{
		stopSearchLightLogic();

		if (!mTargetEntity.valid())
			return;

		{
			qsf::GameLogicComponent& gameLogicComponent = mTargetEntity->getOrCreateComponentSafe<qsf::GameLogicComponent>();
			FollowedByHelicopterLogic* followedByHelicopterLogic = gameLogicComponent.getGameLogic<FollowedByHelicopterLogic>();
			if (followedByHelicopterLogic != nullptr)
			{
				gameLogicComponent.deleteGameLogic(*followedByHelicopterLogic);
			}
		}

		if (EntityHelper(*mTargetEntity).isGangsterRoadVehicle())
		{
			qsf::Entity* gangster = nullptr;
			for (auto& iterator : mTargetEntity->getComponent<RoadVehicleComponent>()->getLinks())
			{
				const qsf::ContainerLink& containerLink = *iterator.second;
				qsf::Entity& linkedEntity = containerLink.getTargetEntity();

				if (EntityHelper(linkedEntity).isGangsterPerson())
				{
					gangster = &linkedEntity;
					break;
				}
			}
			if (nullptr != gangster)
			{
				GangsterCarHijackerLogic* gameLogic = gangster->getOrCreateComponentSafe<qsf::GameLogicComponent>().getGameLogic<GangsterCarHijackerLogic>();
				if (nullptr != gameLogic)
				{
					gameLogic->abortCarChase();
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result HelicopterFollowAction::updateAction(const qsf::Clock& clock)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				EntityHelper entityHelper(*mTargetEntity);

				// Gangster
				if (entityHelper.isGangsterPerson())
				{
					qsf::GameLogicComponent& gameLogicComponent = mTargetEntity->getOrCreateComponentSafe<qsf::GameLogicComponent>();
					GangsterBaseLogic* gameLogic = gameLogicComponent.getGameLogic<GangsterBaseLogic>();

					// Add a gangster logic if gangster hasn't already (usually gangsters should have a gangster logic)
					if (nullptr == gameLogic)
					{
						gameLogic = &gameLogicComponent.getOrCreateGameLogicSafe<GangsterCivilUnarmedLogic>();
					}
					gameLogic->escape(GangsterBaseLogic::ESCAPE_FOOT_LONG);
				}

				// Gangster vehicle
				else if (entityHelper.isGangsterRoadVehicle())
				{
					qsf::Entity* gangster = nullptr;
					for (auto& iterator : mTargetEntity->getComponent<RoadVehicleComponent>()->getLinks())
					{
						const qsf::ContainerLink& containerLink = *iterator.second;
						qsf::Entity& linkedEntity = containerLink.getTargetEntity();

						if (EntityHelper(linkedEntity).isGangsterPerson())
						{
							gangster = &linkedEntity;
							break;
						}
					}
					if (nullptr != gangster)
					{
						GangsterCarHijackerLogic* gameLogic = EntityHelper(gangster).getGameLogic<GangsterCarHijackerLogic>();
						if (nullptr != gameLogic)
						{
							gameLogic->startCarChase();
						}
					}
				}

				// Civilist
				else
				{
					qsf::GameLogicComponent& gameLogicComponent = mTargetEntity->getOrCreateComponentSafe<qsf::GameLogicComponent>();
					FollowedByHelicopterLogic& followedByHelicopterLogic = gameLogicComponent.getOrCreateGameLogicSafe<FollowedByHelicopterLogic>();
					followedByHelicopterLogic.setFollowingHelicopter(getEntityId());
					followedByHelicopterLogic.startup();
				}

				mState = STATE_FOLLOWING;
				// Fall through by design
			}

			case STATE_FOLLOWING:
			{
				// If the target isn't valid anymore cancel the following
				if (!checkTarget())
					return qsf::action::RESULT_DONE;

				// Calculate position and rotation of the helicopter
				{
					qsf::Entity* targetEntity = mTargetEntity.get();

					qsf::TransformComponent& targetTransformComponent = targetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
					qsf::TransformComponent& heliTransformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
					qsf::MovableComponent& heliMovableComponent = getEntity().getOrCreateComponentSafe<qsf::MovableComponent>();

					glm::vec3 heliPosition = heliTransformComponent.getPosition();
					glm::vec3 targetPosition = targetTransformComponent.getPosition();

					// Eliminate high level
					targetPosition.y = heliPosition.y;

					glm::quat lookToTargetRotation = qsf::Math::getLookAtQuaternion(heliPosition, targetPosition, qsf::CoordinateSystem::getUp());

					// Get the following distance from the specs file
					const RoadVehicleSpecsGroup& roadVehicleSpecsGroup = RoadVehicleSpecsGroup::getInstanceSafe();

					if (qsf::game::DistanceHelper::get2dDistance(targetPosition, heliPosition) > roadVehicleSpecsGroup.getHelicopterFollowDistance())
					{
						// Move caller closer to target
						glm::vec3 newPosition = (lookToTargetRotation * glm::vec3(0, 0, roadVehicleSpecsGroup.getHelicopterFollowDistance())) + targetPosition;
						//newPosition = glm::lerp(newPosition, heliPosition, 0.1f * clock.getTimePassed().getSeconds());	// TODO(co) Why is this commented?

						// Turn to target
						glm::quat newRotation = glm::lerp(lookToTargetRotation, heliTransformComponent.getRotation(), 0.1f * clock.getTimePassed().getSeconds());

						heliMovableComponent.moveToPositionAndRotation(newPosition, newRotation);
					}
					else
					{
						// Turn to target
						glm::quat newRotation = glm::lerp(lookToTargetRotation, heliTransformComponent.getRotation(), 0.1f * clock.getTimePassed().getSeconds());
						heliMovableComponent.moveToRotation(newRotation);
					}
				}

				return qsf::action::RESULT_CONTINUE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	bool HelicopterFollowAction::checkTarget()
	{
		// Check the target still valid
		if (!mTargetEntity.valid())
			return false;

		EntityHelper entityHelper(*mTargetEntity.get());

		// Has the target left the map
		if (!entityHelper.isInMapBoundaries())
			return false;

		// TODO(db): This check gives always true, so every vehicle is a gangster road vehicle, enable this when the bug is fixed
		// Target is gangster vehicle and gangster leaves vehicle
		//if (entityHelper.isGangsterRoadVehicle() && entityHelper.isContainerEmpty())
		//	return false;

		// Target is gangster and arrested
		if (entityHelper.isArrested())
			return false;

		return true;
	}

	void HelicopterFollowAction::startSearchLightLogic()
	{
		// Get the HelicopterSearchLightComponents
		std::vector<HelicopterSearchLightComponent*> helicopterSearchLightComponents;
		EntityHelper(getEntity()).getComponentsFromChildren(helicopterSearchLightComponents);

		if (!helicopterSearchLightComponents.empty())
		{
			for (HelicopterSearchLightComponent* helicopterSearchLightComponent : helicopterSearchLightComponents)
			{
				helicopterSearchLightComponent->setTargetEntityId(mTargetEntity->getId());
				helicopterSearchLightComponent->startFollowTarget();
			}
		}

		{ // Disable the AI navigation component while following, else the helicopter will jitter around
			qsf::ai::NavigationComponent* navigationComponent = getEntity().getComponent<qsf::ai::NavigationComponent>();
			if (nullptr != navigationComponent)
			{
				navigationComponent->setActive(false);
			}
		}
	}

	void HelicopterFollowAction::stopSearchLightLogic()
	{
		{ // Reenable the AI navigation component
			qsf::ai::NavigationComponent* navigationComponent = getEntity().getComponent<qsf::ai::NavigationComponent>();
			if (nullptr != navigationComponent)
			{
				navigationComponent->setActive(true);
			}
		}

		// Get the HelicopterSearchLightComponents
		std::vector<HelicopterSearchLightComponent*> helicopterSearchLightComponents;
		EntityHelper(getEntity()).getComponentsFromChildren(helicopterSearchLightComponents);

		if (!helicopterSearchLightComponents.empty())
		{
			for (HelicopterSearchLightComponent* helicopterSearchLightComponent : helicopterSearchLightComponents)
			{
				// Setting the target uninitialized lets the light move back in the original position and deactivates the update job if finished
				helicopterSearchLightComponent->setTargetEntityId(qsf::getUninitialized<uint64>());
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/StartHelicopterAction.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/vehicle/HelicopterPlacementComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"
#include "em5/map/EntityHelper.h"

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/move/MovableComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier StartHelicopterAction::ACTION_ID = "em5::StartHelicopterAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StartHelicopterAction::StartHelicopterAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mOptimalFlyHeight(0.0f),
		mGroundHeight(0.0f),
		mForceStart(false)
	{
		// Nothing here
	}

	StartHelicopterAction::~StartHelicopterAction()
	{
		// Nothing here
	}

	void StartHelicopterAction::init(bool forceStart)
	{
		mForceStart = forceStart;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void StartHelicopterAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialization
	}

	bool StartHelicopterAction::onStartup()
	{
		// Can only be done by helicopter
		HelicopterComponent* helicopterComponent = getEntity().getComponent<HelicopterComponent>();
		if (nullptr == helicopterComponent)
			return false;

		// Check if helicopter is already in air
		{
			const glm::vec3& position = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();

			// GroundMapQuery fills mGroundHeight with the height of the ground map
			qsf::GroundMapQuery(getMap(), GroundMaps::FILTER_HELICOPTER_GROUND).getHeightAt(position, mGroundHeight);
			const RoadVehicleSpecsGroup& roadVehicleSpecsGroup = RoadVehicleSpecsGroup::getInstanceSafe();
			const float flyHeightMultiplier = (EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE) ? 1 : roadVehicleSpecsGroup.getHelicopterStartRaiseTillFlyHeightPercent());	// The fire drone must fly to the hightes possible point
			mOptimalFlyHeight = mGroundHeight + (roadVehicleSpecsGroup.getHelicopterFlyHeight() * flyHeightMultiplier);

			if (position.y > mOptimalFlyHeight)
			{
				// Helicopter is already in air
				return false;
			}
		}

		EntityHelper entityHelper(getEntity());
		if (entityHelper.isBurned() || entityHelper.isBurning())
			return false;

		return true;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result StartHelicopterAction::updateAction(const qsf::Clock& clock)
	{
		HelicopterComponent& helicopterComponent = getEntity().getOrCreateComponentSafe<HelicopterComponent>();

		switch (mState)
		{
			case STATE_INIT:
			{
				// Detect the right time to start rotor/dust effect
				if (helicopterComponent.isFlying() == false)
				{
					if (mForceStart)
					{
						helicopterComponent.forceFlying(mForceStart);
						if (!EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE))
						{
							helicopterComponent.stopSwing();
						}
					}
					helicopterComponent.setFlying(true);
					helicopterComponent.startDustEffects();
					helicopterComponent.setIsStartingOrLanding(true);
					// Its not possible to damage the helicopter in air
					helicopterComponent.setInvincible(true);
				}
				mState = STATE_START_ROTOR;
				// Fall through by intent
			}

			case STATE_START_ROTOR:
			{
				if (!helicopterComponent.isRotorAtFullSpeed())
				{
					// Wait till rotor is at full speed
					return qsf::action::RESULT_CONTINUE;
				}
				mState = STATE_STARTING;
				// Fall through by intent
			}

			case STATE_STARTING:
			{
				// Raise the helicopter
				qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
				glm::vec3 position = transformComponent.getPosition();

				const float heightDifference = position.y - mGroundHeight;	// Meters in above ground

				const RoadVehicleSpecsGroup& roadVehicleSpecsGroup = RoadVehicleSpecsGroup::getInstanceSafe();
				float flyHeightChangeSpeed = roadVehicleSpecsGroup.getHelicopterFlyHeightSpeed() * clock.getTimePassed().getSeconds();
				if (heightDifference < roadVehicleSpecsGroup.getHelicopterStartLandingHeightLower())
				{
					flyHeightChangeSpeed *= 0.4f;
				}
				else if (heightDifference < roadVehicleSpecsGroup.getHelicopterStartLandingHeightUpper())
				{
					flyHeightChangeSpeed *= 0.7f;
				}

				// Wait till the helicopter reaches the optimal fly height level
				position.y += flyHeightChangeSpeed;
				transformComponent.setPosition(position);
				if (position.y < mOptimalFlyHeight)
				{
					return qsf::action::RESULT_CONTINUE;
				}

				helicopterComponent.stopDustEffects();
				helicopterComponent.setCollisionShapeAir();
				helicopterComponent.setIsStartingOrLanding(false);

				if (!EntityHelper(getEntity()).isUnit(CommandableComponent::UNITTAG_FIREFIGHTERS_DRONE))
				{
					// Start swing effect
					helicopterComponent.startSwing();
				}
				else
				{
					helicopterComponent.stopSwing();
				}

				// Tell helicopter placement to take the currently set height instead of its own
				HelicopterPlacementComponent* helicopterPlacementComponent = getEntity().getComponent<HelicopterPlacementComponent>();
				if (nullptr != helicopterPlacementComponent)
				{
					helicopterPlacementComponent->dirtyCurrentHeight();
				}

				// End
				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

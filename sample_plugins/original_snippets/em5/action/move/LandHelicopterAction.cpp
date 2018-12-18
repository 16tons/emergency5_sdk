// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/LandHelicopterAction.h"
#include "em5/action/move/StartHelicopterAction.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/vehicle/HelicopterPlacementComponent.h"
#include "em5/component/vehicle/parts/HelicopterHatchComponent.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/specs/RoadVehicleSpecsGroup.h"
#include "em5/map/CollisionHelper.h"

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/component/base/TransformComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier LandHelicopterAction::ACTION_ID = "em5::LandHelicopterAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	LandHelicopterAction::LandHelicopterAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mCanStartDustEffect(true),
		mGroundHeight(0.0f),
		mOnlyHovering(false)
	{
		// Nothing here
	}

	LandHelicopterAction::~LandHelicopterAction()
	{
		// Nothing here
	}

	void LandHelicopterAction::init(bool onlyHovering)
	{
		mOnlyHovering = onlyHovering;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void LandHelicopterAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialization
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	bool LandHelicopterAction::onStartup()
	{
		// Can only done by helicopter
		HelicopterComponent* helicopterComponent = getEntity().getComponent<HelicopterComponent>();
		if (nullptr == helicopterComponent)
			return false;

		// Check if helicopter is in air
		const glm::vec3& position = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();

		// GroundMapQuery fills mGroundHeight with the height of the ground map
		qsf::GroundMapQuery(getMap(), GroundMaps::FILTER_DEFAULT).getHeightAt(position, mGroundHeight);

		// Is the helicopter already landed?
		if (mGroundHeight == position.y && !helicopterComponent->isFlying())
		{
			return false;
		}

		{ // If the helicopter has an hatch (BHC), check if it used
			const qsf::Entity* specialEntity = getMap().getEntityById(helicopterComponent->getSpecialEntity());
			if (nullptr != specialEntity)
			{
				// An injured person blockade all other transports of the helicopter
				HelicopterHatchComponent* helicopterHatchComponent = specialEntity->getComponent<HelicopterHatchComponent>();
				if (nullptr != helicopterHatchComponent && helicopterHatchComponent->isTransportingInjuredPerson())
				{
					return false;
				}
			}
		}

		mGroundPosition = position;
		mGroundPosition.y = mGroundHeight;

		if (!mOnlyHovering)
		{
			// Don't land in water
			CollisionHelper collisionHelper(getMap());
			if (collisionHelper.isInWater(mGroundPosition))
				return false;

			// Don't land in special "no helicopter landing" blockers
			// TODO(sw) Hack. Ignore collisions when the helicopter is near the ground (aka already landed). This is needed because the helicoper has a child object with a active collision
			if (glm::abs(mGroundHeight - position.y) > 0.1f && collisionHelper.hasCollisionEM3Helilanding(getEntity(), mGroundPosition, 0.0f))
				return false;
		}

		// Tell helicopter placement to take the currently set height next time, instead of its own
		HelicopterPlacementComponent* helicopterPlacementComponent = getEntity().getComponent<HelicopterPlacementComponent>();
		if (nullptr != helicopterPlacementComponent)
		{
			helicopterPlacementComponent->dirtyCurrentHeight();
		}

		return true;
	}

	void LandHelicopterAction::onShutdown()
	{
		// Ensure the dust effect is really gone, also when e.g. aborting the action
		HelicopterComponent* helicopterComponent = getEntity().getComponent<HelicopterComponent>();
		if (nullptr != helicopterComponent)
		{
			helicopterComponent->stopDustEffects();
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result LandHelicopterAction::updateAction(const qsf::Clock& clock)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				HelicopterComponent* helicopterComponent = getEntity().getComponent<HelicopterComponent>();
				if (nullptr != helicopterComponent)
					helicopterComponent->setCollisionShapeGround();

				mState = STATE_CHECK_COLLISION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CHECK_COLLISION:
			{
				HelicopterComponent* helicopterComponent = getEntity().getComponent<HelicopterComponent>();
				if (nullptr != helicopterComponent)
				{
					// check for collisions
					CollisionHelper collisionHelper(getMap());
					if (collisionHelper.hasCollisionEM3(getEntity(), mGroundPosition, 0.05f) && helicopterComponent->isRotorAtFullSpeed())
					{
						helicopterComponent->setCollisionShapeAir();
						getComponent().pushAction<StartHelicopterAction>();
						return qsf::action::RESULT_DONE;
					}
				}
				mState = STATE_LANDING;
				//return qsf::action::RESULT_CONTINUE;
			}

			case STATE_LANDING:
			{
				// Lower the helicopter
				qsf::TransformComponent& transformComponent = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
				glm::vec3 position = transformComponent.getPosition();

				const RoadVehicleSpecsGroup& roadVehicleSpecsGroup = RoadVehicleSpecsGroup::getInstanceSafe();
				float flyHeightChangeSpeed = roadVehicleSpecsGroup.getHelicopterFlyHeightSpeed() * clock.getTimePassed().getSeconds();

				HelicopterComponent& helicopterComponent = getEntity().getOrCreateComponentSafe<HelicopterComponent>();
				helicopterComponent.setIsStartingOrLanding(true);

				const float heightDifference = position.y - mGroundHeight;
				if (heightDifference < roadVehicleSpecsGroup.getHelicopterStartLandingHeightLower())
				{
					flyHeightChangeSpeed *= 0.4f;
				}
				else if (heightDifference < roadVehicleSpecsGroup.getHelicopterStartLandingHeightUpper())
				{
					// Ask helicopter if dust effect can started again (we need an timer for the dust effect)
					// Start dust effect
					if (mCanStartDustEffect)
					{
						mCanStartDustEffect = false;

						helicopterComponent.startDustEffects();
					}

					flyHeightChangeSpeed *= 0.7f;

					if (mOnlyHovering)
					{
						// Valid abort here
						return qsf::action::RESULT_DONE;
					}
				}

				// Set the helicopter position
				position.y -= flyHeightChangeSpeed;
				if (position.y > mGroundHeight)
				{
					// Wait till the helicopter reaches the ground level
					transformComponent.setPosition(position);
					return qsf::action::RESULT_CONTINUE;
				}

				// Set helicopter on ground
				position.y = mGroundHeight;
				transformComponent.setPosition(position);

				helicopterComponent.setFlying(false);

				// End swing effect
				helicopterComponent.stopSwing();

				mState = STATE_LANDED_STOP_ROTOR;
				// Fall through by intent
			}

			case STATE_LANDED_STOP_ROTOR:
			{
				HelicopterComponent& helicopterComponent = getEntity().getOrCreateComponentSafe<HelicopterComponent>();
				if (!helicopterComponent.isRotorStopped())
				{
					// Wait till the rotor has stopped
					return qsf::action::RESULT_CONTINUE;
				}
				helicopterComponent.stopDustEffects();
				helicopterComponent.setIsStartingOrLanding(false);

				// No its possible to damage the helicopter
				helicopterComponent.setInvincible(false);

				// END
				break;
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

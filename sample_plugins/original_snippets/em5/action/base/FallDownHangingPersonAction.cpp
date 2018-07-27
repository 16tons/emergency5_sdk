// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/FallDownHangingPersonAction.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/objects/CallForRescueComponent.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf/map/query/GroundMapQuery.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier FallDownHangingPersonAction::ACTION_ID = "em5::FallDownHangingPersonAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	FallDownHangingPersonAction::FallDownHangingPersonAction() :
		Action(ACTION_ID),
		mState(STATE_INIT),
		mIsDead(false)
	{
		// Nothing here
	}

	FallDownHangingPersonAction::~FallDownHangingPersonAction()
	{
		// Nothing here
	}

	void FallDownHangingPersonAction::init(bool isDead)
	{
		mIsDead = isDead;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void FallDownHangingPersonAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) add serialization
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result FallDownHangingPersonAction::updateAction(const qsf::Clock&)
	{
		switch (mState)
		{
			case STATE_INIT:
			{
				getEntity().getOrCreateComponentSafe<FadeEffectComponent>().fadeOut();

				getEntity().destroyComponent<CallForRescueComponent>();

				mState = STATE_FALLINGDOWN;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_FALLINGDOWN:
			{
				if (getEntity().getOrCreateComponentSafe<FadeEffectComponent>().isFading())
				{
					// Wait till the animation is finished
					return qsf::action::RESULT_CONTINUE;
				}

				EntityHelper entityHelper(getEntity());
				float groundHeight = 0.0f;
				glm::vec3 position = entityHelper.getPosition();
				qsf::GroundMapQuery(getMap(), GroundMaps::FILTER_MASK_ALL).getHeightAt(position, groundHeight);

				position.y = groundHeight;

				// Set position to the ground
				entityHelper.setPosition(position);

				if (mIsDead)
				{
					HealthComponent* healthComponent = getEntity().getComponent<HealthComponent>();
					if (nullptr != healthComponent)
					{
						healthComponent->killPerson(entityHelper.getFreeplayEvent());
					}
				}

				getEntity().getOrCreateComponentSafe<FadeEffectComponent>().fadeIn();
			}
		}

		// Done
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

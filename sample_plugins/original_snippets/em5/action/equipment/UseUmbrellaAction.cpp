// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/equipment/UseUmbrellaAction.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_compositing/component/TintableMeshComponent.h>

#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier UseUmbrellaAction::ACTION_ID = "em5::UseUmbrellaAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	UseUmbrellaAction::UseUmbrellaAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT),
		mCloseUmbrella(false)
	{
		// Nothing here
	}

	UseUmbrellaAction::~UseUmbrellaAction()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void UseUmbrellaAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) serialize this class
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result UseUmbrellaAction::updateAction(const qsf::Clock&)
	{
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// If it has already an open umbrella equipped, close the umbrella
				mCloseUmbrella = EntityHelper(getEntity()).checkIsActiveEquipment(equipment::UMBRELLA);
				AnimationHelper(getEntity()).clearAnimation();

				// Get the equipment
				if (!mCloseUmbrella)
				{
					getComponent().pushAction<GetEquipmentAction>(action::AUTOMATISM_STD, qsf::action::INSERT_BEFORE_CURRENT).init(equipment::UMBRELLA_CLOSED);
				}

				mOpeningTime = qsf::Time::ZERO;

				mCurrentState = STATE_START_USING_ANIMATION;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_START_USING_ANIMATION:
			{
				AnimationHelper animationHelper(getEntity());
				mAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationUmbrellaOpen(), false, mCloseUmbrella, qsf::Time::fromSeconds(0.0f));

				mCurrentState = STATE_WAIT_FOR_OPEN;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_WAIT_FOR_OPEN:
			{
				if (mAnimationChannel.valid() && !mAnimationChannel->isFinished())
				{
					static const float FADEIN_EQUIPMENT_PERCENT_OPEN = 0.30f;
					static const float FADEIN_EQUIPMENT_PERCENT_CLOSE = 0.5f;
					if ((mAnimationChannel->getReverse() && mAnimationChannel->getOffset() >= FADEIN_EQUIPMENT_PERCENT_CLOSE) ||
						(!mAnimationChannel->getReverse() && mAnimationChannel->getOffset() <= FADEIN_EQUIPMENT_PERCENT_OPEN))
					{
						// Wait till animation is finished
						return qsf::action::RESULT_CONTINUE;
					}
				}

				int tintColorPalette = -1;	// -1 is default for random
				qsf::Entity* equipmentEntity = EntityHelper(getEntity()).getActiveEquipmentEntity();
				if (nullptr != equipmentEntity)
				{
					qsf::compositing::TintableMeshComponent* tintableMeshComponent = equipmentEntity->getComponent<qsf::compositing::TintableMeshComponent>();
					if (nullptr != tintableMeshComponent)
					{
						// Get the tint color from the current equipment
						tintColorPalette = tintableMeshComponent->getTintPaletteIndex();
					}
				}

				// Get the final equipment
				const std::string& equipmentName = (mCloseUmbrella ? equipment::UMBRELLA_CLOSED : equipment::UMBRELLA);
				getComponent().pushAction<GetEquipmentAction>(action::AUTOMATISM_STD, qsf::action::INSERT_BEFORE_CURRENT).init(equipmentName, tintColorPalette);

				mCurrentState = STATE_WAIT_FOR_FINISH;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_WAIT_FOR_FINISH:
			{
				if (mAnimationChannel.valid() && !mAnimationChannel->isFinished())
				{
					// Wait till animation is finished
					return qsf::action::RESULT_CONTINUE;
				}

				if (mCloseUmbrella)
				{
					// Remove the umbrella
					getComponent().pushAction<GetEquipmentAction>(action::AUTOMATISM_STD, qsf::action::INSERT_BEFORE_CURRENT).init("");
				}

				// Remove the shaking of the umbrella after the person is start moving
				AnimationHelper(getEntity()).clearAnimation();

				// Fall through by design
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

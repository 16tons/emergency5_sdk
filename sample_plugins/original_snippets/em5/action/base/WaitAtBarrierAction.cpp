// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/WaitAtBarrierAction.h"
#include "em5/game/animation/AnimationHelper.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier WaitAtBarrierAction::ACTION_ID = "em5::WaitAtBarrierAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	WaitAtBarrierAction::WaitAtBarrierAction() :
		Action(ACTION_ID),
		mInitiatorID(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	WaitAtBarrierAction::~WaitAtBarrierAction()
	{
		// Nothing here
	}

	void WaitAtBarrierAction::init(uint64 initiatorId)
	{
		mInitiatorID = initiatorId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void WaitAtBarrierAction::serialize(qsf::BinarySerializer& serializer)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool WaitAtBarrierAction::onStartup()
	{
		// Set animation to idle (otherwise people might continue with their walk animation for some reason)
		AnimationHelper animationHelper(getEntity());
		AnimationHelper::AnimationMaskData animationMaskData;
		animationHelper.playAnimation(animationHelper.getIdleAnimation(animationMaskData));

		return true;
	}

	qsf::action::Result WaitAtBarrierAction::updateAction(const qsf::Clock&)
	{
		// Keep this action active as long as the initiator who stopped this entity has the GuardBarrierAction.
		const qsf::Entity* initiatorEntity = getEntity().getMap().getEntityById(mInitiatorID);
		if (nullptr != initiatorEntity)
		{
			const qsf::ActionComponent* actionComponent = initiatorEntity->getComponent<qsf::ActionComponent>();
			if (nullptr != actionComponent)
			{
				//TODO(tl): We need to design this action new when we use it again
				//const qsf::Action* currentAction = actionComponent->getActionByTypeId(GuardBarrierAction::ACTION_ID);
				//if (nullptr != currentAction)
				{
					return qsf::action::RESULT_CONTINUE;
				}
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

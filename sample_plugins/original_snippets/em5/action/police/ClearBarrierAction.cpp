// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/police/ClearBarrierAction.h"
#include "em5/component/objects/BarrierTapeComponent.h"
#include "em5/map/MapHelper.h"

#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ClearBarrierAction::ACTION_ID = "em5::ClearBarrierAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ClearBarrierAction::ClearBarrierAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing here
	}

	ClearBarrierAction::~ClearBarrierAction()
	{
		// Nothing here
	}

	void ClearBarrierAction::init(qsf::Entity& targetEntity, const qsf::Time& disappearTime)
	{
		mTargetEntityId = targetEntity.getId();
		mDisappearTime = disappearTime;
	}

	uint64 ClearBarrierAction::getTargetEntityId() const
	{
		return mTargetEntityId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ClearBarrierAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mDisappearTime);
		serializer.serialize(mTimePassed);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool ClearBarrierAction::onStartup()
	{
		return true;
	}

	qsf::action::Result ClearBarrierAction::updateAction(const qsf::Clock& clock)
	{
		qsf::Entity* targetEntity = getEntity().getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return qsf::action::RESULT_DONE;

		BarrierTapeComponent* barrierTapeComponent = targetEntity->getComponent<BarrierTapeComponent>();
		if (nullptr == barrierTapeComponent)
			return qsf::action::RESULT_DONE;

		mTimePassed += clock.getTimePassed();
		if (mTimePassed < mDisappearTime)
		{
			float transparency = mTimePassed.getSeconds() / mDisappearTime.getSeconds();

			barrierTapeComponent->setPoleTransparency(0, transparency);
			barrierTapeComponent->setPoleTransparency(1, transparency);
			barrierTapeComponent->setTapeTransparency(transparency);

			return qsf::action::RESULT_CONTINUE;
		}

		MapHelper(getEntity().getMap()).destroyEntityById(mTargetEntityId);

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

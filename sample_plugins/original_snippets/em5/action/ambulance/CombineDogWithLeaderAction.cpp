// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/ambulance/CombineDogWithLeaderAction.h"
#include "em5/logic/local/ambulance/RescueDogLeaderLogic.h"

#include <qsf/component/base/TransformComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier CombineDogWithLeaderAction::ACTION_ID = "em5::CombineDogWithLeaderAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CombineDogWithLeaderAction::CombineDogWithLeaderAction() :
		Action(ACTION_ID),
		mState(STATE_INIT)
	{
		// Nothing here
	}

	CombineDogWithLeaderAction::~CombineDogWithLeaderAction()
	{
		// Nothing here
	}

	void CombineDogWithLeaderAction::init(qsf::Entity& targetEntity)
	{
		mTargetEntity = &targetEntity;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void CombineDogWithLeaderAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(mk) Add serialize function
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool CombineDogWithLeaderAction::onStartup()
	{
		// TODO(mk) Check conditions
		return mTargetEntity.valid();
	}

	qsf::action::Result CombineDogWithLeaderAction::updateAction(const qsf::Clock&)
	{
		// Get the target entity instance
		QSF_CHECK(mTargetEntity.valid(), "CombineDogWithLeaderAction::updateAction() was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		switch (mState)
		{
			case STATE_INIT:
			{
				ensureRelativePositions();

				RescueDogLeaderLogic::linkDogWithLeader(mTargetEntity.getSafe());
				// Fall through by design
			}
		}

		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CombineDogWithLeaderAction::ensureRelativePositions()
	{
		qsf::TransformComponent& transformComponentUnit = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		qsf::TransformComponent& transformComponent = mTargetEntity->getOrCreateComponentSafe<qsf::TransformComponent>();

		// Position dog at correct offset
		static const glm::vec3 DOGOFFSET(0.07f, 0.f, 0.96f);
		const glm::vec3 offsetWorld = transformComponent.getTransform().vec3DirectionLocalToWorld(DOGOFFSET);
		transformComponentUnit.setPosition(transformComponent.getPosition() - offsetWorld);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

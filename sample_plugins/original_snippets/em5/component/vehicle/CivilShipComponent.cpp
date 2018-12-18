// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/vehicle/CivilShipComponent.h"
#include "em5/ai/MovementModes.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/ActionPriority.h"

#include <qsf_game/component/movement/AiPathComponent.h>

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/plugin/QsfJobs.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CivilShipComponent::COMPONENT_ID = qsf::StringHash("em5::CivilShipComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CivilShipComponent::CivilShipComponent(qsf::Prototype* prototype) :
		Component(prototype),
		mPathEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing to do in here
	}

	CivilShipComponent::~CivilShipComponent()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void CivilShipComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Nothing yet
	}

	bool CivilShipComponent::onStartup()
	{
		mUpdateJobProxy.registerAt(qsf::QsfJobs::ANIMATION_GENERAL, boost::bind(&CivilShipComponent::updatePath, this, _1));

		return true;
	}

	uint64 CivilShipComponent::getPathEntityId() const
	{
		return mPathEntityId;
	}

	void CivilShipComponent::setPathEntityId(uint64 pathEntityId)
	{
		mPathEntityId = pathEntityId;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CivilShipComponent::updatePath(const qsf::JobArguments& arguments)
	{
		if (isSimulating() && isRunning())
		{
			qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();

			if (nullptr == actionComponent.getAction<MoveAction>())
			{
				followPath();
			}
		}
	}

	bool CivilShipComponent::followPath()
	{
		qsf::Entity* entity = getEntity().getMap().getEntityById(mPathEntityId);
		if (nullptr == entity)
			return false;

		qsf::game::AiPathComponent* aiPathComponent = entity->getComponent<qsf::game::AiPathComponent>();
		if (nullptr == aiPathComponent)
			return false;

		std::auto_ptr<qsf::ai::Path> path = aiPathComponent->createAiPathSmoothed();
		const qsf::logic::TargetPoint targetPoint(path->getNodes().back().mPosition, false, 0.0f, 0.1f);

		qsf::ActionComponent& actionComponent = getEntity().getOrCreateComponentSafe<qsf::ActionComponent>();
		actionComponent.pushAction<MoveAction>(action::DAILY_ROUTINE).init(new qsf::ai::ReachSinglePointGoal(targetPoint), MovementModes::MOVEMENT_MODE_DEFAULT, path.release());

		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/police/RedirectTrafficCommand.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/move/MoveAction.h"
#include "em5/action/police/RedirectTrafficAction.h"
#include "em5/ai/NavigationMaps.h"
#include "em5/ai/MoverType.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/component/movement/PersonMagnetComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/goal/ReachSinglePointGoal.h>
#include <qsf_ai/worldModel/trafficLanes/ClosestTrafficLanesHelper.h>

#include <qsf_game/command/CommandContext.h>
#include <qsf_game/equipment/EquipmentComponent.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 RedirectTrafficCommand::PLUGINABLE_ID = qsf::StringHash("em5::RedirectTrafficCommand");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 RedirectTrafficCommand::ACTION_PRIORITY = action::COMMAND_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	RedirectTrafficCommand::RedirectTrafficCommand(qsf::game::CommandManager* commandManager) :
		Command(commandManager, PLUGINABLE_ID)
	{
		// Priority is at default value

		// Set name of the mouse cursor
		mIconSettings.mShowAsGuiButton = false;
		mIconSettings.mMouseIconName = "redirect_traffic";
	}

	bool RedirectTrafficCommand::getTargetStreet(const qsf::Entity& entity, qsf::ai::SpatialConfiguration3D& result)
	{
		qsf::TransformComponent* transformComponent = entity.getComponent<qsf::TransformComponent>();
		if (nullptr != transformComponent)
		{
			// TODO(mz): should probably have some max-distance.
			std::vector<qsf::ai::SpatialConfiguration3D> closestConfigs;
			qsf::ai::findClosestConfigurationsOnIdealLane(transformComponent->getPosition(), NavigationMap::STREET_NETWORK, MoverType::CIVIL_VEHICLE, closestConfigs);
			if (!closestConfigs.empty())
			{
				// TODO(mz): Find optimal config - maybe closest - maybe the one going exactly in the opposite direction - or a mixture of those.
				result = closestConfigs[0];
				return true;
			}
		}

		return false;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	bool RedirectTrafficCommand::checkAvailable()
	{
		return true;
	}

	bool RedirectTrafficCommand::checkCaller(qsf::Entity& caller)
	{
		// Caller has action with a too high priority?
		if (!checkCallerActionPriority(caller, ACTION_PRIORITY))
			return false;

		// Check if unit state is valid
		EntityHelper entityHelper(caller);
		if (!entityHelper.isSquadPersonInValidState())
			return false;

		// Check if equipment matches
		if (!entityHelper.checkIsActiveEquipment(equipment::POLICE_SIGNALLINGDISC))
			return false;

		return true;
	}

	bool RedirectTrafficCommand::checkContext(const qsf::game::CommandContext& context)
	{
		// We need a target entity
		if (nullptr == context.mTargetEntity)
			return false;

		// unit has to command itself here
		return context.mCaller == context.mTargetEntity;
	}

	void RedirectTrafficCommand::execute(const qsf::game::CommandContext& context)
	{
		if (nullptr != context.mCaller)
		{
			// Access the caller's action plan
			qsf::ActionComponent& actionComponent = getActionComponent(*context.mCaller);
			actionComponent.clearPlan();

			qsf::ai::SpatialConfiguration3D target;
			if (getTargetStreet(*context.mCaller, target))
			{
				qsf::logic::TargetPoint targetPoint;
				targetPoint.mPosition = target.mPosition;
				targetPoint.mDistance = 0.5f;
				targetPoint.mTolerance = 1.f;

				actionComponent.pushAction<MoveAction>(ACTION_PRIORITY).init(new qsf::ai::ReachSinglePointGoal(targetPoint));
				actionComponent.pushAction<RedirectTrafficAction>(ACTION_PRIORITY).init();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

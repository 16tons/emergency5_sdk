// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/targetpoint/ExtinguishTargetPointProvider.h"
#include "em5/debug/GameDebugGroup.h"
#include "em5/fire/FireHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_logic/targetpoint/TargetPointManager.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/map/Entity.h>
#include <qsf/debug/request/CircleDebugDrawRequest.h>
#include <qsf/debug/DebugDrawManager.h>
#include <qsf/base/ScratchBuffer.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ExtinguishTargetPointProvider::TARGET_POINT_ID = qsf::StringHash("ExtinguishTargetPointProvider");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExtinguishTargetPointProvider::ExtinguishTargetPointProvider(qsf::logic::TargetPointManager* manager) :
		qsf::logic::TargetPointProvider(manager)
	{
		// Register callbacks
		manager->registerProvider(TARGET_POINT_ID, this);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	void ExtinguishTargetPointProvider::calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const
	{
		qsf::EntityVectorScratchBuffer extinguishTargetEntities;
		FireHelper::getExtinguishTargets(extinguishTargetEntities, target, caller);
		QSF_CHECK(!extinguishTargetEntities.empty(), "ExtinguishTargetPointProvider::calculateTargetPoints() Found neither fire particles nor fire components for entity " << target.getId() << ", unable to determine extinguish position", return);

		// Take 90% of the extinguish range to have an better lock while extinguishing
		const float distance = FireHelper::getExtinguishRangeFromEntity(caller) * 0.9f;
#ifndef ENDUSER
		const bool showDebug = GameDebugGroup::getInstanceSafe().getShowFireDebug();
#endif
		for (qsf::Entity* fireEffectEntity : extinguishTargetEntities)
		{
			// We ignore the target transform here, as we are relying on childs anyway.
			// we *could* ask for all the relative link positions instead and recalculate the absolute positions given the transform component passed.
			// But I don't think burning entities will ever move so we might be good for EM5
			qsf::TransformComponent& fireEffectTransformComponent = fireEffectEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
			glm::vec3 position = fireEffectTransformComponent.getPosition();

			// Targetposition should not be in height
			qsf::TransformComponent& targetTransformComponent = target.getOrCreateComponentSafe<qsf::TransformComponent>();
			position.y = targetTransformComponent.getPosition().y;


			bool pushNewTarget = true;
			// to avoid to have too many targets, ignore targets, that are near already pushed targets
			for (qsf::logic::TargetPoint& tp : targetPoints)
			{
				if (qsf::game::DistanceHelper::get2dDistance(tp.mPosition, position) < 4.f)
				{
					pushNewTarget = false;
					continue;
				}
			}

			if (pushNewTarget)
				targetPoints.emplace_back(position, false, distance, 0.5f);

#ifndef ENDUSER
			if (showDebug)
			{
				QSF_DEBUGDRAW.requestDraw(qsf::CircleDebugDrawRequest(position, qsf::CoordinateSystem::getUp(), 0.5f, qsf::Color4::WHITE), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
			}
#endif
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

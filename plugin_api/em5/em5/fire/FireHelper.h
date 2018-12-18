// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FireComponent;
}
namespace qsf
{
	class Entity;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 static fire helper class
	*/
	class EM5_API_EXPORT FireHelper
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static float getAutomatismDistanceFromCaller(qsf::Entity& callerEntity);
		static qsf::Entity* getConnectedHydrantFromEntity(const qsf::Entity& firemanEntity);
		static const qsf::Entity* findFreeHydrantNearEntity(qsf::Entity& firemanEntity, qsf::Entity& targetEntity, bool& foundAtLeastOneInRange);
		static bool isHydrantInRange(const qsf::Entity& hydrantEntity, const glm::vec3& position, float radius);
		static bool isHydrantInRange(const qsf::Entity& hydrantEntity, const qsf::Entity& targetEntity, float radius);
		static void getExtinguishTargets(std::vector<qsf::Entity*>& outTargets, qsf::Entity& targetEntity, qsf::Entity& firefightingUnit);
		static void getSortedTargetsInRange(std::vector<FireComponent*>& outTargets, qsf::Entity& callerEntity, float range);
		static qsf::Entity* getAnyEffectTarget(qsf::Entity& callerEntity, qsf::Entity& targetEntity);
		static qsf::Entity* getEffectTargetInRange(qsf::Entity& callerEntity, qsf::Entity& targetEntity, float maxExtinguishRange);
		static bool isTargetInExtinguishRange(qsf::Entity& targetEntity, const glm::vec3& callerPosition, float maxExtinguishRange);
		static float getExtinguishRangeFromEntity(qsf::Entity& callerEntity);
		static float getHardRadius(qsf::Entity& callerEntity);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		FireHelper() {}
		~FireHelper() {}


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/WeakPtr.h>

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Component;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Static distance helper class
		*/
		class QSF_GAME_API_EXPORT DistanceHelper
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			// 2D variants that ignore height differences.
			// These throw an exception if any entity passed as a parameter here doesn't have a transform component attached
			static float get2dDistance(const glm::vec3& point, const Entity& entity);
			static float get2dDistance(const glm::vec3& targetPoint, const glm::vec3& callerPoint);
			static float get2dDistance(const Entity& targetEntity, const Entity& callerEntity);

			static float getSquaredDistance(const glm::vec3& point, const Entity& entity);
			static float getSquaredDistance(const glm::vec3& targetPoint, const glm::vec3& callerPoint);
			static float getSquaredDistance(const Entity& targetEntity, const Entity& callerEntity);

			static float getDistance(const glm::vec3& point, const Entity& entity);
			static float getDistance(const glm::vec3& targetPoint, const glm::vec3& callerPoint);
			static float getDistance(const Entity& targetEntity, const Entity& callerEntity);

			static qsf::Entity* getClosestEntity(std::vector<qsf::Entity*>& entitiesToCheck, const Entity& entity);
			static qsf::Entity* getClosestEntity(std::vector<qsf::WeakPtr<qsf::Entity>>& entitiesToCheck, const Entity& entity);


		};


		/**
		*  @brief
		*    Class used as compare function for distances of components. Use this for std::sort
		*
		*  @code
		*    detail::IsLeftComponentCloserToTargetPosition compareObject(ownerPosition);
		*    std::sort(componentVector.begin(), componentVector.end(), compareObject);
		*  @endcode
		*/
		class QSF_GAME_API_EXPORT IsLeftComponentCloserToTargetPosition
		{
		public:
			IsLeftComponentCloserToTargetPosition(glm::vec3 targetPosition);

			bool operator ()(qsf::Component* lhs, qsf::Component* rhs) const;

			// Need an copy constructor for saving the position
			IsLeftComponentCloserToTargetPosition& operator =(const IsLeftComponentCloserToTargetPosition& other);

		private:
			glm::vec3 mTargetPosition;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf

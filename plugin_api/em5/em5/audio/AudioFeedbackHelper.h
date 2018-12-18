// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class OrderInfo;
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
	*    EMERGENCY 5 audio feedback helper class
	*/
	class EM5_API_EXPORT AudioFeedbackHelper
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Play command feedback for the given entity
		*
		*  @param[in] entity
		*    The entity, which defines audio index range and speaker index
		*/
		static void playCommandResponse(qsf::Entity& entity);

		/**
		*  @brief
		*    Play command feedback for the given order info
		*
		*  @param[in] orderInfo
		*    Order info pointer, may be a null pointer to play a completely unspecific command feedback
		*
		*  @note
		*    - If possible, prefer the method "playCommandResponse(qsf::Entity&)"
		*    - This one is used for ordering of units, when there is no entity created yet
		*/
		static void playCommandResponse(const OrderInfo* orderInfo);

		/**
		*  @brief
		*    Play command feedback for the given entity
		*
		*  @param[in] entity
		*    The entity, which defines audio index range and speaker index
		*/
		static void playSelectResponse(qsf::Entity& entity);

		/**
		*  @brief
		*    Play command feedback for multiple entities
		*
		*  @param[in] entityList
		*    The list of entities
		*/
		static void playSelectResponse(const std::vector<qsf::Entity*>& entityList);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

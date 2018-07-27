// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/main/cologne/mainevent203/MainEvent203.h"	// TODO(mk) maybe a bit to much


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
	*	 Class matches gangsterEntity and path Entities via watching the event tag endings
	*    This class is very close the gamelogic of MainEvent203
	*/
	class GangsterPathMatcher
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		void match(std::vector<qsf::WeakPtr<qsf::Entity>>& personEntities, const std::string& personTag,
			std::vector<qsf::WeakPtr<qsf::Entity>>& waypoints, const std::string& waypointTag,
			std::vector<MainEvent203::GangsterWaypointData>& result, bool loop);


	};
}

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <string>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		namespace server
		{


			//[-------------------------------------------------------]
			//[ Helper methods                                        ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Returns a list of global ipv6 addresses (separated by ',') of the local computer (which can be used to connect to the local computer from the internet)
			*/
			const std::string getLocalIPV6Address();


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // server
	} // multiplayer
} // em5

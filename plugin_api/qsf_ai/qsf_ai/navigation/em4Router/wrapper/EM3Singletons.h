// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		class RouterScene;
		class ERouteFinder;
		class ActorFactory;
		class ETerrainManager;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		class QSF_AI_API_EXPORT EM3
		{


		//[-------------------------------------------------------]
		//[ Public static members                                 ]
		//[-------------------------------------------------------]
		public:
			static RouterScene*		Scene;
			static ERouteFinder*	Router;
			static ETerrainManager*	Terrains;
			static ActorFactory*	ActorFactory;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf

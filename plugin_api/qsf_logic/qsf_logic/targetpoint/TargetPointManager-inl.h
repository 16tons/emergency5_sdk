// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline TargetPointManager::TargetPointManager()
		{
			// Register all known and supported CAMP pluginable classes
			registerPluginables();
		}

		inline TargetPointManager::~TargetPointManager()
		{
			// Unregister all known and supported CAMP pluginable classes
			unregisterPluginables();
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf

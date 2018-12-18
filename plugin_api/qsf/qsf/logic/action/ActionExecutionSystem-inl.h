// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ActionExecutionSystem::ActionExecutionSystem() :
		LogicSystem("Action execution system")
	{
		// Nothing here
	}

	inline ScriptActionManager& ActionExecutionSystem::getScriptActionManager()
	{
		return mScriptActionManager;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

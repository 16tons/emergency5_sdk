// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PluginSystem::PluginSystem() :
		mRunning(false)
	{
		// Nothing to do in here
	}

	inline PluginSystem::~PluginSystem()
	{
		// Nothing to do in here
	}

	inline const std::vector<Plugin*>& PluginSystem::getPlugins() const
	{
		return mPlugins;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* PluginSystem::getName() const
	{
		return "Plugin system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

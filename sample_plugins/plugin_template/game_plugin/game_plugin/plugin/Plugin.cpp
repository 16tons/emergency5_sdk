// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"
#include "game_plugin/plugin/Plugin.h"

#include <em5/plugin/version/PluginVersion.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	Plugin::Plugin() :
		qsf::Plugin(new em5::PluginVersion())
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Plugin methods                 ]
	//[-------------------------------------------------------]
	bool Plugin::onInstall()
	{
		try
		{
			// Declare CAMP reflection system classes
			// -> Use Qt's "QT_TR_NOOP()"-macro in order to enable Qt's "lupdate"-program to find the internationalization texts


			// Done
			return true;
		}
		catch (const std::exception& e)
		{
			// Error!
			QSF_ERROR("Failed to install the plugin '" << getName() << "'. Exception caught: " << e.what(), QSF_REACT_NONE);
			return false;
		}
	}

	bool Plugin::onStartup()
	{
		// Nothing to do in here

		// Done
		return true;
	}

	void Plugin::onShutdown()
	{
		// Nothing to do in here
	}

	void Plugin::onUninstall()
	{
		// Removing classes is not possible within the CAMP reflection system

		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user

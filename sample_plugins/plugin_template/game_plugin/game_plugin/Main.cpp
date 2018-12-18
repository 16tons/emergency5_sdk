// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/PrecompiledHeader.h"


// Is this project built as shared library?
#ifdef _USRDLL

	//[-------------------------------------------------------]
	//[ Includes                                              ]
	//[-------------------------------------------------------]
	#include "game_plugin/Export.h"
	#include "game_plugin/plugin/Plugin.h"


	//[-------------------------------------------------------]
	//[ Global functions                                      ]
	//[-------------------------------------------------------]
	GAMEPLUGIN_FUNCTION_EXPORT qsf::Plugin* CreatePluginInstance_game_plugin()
	{
		return new user::Plugin();
	}

#endif

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "editor_plugin/PrecompiledHeader.h"


// Is this project built as shared library?
#ifdef _USRDLL

	//[-------------------------------------------------------]
	//[ Includes                                              ]
	//[-------------------------------------------------------]
	#include "editor_plugin/Export.h"
	#include "editor_plugin/plugin/Plugin.h"


	//[-------------------------------------------------------]
	//[ Global functions                                      ]
	//[-------------------------------------------------------]
	EDITORPLUGIN_FUNCTION_EXPORT qsf::Plugin* CreatePluginInstance_editor_plugin()
	{
		return new user::editor::Plugin();
	}

#endif

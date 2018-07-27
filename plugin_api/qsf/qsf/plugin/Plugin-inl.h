// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const std::string& Plugin::getFilename() const
	{
		return mFilename;
	}

	inline const std::vector<const camp::Class*>& Plugin::getCampClasses() const
	{
		return mCampClasses;
	}

	inline const PluginVersionBase& Plugin::getPluginVersion() const
	{
		QSF_ASSERT(nullptr != mPluginVersion, "QSF tried to get plugin version, but the plugin failed to provide a plugin version object upon construction", QSF_REACT_THROW);
		return *mPluginVersion;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline Plugin::Plugin(PluginVersionBase* pluginVersion) :
		mPluginVersion(pluginVersion)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

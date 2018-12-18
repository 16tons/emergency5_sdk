// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <string>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    QSF plugin version information base class
	*/
	class QSF_API_EXPORT PluginVersionBase
	{


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		std::string mBuildConfiguration;	///< Build configuration string, e.g. "Debug", "Release", "Retail"
		std::string mBuildArchitecture;		///< Build platform architecture string, e.g. "x86" or "x64"
		std::string mBuildDate;				///< Build timestamp: date string
		std::string mBuildTime;				///< Build timestamp: time string
		std::string mSvnRevision;			///< Build SVN revision number as delivered by the "svnversion" tool, e.g. "1234M" (with M for local modifications)


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*/
		PluginVersionBase(const std::string& configuration, const std::string& architecture, const std::string& date, const std::string& time, const std::string& svnRevision);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}  // qsf

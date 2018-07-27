// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>

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
	*    Platform independent crash reporter class
	*
	*  @remarks
	*    The crash reporter class is implemented as a singleton instead of a QSF system.
	*    This way, it can easily be accessed and initialized directly from the application
	*    executable's main function even before the qsf startup procedure begins.
	*/
	class QSF_API_EXPORT CrashReporter : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static CrashReporter& GetInstance();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::CrashReporter methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void setApplicationName(const std::string& applicationName) = 0;
		virtual void setApplicationVersionString(const std::string& versionString) = 0;
		virtual void setCrashMessage(const std::string& title, const std::string& text) = 0;
		virtual void setSupportEmail(const std::string& supportEmail) = 0;
		virtual void setSupportServer(const std::string& hostname, uint16 port) = 0;
		virtual void setSupportHomepage(const std::string& url) = 0;
		virtual void setReportFilePath(const std::string& filePath) = 0;
		virtual void sealConfiguration() = 0;	// Call this method to avoid that for instance the configuration of a total conversion of a project doesn't get changed later on
		virtual void initialize() = 0;
		virtual void addFileAttachment(const std::string& filename) = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		CrashReporter();
		virtual ~CrashReporter();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

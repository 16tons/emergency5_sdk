// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/CrashReporter.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class QSF_API_EXPORT CrashReporterNull : public CrashReporter
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class CrashReporter;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::CrashReporter methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void setApplicationName(const std::string& applicationName) override;
		virtual void setApplicationVersionString(const std::string& versionString) override;
		virtual void setCrashMessage(const std::string& title, const std::string& text) override;
		virtual void setSupportEmail(const std::string& supportEmail) override;
		virtual void setSupportServer(const std::string& hostname, uint16 port) override;
		virtual void setSupportHomepage(const std::string& url) override;
		virtual void setReportFilePath(const std::string& filePath) override;
		virtual void sealConfiguration() override;
		virtual void initialize() override;
		virtual void addFileAttachment(const std::string& filename) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		CrashReporterNull();
		virtual ~CrashReporterNull();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

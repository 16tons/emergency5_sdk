// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 update check class
	*/
	class EM5_API_EXPORT UpdateCheck
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class NetworkManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const std::string UPDATE_URL;
		static const std::string FORMAT_TYPE;
		static const uint32		 FORMAT_VERSION;
		static const std::string UPDATE_SETUP_EXE_NAME;

		struct VersionInformation
		{
			std::string applicationName;
			std::string applicationVersion;
			std::string architecture;
			std::string region;
			std::string languagePackage;
			std::string edition;
			std::string productVariant;
			std::string launcherChecksum;
		};


	//[-------------------------------------------------------]
	//[ Public boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		/** void onProgress(uint32 currentFile, uint32 totalFiles, uint64 currentBytes, uint64 totalBytes) */
		boost::signals2::signal<void(uint32, uint32, uint64, uint64)> progress;

		/** void onFinished() */
		boost::signals2::signal<void()> finished;

		/** void onError() */
		boost::signals2::signal<void()> error;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		bool isCheckComplete() const;
		bool isUpdateAvailable() const;
		const std::string& getUpdateVersion() const;
		bool startCheckForUpdates(const VersionInformation& versionInformation);
		bool startDownloadAndInstallUpdate();
		const std::string& getInfoText() const;
		void getMatchmakingAddress(std::string& matchmakingUrl, uint16& matchmakingPort) const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		UpdateCheck();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~UpdateCheck();

		bool checkForUpdatesAtNextUrl();

		bool startDownload(const std::string& url, const std::string& absoluteFilename, uint32 currentFileIndex, uint32 totalFileCount, boost::function<void(const std::string&, const std::string&)> callback);

		void onDownloadProgress(uint64 currentBytes, uint64 totalBytes);
		void onDownloadFinished(boost::function<void(const std::string&, const std::string&)> callback);
		void onDownloadError();

		void parseUpdateInformation(const std::string& effectiveUrl, const std::string& absoluteFilename);

		void downloadedUpdatePackage(const std::string& effectiveUrl, const std::string& absoluteFilename);

		std::string searchPatchSetup() const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct Private;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool		mIsCheckComplete;
		bool		mIsUpdateAvailable;
		std::string mUpdateVersion;
		std::string mInfoText;

		// Progress reporting
		uint32 mTotalFileCount;
		uint32 mCurrentFileIndex;

		// Error handling
		uint32 mTryCounter;

		// Internal data
		Private& mPrivate;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

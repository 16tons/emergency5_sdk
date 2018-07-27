// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/application/PlayerProgress.h"

#include <qsf/base/manager/Manager.h>
#include <qsf/forward/BoostPTree.h>
#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/network/HttpDownload.h>

#include <boost/signals2.hpp>

#include <queue>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class NamedIdentifier;
}
namespace em5
{
	class Achievement;
	class HighscoreManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class EM5_API_EXPORT OnlineAccountData
	{
	public:
		std::string screenName;
		std::string	title;
		uint32		ep;
		uint32		totalScore;
		uint32		lastTitleScore;
		uint32		nextTitleScore;
		bool		firstLogin;

		inline OnlineAccountData() :
			ep(qsf::getUninitialized<uint32>()),
			totalScore(qsf::getUninitialized<uint32>()),
			lastTitleScore(qsf::getUninitialized<uint32>()),
			nextTitleScore(qsf::getUninitialized<uint32>()),
			firstLogin(false)
		{
			// Nothing to do here
		}

		const Achievement* getCurrentTitleAchievement() const;
	};

	/**
	*  @brief
	*    EMERGENCY 5 network manager class
	*/
	class EM5_API_EXPORT AccountManager : public qsf::Manager
	{


	//[-------------------------------------------------------]
	//[ Friend classes                                        ]
	//[-------------------------------------------------------]
		friend PlayerProgress;
		friend HighscoreManager;


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (const std::string&, const std::string&, std::string&, qsf::StringQueryParameters&)> LogInStringQueryParameters;			///< This Boost signal is emitted just before the string parameters are used for log-in, log-in name as first parameter, password as second parameter, login URL as third parameter and current string query parameters as fourth parameter
		boost::signals2::signal<void (boost::property_tree::wptree&)>													  LogInResponsePTree;					///< This Boost signal is emitted after log-in response, can e.g. be used to transform the response to make it compatible with the EMERGENCY 5 account manager, first parameter is server ptree response
		boost::signals2::signal<void (std::string&, qsf::StringQueryParameters&)>										  LogOutStringQueryParameters;			///< This Boost signal is emitted just before the string parameters are used for log-out, log-out URL as first parameter and current string query parameters as second parameter
		boost::signals2::signal<void (std::string&, qsf::StringQueryParameters&)>										  QueryNewsEntryStringQueryParameters;	///< This Boost signal is emitted just before the string parameters are used for query news entry, query news entry URL as first parameter and current string query parameters as second parameter
		boost::signals2::signal<void (std::string&, qsf::StringQueryParameters&)>										  CloudListStringQueryParameters;		///< This Boost signal is emitted just before the string parameters are used for cloud list, cloud list URL as first parameter and current string query parameters as second parameter
		boost::signals2::signal<void (std::string&, qsf::MultipartFormPostData&)>										  CloudStoreMultipartFormPostData;		///< This Boost signal is emitted just before the string parameters are used for cloud store, cloud store URL as first parameter and current multipart form post data as second parameter
		boost::signals2::signal<void (const std::string&, std::string&, qsf::MultipartFormPostData&)>					  CloudDeleteMultipartFormPostData;		///< This Boost signal is emitted just before the string parameters are used for cloud delete, path as first parameter, cloud delete URL as second parameter and current multipart form post data as third parameter
		boost::signals2::signal<void (const std::string&, std::string&, qsf::MultipartFormPostData&)>					  CloudLoadMultipartFormPostData;		///< This Boost signal is emitted just before the string parameters are used for cloud load, path as first parameter, cloud load URL as second parameter and current multipart form post data as third parameter
		boost::signals2::signal<void (boost::property_tree::ptree&)>													  CloudResponsePTree;					///< This Boost signal is emitted just before the cloud ptree response is processed, can e.g. be used to transform the response to make it compatible with the EMERGENCY 5 account manager, first parameter is server ptree response


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<qsf::CookieComponents> SessionCookies;

		struct NewsEntry
		{
			uint32		newsId;
			std::string url;
			std::string title;
			std::string sub;
			std::string text;
			std::string date;
		};

		struct ProgressHttpPostQueryData
		{
			inline ProgressHttpPostQueryData() :
				parseAndDeleteDownloadedJsonFile(false)
			{
				// Nothing to do in here
			}

			std::string absoluteFilename;
			std::string url;
			std::shared_ptr<qsf::MultipartFormPostData> postData;
			boost::function<bool(const boost::property_tree::ptree&)> resultCallback;
			bool parseAndDeleteDownloadedJsonFile;
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static std::string getAccountDirectoryPath(const std::string& screenName);
		static std::string getAccountTempJsonPath();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		AccountManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AccountManager();

		const std::string& getServerAddress() const;
		void setServerAddress(const std::string& serverAddress);
		void setSharedSecret(const std::string& sharedSecret);	// No getter method for security reasons
		const std::string& getCloudSyncDirectoryAndSlug() const;
		const std::string& getGameSlug() const;
		void setGameSlug(const std::string& gameSlug);
		const std::string& getSlug() const;
		void setCloudSyncDirectoryAndSlug(const std::string& directory, const std::string& slug);

		bool isLoggedIn() const;
		void setCloudSyncPaused(bool paused);
		const SessionCookies& getSessionCookies() const;
		const OnlineAccountData& getOnlineAccountData() const;
		std::string getAccountDirectoryPath() const;
		bool save();
		bool getConnectionError() const;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		bool startup();
		void shutdown();

		//[-------------------------------------------------------]
		//[ Server interactions                                   ]
		//[-------------------------------------------------------]
		bool logIn(const std::string& loginName, const std::string& password, bool passwordAlreadyHashed = false);
		void logOut();
		bool setTitle(const qsf::NamedIdentifier& title);
		bool queryNewsEntry(NewsEntry& entry);

		//[-------------------------------------------------------]
		//[ Server interactions                                   ]
		//[-------------------------------------------------------]
		/** Triggers a new synchronization cycle - if cloud sync is already synchronizing it will rescan all files after finishing the current synchronization */
		void triggerCloudSync(bool forceBlockingAutosaveDownload = false);

		/** Triggers a new synchronization cycle - but only in case we're currently logged in */
		void tryTriggerCloudSync(bool forceBlockingAutosaveDownload = false);

		/** Returns true while the cloud sync is active (comparing or transferring) */
		bool isCloudSyncActive() const;

		/** Returns true while the cloud sync is active and not all locally outdated or missing files have been transferred yet */
		bool isCloudSyncLocalCopyOutdated() const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		bool notifyAchievementProgress(const Achievement& achievement);
		bool unlockMedal(const qsf::NamedIdentifier& medal, PlayerProgress::MedalType medalType);
		bool setTotalScore(uint32 score);
		uint32 submitHighscore(const std::string& ranking, const std::string& name, uint32 score, const std::string& group);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		std::string generateVerifyStringHash(const std::string& value) const;

		void queueHttpPostDownload(const std::string& url, const std::string& absoluteTargetFileName, std::shared_ptr<qsf::MultipartFormPostData> postData, boost::function<bool(const boost::property_tree::ptree&)> resultCallback = 0);
		bool executeHttpPostDownload(const std::string& url, const std::string& absoluteTargetFileName, std::shared_ptr<qsf::MultipartFormPostData> postData, boost::function<bool(const boost::property_tree::ptree&)> resultCallback = 0);

		void queueHttpPostQuery(const std::string& url, std::shared_ptr<qsf::MultipartFormPostData> postData, boost::function<bool(const boost::property_tree::ptree&)> resultCallback = 0);
		void queueHttpPostQuery(const std::string& url, const qsf::StringQueryParameters& postParameters, boost::function<bool(const boost::property_tree::ptree&)> resultCallback = 0);
		bool executeHttpPostQuery(const std::string& url, std::shared_ptr<qsf::MultipartFormPostData> postData, boost::function<bool(const boost::property_tree::ptree&)> resultCallback = 0);
		bool executeHttpPostQuery(const std::string& url, const qsf::StringQueryParameters& postParameters, boost::function<bool(const boost::property_tree::ptree&)> resultCallback = 0);

		void updateJob(const qsf::JobArguments& jobArguments);
		void updateAchievementUpload(const qsf::JobArguments& jobArguments);
		void updateCloudSynchronisationWindow();

		void onHttpPostQueryFinished();
		void onHttpPostQueryFailed();

		bool handlePostQueryResponse();

		bool onSetTotalScoreHttpResponse(const boost::property_tree::ptree& resultPtree);
		bool onSetAchievementProgressHttpResponse(const boost::property_tree::ptree& resultPtree);
		bool onSetHighscoreHttpResponse(const boost::property_tree::ptree& resultPtree, uint32& rank);

		bool onCloudListFilesHttpResponse(const boost::property_tree::ptree& resultPtree);
		bool onCloudLoadFileHttpResponse(const boost::property_tree::ptree& resultPtree, const std::string& absoluteFilename, std::time_t modificationDate);
		bool onCloudDeleteFileHttpResponse(const boost::property_tree::ptree& resultPtree);
		bool onCloudStoreFileHttpResponse(const boost::property_tree::ptree& resultPtree, const std::string& absoluteFilename, std::time_t localTimestamp);

		void updateCloudSyncStateFile();

		void onAchievementUpdated(const qsf::MessageParameters& parameters);
		void onPlayerMedalUnlocked(const qsf::MessageParameters& parameters);
		void onPlayerTotalScoreChanged(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string		  mServerAddress;
		bool			  mIsLoggedIn;
		bool			  mConnectionError;
		OnlineAccountData mOnlineAccountData;
		SessionCookies	  mSessionCookies;
		bool			  mCloudSyncPaused;

		qsf::MessageProxy mAchievementMessageProxy;
		qsf::MessageProxy mPlayerMedalMessageProxy;
		qsf::MessageProxy mPlayerTotalScoreMessageProxy;

		qsf::JobProxy mJobProxy;					///< Realtime job proxy for retrying to send progress after failure
		qsf::JobProxy mAchievementUploadJobProxy;	///< Realtime job proxy for sending achievement progress

		std::queue<ProgressHttpPostQueryData>		   mHttpPostQueue;
		qsf::HttpDownload*							   mCurrentProgressHttpPost;
		boost::container::flat_set<const Achievement*> mAchievementsToUpload;

		bool   mIsCloudSyncRescanQueued;
		uint32 mNumberOfCloudSyncDownloadsQueued;
		uint32 mNumberOfCloudSyncUploadsAndDeletesQueued;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

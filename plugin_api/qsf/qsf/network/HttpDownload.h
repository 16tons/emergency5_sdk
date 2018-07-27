// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/job/JobProxy.h"

#include <boost/signals2/signal.hpp>
#include <boost/container/flat_map.hpp>

#include <mutex>
#include <thread>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
struct curl_httppost;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Definitions                                           ]
	//[-------------------------------------------------------]
	typedef boost::container::flat_map<std::string, std::string> StringQueryParameters;


	//[-------------------------------------------------------]
	//[ Structs                                               ]
	//[-------------------------------------------------------]
	struct UrlComponents
	{
		// Example: "scheme://net_loc/path/to/file.ext;params?query#fragment"
		std::string scheme;		///< "scheme"
		std::string netLoc;		///< "net_loc"
		std::string path;		///< "/path/to/file.ext"
		std::string params;		///< "params"
		std::string query;		///< "query"
		std::string fragment;	///< "fragment"
	};

	struct CookieComponents
	{
		std::string domain;
		bool tailmatch;
		std::string path;
		bool secure;
		uint32 expires;
		std::string name;
		std::string value;

		inline CookieComponents(const std::string& domain, bool tailmatch, const std::string& path, bool secure, uint32 expires, std::string name, std::string value) :
			domain(domain),
			tailmatch(tailmatch),
			path(path),
			secure(secure),
			expires(expires),
			name(name),
			value(value)
		{
			// Nothing to do here
		};
	};


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class QSF_API_EXPORT MultipartFormPostData : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		MultipartFormPostData();
		explicit MultipartFormPostData(MultipartFormPostData&& other);

		~MultipartFormPostData();

		bool addFormField(const std::string& name, const std::string& value);
		bool addFileUpload(const std::string& name, const std::string& absoluteFilename);

		curl_httppost* getCurlHttpPostHandle() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		curl_httppost* mCurlHttpPostFormHandle;
		curl_httppost* mCurlHttpPostLastPtr;
		std::vector<std::vector<char>> mFileUploadData;


	};


	/**
	*  @brief
	*    HTTP download class
	*
	*  @note
	*    - The external library "curl" ( http://curl.haxx.se/libcurl/ ) is used
	*/
	class QSF_API_EXPORT HttpDownload : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static std::string urlEncode(const std::string& text);
		static std::string urlDecode(const std::string& text);
		static std::string buildQuery(const StringQueryParameters& queryParameters);
		static void splitUrl(const std::string& url, UrlComponents& components);
		static std::string buildUrl(const UrlComponents& components);

		/**
		*  @brief
		*    Constructs an absolute URL from the given URL and base URL following specifications from RFC 1808.
		*
		*  @param[in] url
		*    URL to construct the absolute URL for (if the given URL is already absolute it is returned as is)
		*  @param[in] baseUrl
		*    Base URL to use for absolute URL construction (may be the effective absolute URL of the entity the potentially relative URL was read from)
		*
		*  @return
		*    The absolute URL constructed from the given parameters
		*
		*  @note
		*    - If the base URL contains a filename, queries, parameters or fragments these elements are correctly removed before proceeding
		*    - Web addresses without a scheme, which contain a malformed netLoc, ("www.example.com/foobar.txt" instead of "//www.example.com/foobar.txt") might not be handled correctly by this method
		*/
		static std::string getAbsoluteUrl(const std::string& url, const std::string& sourceAbsoluteUrl);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] url
		*    URL, e.g. "http://10.0.0.28/upload/"
		*  @param[in] userPassword
		*    User name and user password in the form "<name>:<password>", e.g. "bob:1234", empty string if no authentication is necessary
		*/
		HttpDownload(const std::string& url, const std::string& userPassword = "");

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HttpDownload();

		const std::string& getUrl() const;
		const std::string& getEffectiveUrl() const;
		const std::string& getAbsoluteTargetFilename() const;

		bool isGood() const;
		bool isFinished() const;

		void cancel();
		bool wait();

		/* Will be sent as application/x-www-form-urlencoded */
		HttpDownload& setUrlencodedFormPostData(const std::string& postData);

		/* Will be sent as multipart/form-data */
		HttpDownload& setPostMultipartData(std::shared_ptr<MultipartFormPostData> postData);

		HttpDownload& setCookies(const std::vector<CookieComponents>& cookies);
		const std::vector<CookieComponents>& getCookies() const;

		bool storeByAbsoluteFilename(const std::string& absoluteFilename, bool waitUntilFinish = true);

		/** void onProgress(uint64 currentBytes, uint64 totalBytes) */
		boost::signals2::signal<void(uint64, uint64)> progress;

		/** void onFinished() */
		boost::signals2::signal<void()> finished;

		/** void onError() */
		boost::signals2::signal<void()> error;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static int _cb_progress(HttpDownload* self, double dltotal, double dlnow, double ultotal, double ulnow);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void downloadThread();

		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateJob(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string	  mUrl;
		std::string	  mUserPassword;
		std::string	  mEffectiveUrl;
		std::string   mAbsoluteTargetFilename;
		std::vector<CookieComponents> mCookies;
		bool		  mIsUrlencodedFormPost;
		std::string	  mUrlencodedFormPostData;
		bool		  mIsMultipartFormPost;
		std::shared_ptr<MultipartFormPostData> mMultipartFormPostData;
		bool		  mIsGood;
		bool		  mIsCancel;
		bool		  mIsDone;
		uint64		  mCurrentBytes;
		uint64		  mTotalBytes;
		std::thread   mThread;
		std::mutex    mMutex;
		JobProxy	  mJobProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

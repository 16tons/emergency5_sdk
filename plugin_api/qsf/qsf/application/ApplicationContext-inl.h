// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ApplicationContext::~ApplicationContext()
	{
		// Nothing to do in here
	}

	inline const std::string& ApplicationContext::getApplicationName() const
	{
		return mApplicationName;
	}

	inline void ApplicationContext::setApplicationName(const std::string& applicationName)
	{
		mApplicationName = applicationName;
	}

	inline const std::string& ApplicationContext::getShownApplicationName() const
	{
		return mShownApplicationName;
	}

	inline void ApplicationContext::setShownApplicationName(const std::string& shownApplicationName)
	{
		mShownApplicationName = shownApplicationName;
	}

	inline const std::string& ApplicationContext::getApplicationVersion() const
	{
		return mApplicationVersion;
	}

	inline void ApplicationContext::setApplicationVersion(const std::string& applicationVersion)
	{
		mApplicationVersion = applicationVersion;
	}

	inline const std::vector<NamedIdentifier>& ApplicationContext::getApplicationTags() const
	{
		return mApplicationTags;
	}

	inline const std::string& ApplicationContext::getOrganizationName() const
	{
		return mOrganizationName;
	}

	inline void ApplicationContext::setOrganizationName(const std::string& organizationName)
	{
		mOrganizationName = organizationName;
	}

	inline const std::string& ApplicationContext::getOrganizationDomain() const
	{
		return mOrganizationDomain;
	}

	inline void ApplicationContext::setOrganizationDomain(const std::string& organizationDomain)
	{
		mOrganizationDomain = organizationDomain;
	}

	inline const std::string& ApplicationContext::getSplashScreen() const
	{
		return mSplashScreen;
	}

	inline void ApplicationContext::setSplashScreen(const std::string& splashScreen)
	{
		mSplashScreen = splashScreen;
	}

	inline bool ApplicationContext::getSplashScreenInfoTexts() const
	{
		return mSplashScreenInfoTexts;
	}

	inline void ApplicationContext::setSplashScreenInfoTexts(bool show)
	{
		mSplashScreenInfoTexts = show;
	}

	inline const std::vector<std::string>& ApplicationContext::getPlugins() const
	{
		return mPlugins;
	}

	inline void ApplicationContext::setPlugins(const std::vector<std::string>& plugins)
	{
		mPlugins = plugins;
	}

	inline const std::vector<std::string>& ApplicationContext::getProjects() const
	{
		return mProjects;
	}

	inline void ApplicationContext::setProjects(const std::vector<std::string>& projects)
	{
		mProjects = projects;
	}

	inline const std::string& ApplicationContext::getCurrentProject() const
	{
		return mCurrentProject;
	}

	inline void ApplicationContext::setCurrentProject(const std::string& currentProject)
	{
		mCurrentProject = currentProject;
	}

	inline const std::string& ApplicationContext::getUserDirectory() const
	{
		return mUserDirectory;
	}

	inline void ApplicationContext::setUserDirectory(const std::string& userDirectory)
	{
		mUserDirectory = userDirectory;
	}

	inline const std::string& ApplicationContext::getCacheDirectory() const
	{
		return mCacheDirectory;
	}

	inline void ApplicationContext::setCacheDirectory(const std::string& cacheDirectory)
	{
		mCacheDirectory = cacheDirectory;
	}

	inline const std::string& ApplicationContext::getSettingsPrefix() const
	{
		return mSettingsPrefix;
	}

	inline void ApplicationContext::setSettingsPrefix(const std::string& settingsPrefix)
	{
		mSettingsPrefix = settingsPrefix;
	}

	inline const std::string& ApplicationContext::getApplicationClassName() const
	{
		return mApplicationClassName;
	}

	inline void ApplicationContext::setApplicationClassName(const std::string& applicationClassName)
	{
		mApplicationClassName = applicationClassName;
	}

	inline const boost::property_tree::ptree& ApplicationContext::getApplicationParameters() const
	{
		return mApplicationParameters;
	}

	inline void ApplicationContext::setApplicationParameters(const boost::property_tree::ptree& pTree)
	{
		mApplicationParameters = pTree;
	}

	inline const std::string& ApplicationContext::getFilename() const
	{
		return mFilename;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

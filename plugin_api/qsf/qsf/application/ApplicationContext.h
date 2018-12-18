// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/Context.h"
#include "qsf/base/NamedIdentifier.h"

#include <boost/property_tree/ptree.hpp>

#include <string>
#include <vector>


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
	*    Application context
	*/
	class QSF_API_EXPORT ApplicationContext : public Context
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const char	FORMAT_TYPE[];
		static const uint32 FORMAT_VERSION;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ApplicationContext();

		/**
		*  @brief
		*    Constructor loading the application context configuration by using a given filename
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the application context configuration to load
		*/
		explicit ApplicationContext(const std::string& filename);

		/**
		*  @brief
		*    Constructor loading the application context configuration by using a given filename
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the application context configuration to load
		*  @param[in] parametersPTree
		*    The application parameters as a generic Boost property tree; its structure is defined by the application
		*
		*  @note
		*    The parameters denoted in the specified file (if any) are replaced by parametersPTree, i.e. completely overwritten
		*/
		ApplicationContext(const std::string& filename, const boost::property_tree::ptree& parametersPTree);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ApplicationContext();

		/**
		*  @brief
		*    Get absolute settings filename prefix
		*
		*  @return
		*    Absolute settings filename prefix (example: "C:/Users/<User Name>/AppData/Roaming/Promotion Software GmbH/EMERGENCY 5 Offline Editor/emergency_5_offline_editor_")
		*/
		std::string getAbsoluteSettingsFilenamePrefix() const;

		//[-------------------------------------------------------]
		//[ Properties                                            ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the application name
		*
		*  @return
		*    The UTF-8 application name (example: "QSF sample")
		*
		*  @note
		*    - This application name is used e.g. to automatically load settings
		*/
		inline const std::string& getApplicationName() const;

		/**
		*  @brief
		*    Set the application name
		*
		*  @param[in] applicationName
		*    The new UTF-8 application name (example: "QSF sample")
		*
		*  @see
		*    - "qsf::ApplicationContext::getApplicationName()"
		*/
		inline void setApplicationName(const std::string& applicationName);

		/**
		*  @brief
		*    Return the optional shown application name
		*
		*  @return
		*    The optional UTF-8 shown application name (example: "QSF sample")
		*/
		inline const std::string& getShownApplicationName() const;

		/**
		*  @brief
		*    Set the optional shown application name
		*
		*  @param[in] shownApplicationName
		*    The new optional shown UTF-8 application name (example: "QSF sample")
		*/
		inline void setShownApplicationName(const std::string& shownApplicationName);

		/**
		*  @brief
		*    Return the application version
		*
		*  @return
		*    The ASCII application version (example: "42.21.0")
		*/
		inline const std::string& getApplicationVersion() const;

		/**
		*  @brief
		*    Set the application version
		*
		*  @param[in] applicationVersion
		*    The new ASCII application version (example: "42.21.0")
		*/
		inline void setApplicationVersion(const std::string& applicationVersion);

		/**
		*  @brief
		*    Return the list of application tags
		*
		*  @return
		*    The list of ASCII application tags (example: {"sample","sample_game"} )
		*/
		inline const std::vector<NamedIdentifier>& getApplicationTags() const;

		/**
		*  @brief
		*    Return the list of application tags a a single string
		*
		*  @return
		*    The ASCII application tags as a space-separated string (example: "sample sample_game")
		*/
		std::string getApplicationTagsAsString() const;

		/**
		*  @brief
		*    Set the application tags
		*
		*  @param[in] applicationTags
		*    The ASCII application tags as a space-separated string (example: "sample sample_game")
		*/
		void setApplicationTagsAsString(const std::string& applicationTags);

		/**
		*  @brief
		*    Return the organization name
		*
		*  @return
		*    The UTF-8 organization name (example: "Promotion Software GmbH")
		*/
		inline const std::string& getOrganizationName() const;

		/**
		*  @brief
		*    Set the organization name
		*
		*  @param[in] organizationName
		*    The new UTF-8 organization name (example: "Promotion Software GmbH")
		*/
		inline void setOrganizationName(const std::string& organizationName);

		/**
		*  @brief
		*    Return the organization domain
		*
		*  @return
		*    The UTF-8 organization domain (example: "promotion-software.de")
		*/
		inline const std::string& getOrganizationDomain() const;

		/**
		*  @brief
		*    Set the organization domain
		*
		*  @param[in] organizationDomain
		*    The new UTF-8 organization domain (example: "promotion-software.de")
		*/
		inline void setOrganizationDomain(const std::string& organizationDomain);

		/**
		*  @brief
		*    Return the splash screen
		*
		*  @return
		*    The UTF-8 filename in platform-independent notation of the splash screen to show when starting the application in order to provide the user with an instant visual feedback
		*/
		inline const std::string& getSplashScreen() const;

		/**
		*  @brief
		*    Set the splash screen
		*
		*  @param[in] splashScreen
		*    The new UTF-8 filename in platform-independent notation of the splash screen to show when starting the application in order to provide the user with an instant visual feedback
		*/
		inline void setSplashScreen(const std::string& splashScreen);

		/**
		*  @brief
		*    Return if the splash screen should show info texts
		*
		*  @return
		*    "true" if info texts should be shown
		*/
		inline bool getSplashScreenInfoTexts() const;

		/**
		*  @brief
		*    Enable or disable splash screen info texts
		*
		*  @param[in] show
		*    "true" if info texts should be shown
		*/
		inline void setSplashScreenInfoTexts(bool show);

		/**
		*  @brief
		*    Return the plugins
		*
		*  @return
		*    The list of UTF-8 QSF plugin names (example: "editor", "sample", "sample_editor"), plugin filenames are automatically derived from those names (example: "editor.dll", "sample.dll", "sample_editor.dll")
		*
		*  @see
		*    - "qsf::ApplicationContext::setPlugins()"
		*/
		inline const std::vector<std::string>& getPlugins() const;

		/**
		*  @brief
		*    Return the plugins as string
		*
		*  @return
		*    UTF-8 QSF plugin names (example: "editor sample sample_editor"), plugin filenames are automatically derived from those names (example: "editor.dll", "sample.dll", "sample_editor.dll")
		*
		*  @see
		*    - "qsf::ApplicationContext::setPlugins()"
		*/
		std::string getPluginsAsString() const;

		/**
		*  @brief
		*    Set the plugins
		*
		*  @param[in] plugins
		*    The list of UTF-8 QSF plugin names (example: "editor", "sample", "sample_editor"), plugin filenames are automatically derived from those names (example: "editor.dll", "sample.dll", "sample_editor.dll")
		*
		*  @note
		*    - In case of debug build, "qsf::PluginSystem::DEBUG_POSTFIX" will be appended to the plugin name, meaning e.g. "sample.dll" will become "sampled.dll" in debug build
		*/
		inline void setPlugins(const std::vector<std::string>& plugins);

		/**
		*  @brief
		*    Return the projects
		*
		*  @return
		*    The list of UTF-8 QSF project names (example: "qsf", "sample"), the listed projects will be automatically loaded in the given order on ProjectManager initialization
		*
		*  @see
		*    - "qsf::ApplicationContext::setProjects()"
		*/
		inline const std::vector<std::string>& getProjects() const;

		/**
		*  @brief
		*    Return the projects as string
		*
		*  @return
		*    UTF-8 QSF project names (example: "qsf", "sample"), the listed projects will be automatically loaded in the given order on ProjectManager initialization
		*
		*  @see
		*    - "qsf::ApplicationContext::setProjects()"
		*/
		std::string getProjectsAsString() const;

		/**
		*  @brief
		*    Set the projects
		*
		*  @param[in] plugins
		*    The list of UTF-8 QSF project names (example: "qsf", "sample"), the listed projects will be automatically loaded in the given order on ProjectManager initialization
		*/
		inline void setProjects(const std::vector<std::string>& projects);

		/**
		*  @brief
		*    Return the "current project"
		*
		*  @return
		*    The UTF-8 QSF project name of the "current project" (example: "sample")
		*
		*  @see
		*    - "qsf::ApplicationContext::setCurrentProject()"
		*/
		inline const std::string& getCurrentProject() const;

		/**
		*  @brief
		*    Set the projects
		*
		*  @param[in] plugins
		*    The UTF-8 QSF project name of the "current project" (example: "sample")
		*/
		inline void setCurrentProject(const std::string& currentProject);

		/**
		*  @brief
		*    Return the user directory name or path
		*
		*  @return
		*    The UTF-8 user directory name inside the user data directory (example: "QSF sample")
		*
		*  @see
		*    - "qsf::ApplicationContext::getApplicationUserDataDirectory()"
		*/
		inline const std::string& getUserDirectory() const;

		/**
		*  @brief
		*    Set the user directory name or path
		*
		*  @param[in] userDirectory
		*    The UTF-8 user directory name inside the user data directory (example: "QSF sample")
		*/
		inline void setUserDirectory(const std::string& userDirectory);

		/**
		*  @brief
		*    Return the cache directory name or path
		*
		*  @return
		*    The UTF-8 cache directory name inside the user data directory (example: "QSF sample/cache")
		*
		*  @see
		*    - "qsf::ApplicationContext::getApplicationCacheDirectory()"
		*/
		inline const std::string& getCacheDirectory() const;

		/**
		*  @brief
		*    Set the cache directory name or path
		*
		*  @param[in] cacheDirectory
		*    The UTF-8 cache directory name inside the user data directory (example: "QSF sample/cache")
		*/
		inline void setCacheDirectory(const std::string& cacheDirectory);

		/**
		*  @brief
		*    Return the settings prefix
		*
		*  @return
		*    The UTF-8 settings prefix (example: "editor")
		*/
		inline const std::string& getSettingsPrefix() const;

		/**
		*  @brief
		*    Set the settings prefix
		*
		*  @param[in] settingsPrefix
		*    The UTF-8 settings prefix (example: "editor")
		*/
		inline void setSettingsPrefix(const std::string& settingsPrefix);

		/**
		*  @brief
		*    Return the application class name
		*
		*  @return
		*    The CAMP reflection ASCII class name of the application to run (example: "sample::Application")
		*/
		inline const std::string& getApplicationClassName() const;

		/**
		*  @brief
		*    Set the application class name
		*
		*  @param[in] applicationClassName
		*    The new CAMP reflection ASCII class name of the application to run (example: "sample::Application")
		*/
		inline void setApplicationClassName(const std::string& applicationClassName);

		/**
		*  @brief
		*    Get the application parameters
		*
		*  @return
		*    The application parameters as a generic Boost property tree; its structure is defined by the application
		*/
		inline const boost::property_tree::ptree& getApplicationParameters() const;

		/**
		*  @brief
		*    Get the application parameters
		*
		*  @param[in] parametersPTree
		*    The application parameters as a generic Boost property tree; its structure is defined by the application
		*/
		inline void setApplicationParameters(const boost::property_tree::ptree& parametersPTree);

		//[-------------------------------------------------------]
		//[ Load, Save, Copy                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the ApplicationContext .app filename
		*
		*  @return
		*    The filename that was passed to loadByFilename() or saveByFilename() (whichever was called last)
		*/
		inline const std::string& getFilename() const;

		/**
		*  @brief
		*    Load application context configuration by using a given filename
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the application context configuration to load
		*
		*  @return
		*    "true" if all went fine, else "false" (file not found?)
		*/
		bool loadByFilename(const std::string& filename);

		/**
		*  @brief
		*    Save the application context configuration by using a given filename
		*
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the application context configuration to save
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool saveByFilename(const std::string& filename);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ApplicationContext methods        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the user dependent application data directory
		*
		*  @return
		*    The user dependent UTF-8 application data directory in platform-independent notation (example: "C:/Users/Bob/Promotion Software GmbH/QSF Sample")
		*
		*  @remarks
		*    Examples on different systems:
		*    - Windows 7:  C:/Users/<UserName>/AppData/Roaming/<organization domain>/<user directory>
		*    - Windows XP: C:/Documents and Settings/<Username>/<organization domain>/<user directory>
		*    - Linux:      /home/<username>/<organization domain>/<user directory>
		*/
		virtual std::string getApplicationDataUserDirectory() const;

		/**
		*  @brief
		*    Return the user dependent cache directory
		*
		*  @return
		*    The user dependent UTF-8 cache directory in platform-independent notation (example: "C:/Users/Bob/Promotion Software GmbH/QSF Sample/cache")
		*
		*  @remarks
		*    Examples on different systems:
		*    - Windows 7:  C:/Users/<UserName>/AppData/Roaming/<organization domain>/<cache directory>
		*    - Windows XP: C:/Documents and Settings/<Username>/<organization domain>/<cache directory>
		*    - Linux:      /home/<username>/<organization domain>/<cache directory>
		*    Note that <cache directory> may consist of a sequence of directories itself
		*/
		virtual std::string getApplicationCacheDirectory() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string					 mApplicationName;			///< UTF-8 application name (example: "QSF sample")
		std::string					 mShownApplicationName;		///< Optional shown UTF-8 application name (example: "QSF sample")
		std::string					 mApplicationVersion;		///< ASCII application version (example: "42.21.0")
		std::vector<NamedIdentifier> mApplicationTags;			///< List of ASCII application tags
		std::string					 mOrganizationName;			///< UTF-8 organization name (example: "Promotion Software GmbH")
		std::string					 mOrganizationDomain;		///< UTF-8 organization domain (example: "promotion-software.de")
		std::string					 mSplashScreen;				///< UTF-8 filename in platform-independent notation of the splash screen to show when starting the application in order to provide the user with an instant visual feedback
		bool						 mSplashScreenInfoTexts;	///< "true" if info texts should be shown inside the splash screen
		std::vector<std::string>	 mPlugins;					///< List of UTF-8 QSF plugin names (example: "editor", "sample", "sample_editor"), plugin filenames are automatically derived from those names (example: "editor.dll", "sample.dll", "sample_editor.dll")
		std::vector<std::string>	 mProjects;					///< List of UTF-8 QSF project names (example: "qsf", "sample"), the listed projects will be automatically loaded in the given order on ProjectManager initialization
		std::string					 mCurrentProject;			///< UTF-8 QSF project name of the "current project" (example: "sample")
		std::string					 mFilename;					///< UTF-8 filename of the .app-file, this application context was loaded from
		std::string					 mUserDirectory;			///< UTF-8 user directory name (example: "QSF sample"), see "getApplicationDataUserDirectory()"
		std::string					 mCacheDirectory;			///< UTF-8 cache directory name (example: "QSF sample")
		std::string					 mSettingsPrefix;			///< UTF-8 settings prefix (example: "editor")
		std::string					 mApplicationClassName;		///< CAMP reflection ASCII class name of the application to run (example: "sample::Application")
		boost::property_tree::ptree  mApplicationParameters;	///< Generic property tree holding parameters to be interpreted by the application


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/application/ApplicationContext-inl.h"

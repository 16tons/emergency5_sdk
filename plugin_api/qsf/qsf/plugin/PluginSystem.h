// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"

#include <string>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Plugin;
}


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
	*    Plugin system class
	*
	*  @todo
	*    - TODO(co) Set plugin filename of plugins not loaded dynamically via "qsf::PluginSystem::loadPlugin()"
	*/
	class QSF_API_EXPORT PluginSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const char* DEBUG_POSTFIX;	///< "d" UTF-8 QSF debug postfix, used e.g. when loading debug versions of shared libraries


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline PluginSystem();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PluginSystem();

		/**
		*  @brief
		*    Return the list of currently loaded plugins
		*
		*  @return
		*    Reference to the internal list of currently loaded plugins, do not manipulate the list or destroy the plugins
		*/
		inline const std::vector<Plugin*>& getPlugins() const;

		/**
		*  @brief
		*    Add a plugin
		*
		*  @param[in] plugin
		*    Reference to the plugin instance to add to this plugin system
		*
		*  @remarks
		*    The plugin system will take over the control of the provided plugin instance and will destroy it in case it's
		*    no longer required. The plugin system will also handle the plugin lifecycle by calling "onInstall()", "onStartup()",
		*    "onShutdown()" and "onUninstall()" at the appropriate time.
		*/
		void addPlugin(Plugin& plugin);

		/**
		*  @brief
		*    Load a plugin
		*
		*  @param[in] pluginName
		*    ASCII plugin name (e.g. "sample")
		*  @param[in] filename
		*    UTF-8 filename in platform-independent notation of the plugin to load in (e.g. "sample.dll")
		*
		*  @return
		*    "true" if all went fine or the plugin is already loaded, else "false"
		*
		*  @remarks
		*    Inside the plugin file, the plugin function entry point is named "CreatePluginInstance_<plugin name>()" (e.g. "CreatePluginInstance_sample()" if the plugin name is "sample").
		*    Sadly, we can't give each plugin the same plugin function entry point name. While this would work on MS Windows, it would not work on Mac OS X due to the dylib behaviour there:
		*    https://developer.apple.com/library/mac/documentation/Darwin/Reference/ManPages/man3/dlsym.3.html
		*      "If dlsym() is called with a handle, returned by dlopen() then only that image and any libraries it depends on are searched for symbol."
		*/
		bool loadPlugin(const std::string& pluginName, const std::string& filename);

		/**
		*  @brief
		*    Unload all plugins
		*
		*  @note
		*    - The plugins are unloaded in reversed order in case there's a dependency between plugins
		*      (which should be avoided whenever possible!)
		*/
		void unloadAllPlugins();

		/**
		*  @brief
		*    Add a search directory to look for plugin dependencies
		*
		*  @param[in] path
		*    UTF-8 absolute path in platform-independent notation of the directory to search in (e.g. "D:/sample_project/bin")
		*/
		void addPluginSearchDirectory(const std::string& path);

		/**
		*  @brief
		*    Get the platform and build configuration depended plugin filename
		*
		*  @param[in] pluginName
		*    UTF-8 QSF plugin name (example: "editor", "sample", "sample_editor"), plugin filenames are automatically derived from those names (example: "editor.dll", "sample.dll", "sample_editor.dll")
		*
		*  @return
		*    The UTF-8 plugin filename (example when using "editor" as plugin name: "editor.dll" for MS Windows release, "editord.dll" for MS Windows debug, "libeditor.so" for Linux release, "libeditord.so" for Linug debug)
		*/
		std::string getSharedLibraryFilename(const std::string& pluginName) const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup the plugin system
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Create the QSF core plugin ("qsf::QsfPlugin()") instance and add it to this plugin system
		*/
		virtual bool onStartup(bool serverMode) override;

		/**
		*  @brief
		*    Shutdown the plugin system
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Unload all plugins
		*
		*  @note
		*    - This method is only called case "onStartup()" returned successfully
		*/
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool			     mRunning;	///< "true" if the plugin system is up-and-running, else "false"
		std::vector<Plugin*> mPlugins;	///< The currently loaded plugins, we're responsible for destroying the instances in case we no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/plugin/PluginSystem-inl.h"

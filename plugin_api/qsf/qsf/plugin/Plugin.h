// Copyright (C) 2012-2018 Promotion Software GmbH


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
#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Class;
}
namespace qsf
{
	class PluginVersionBase;
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
	*    Abstract plugin class
	*
	*  @remarks
	*    Plugin lifecycle:
	*    - "onInstall()"
	*    - "onStartup()"
	*    - "onShutdown()"
	*    - "onUninstall()"
	*
	*  @note
	*    - Keep the constructor/destructor implementation as well as "onInstall()"/"onUninstall()" lightweight and do the heavy work within "onStartup()"/"onShutdown()"
	*/
	class QSF_API_EXPORT Plugin : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class PluginSystem;	// Only the plugin system is allowed to create and manage plugin instances
		friend class Metaclasses;	// CAMP metaclass registration might be inside separate classes


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the filename of the file this plugin is in
		*
		*  @return
		*    Absolute UTF-8 filename in platform-independent notation of the file (executable or shared library) this plugin is in, do no destroy the returned instance
		*/
		inline const std::string& getFilename() const;

		/**
		*  @brief
		*    Return the list of CAMP metaclasses registered by this plugin
		*
		*  @return
		*    Reference to the internal list of CAMP metaclasses registered by this plugin, do not manipulate the list or destroy the plugins
		*/
		inline const std::vector<const camp::Class*>& getCampClasses() const;

		/**
		*  @brief
		*    Return plugin version information
		*
		*  @brief
		*    The plugin version class instance, do no destroy the returned instance
		*/
		inline const PluginVersionBase& getPluginVersion() const;

		/**
		*  @brief
		*    Return the version string of the plugin
		*
		*  @return
		*    The null terminated ASCII version string of the plugin, never a null pointer, do not free the returned memory
		*/
		std::string getPluginVersionString() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Plugin methods                    ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the name of the plugin
		*
		*  @return
		*    The null terminated ASCII name of the plugin, never a null pointer, do not free the returned memory
		*/
		virtual const char* getName() const = 0;

		/**
		*  @brief
		*    Return the vendor of the plugin
		*
		*  @return
		*    The null terminated ASCII vendor of the plugin, never a null pointer, do not free the returned memory
		*/
		virtual const char* getVendor() const = 0;

		/**
		*  @brief
		*    Return the description of the plugin
		*
		*  @return
		*    The null terminated ASCII description of the plugin, never a null pointer, do not free the returned memory
		*/
		virtual const char* getDescription() const = 0;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Plugin methods                 ]
	//[-------------------------------------------------------]
	protected:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Install the plugin
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    Keep the installation implementation lightweight. For example register
		*    your CAMP metaclasses in here or create instances of new system components.
		*
		*  @note
		*    - Register plugin systems in here
		*/
		virtual bool onInstall() = 0;

		/**
		*  @brief
		*    Startup the plugin
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @note
		*    - Only called in case "onInstall()" returned successfully
		*    - Startup plugin systems in here
		*    - Not called if QSF runs in server mode
		*/
		virtual bool onStartup() = 0;

		/**
		*  @brief
		*    Shutdown the plugin
		*
		*  @note
		*    - Only called in case "onStartup()" was called and returned successfully
		*    - Shutdown plugin systems in here
		*/
		virtual void onShutdown() = 0;

		/**
		*  @brief
		*    Uninstall the plugin
		*
		*  @remarks
		*    Keep the installation implementation lightweight. For example unregister
		*    your CAMP metaclasses in here or destroy instances of created system components.
		*
		*  @note
		*    - Only called in case "onInstall()" returned successfully
		*    - Unregister plugin systems in here
		*/
		virtual void onUninstall() = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] pluginVersion plugin version object, qsf::Plugin takes ownership and takes care of destruction
		*/
		inline Plugin(PluginVersionBase* pluginVersion);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Plugin();

		/**
		*  @brief
		*    Add a CAMP class registered by this plugin
		*
		*  @param[in] campClass
		*    CAMP class registered by this plugin, must stay valid as long as this plugin instance exists
		*/
		void addCampClass(const camp::Class& campClass);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::string						mFilename;		///< UTF-8 filename in platform-independent notation of the file (executable or shared library) this plugin is in, set by the plugin system
		std::vector<const camp::Class*> mCampClasses;	///< List of CAMP metaclasses registered by this plugin, do not destroy the CAMP class instances
		PluginVersionBase*				mPluginVersion;	///< Plugin version information, always valid, created and destroyed by child class implementation constructors/destructors

	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/plugin/Plugin-inl.h"

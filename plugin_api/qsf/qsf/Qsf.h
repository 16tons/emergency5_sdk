// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/plugin/SystemContainer.h"

#include <stack>
#include <string>
#include <memory>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace boost
{
	namespace property_tree
	{
		template <typename U, typename V, typename W>
		class basic_ptree;
		typedef basic_ptree<std::string, std::string, std::less<std::string> > ptree;
	}
}
namespace qsf
{
	class ApplicationContext;
	class PrototypeManager;
	class Map;
	class Application;
	class System;
	class WorkerSystem;
	class PlatformSystem;
	class FileSystem;
	class JobSystem;
	class LogSystem;
	class LocalizationSystem;
	class ClassSystem;
	class PluginSystem;
	class AssetSystem;
	class WindowSystem;
	class ScriptSystem;
	class RendererSystem;
	class PhysicsSystem;
	class InputSystem;
	class AudioSystem;
	class NetworkSystem;
	class MessageSystem;
	class ComponentSystem;
	class PrototypeSystem;
	class LinkSystem;
	class MapSystem;
	class GuiSystem;
	class ActionExecutionSystem;
	class SplashScreen;
	class DebugDrawManager;
	class SettingsGroupManager;
	class DebugGroupManager;
	#ifdef QSF_PROFILING
		class ProfilingManager;
	#endif
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
	*    QSF class
	*
	*  @remarks
	*    This is the central part of the Quadriga Simulation Framework (QSF). It's purpose is
	*    to startup and administer all systems (renderer, audio etc.) as well as running
	*    an QSF application.
	*
	*    QSF lifecycle:
	*    - "startup()"
	*    - "run()"
	*    - "shutdown()"
	*
	*    The most basic usage looks like the following:
	*    @code
	*    // Prepare the application context
	*    qsf::ApplicationContext applicationContext;
	*    applicationContext.setApplicationName("QSF Sample");
	*    applicationContext.setApplicationVersion("0.0.1");
	*    applicationContext.setOrganizationName("Promotion Software GmbH");
	*    applicationContext.setOrganizationDomain("promotion-software.de");
	*    applicationContext.setSplashScreen("sample.bmp");
	*    { // Plugins
	*        std::vector<std::string> plugins;
	*        plugins.push_back("editor");
	*        plugins.push_back("sample");
	*        plugins.push_back("sample_editor");
	*        applicationContext.setPlugins(plugins);
	*    }
	*    applicationContext.setApplicationClassName("sample::Application");
	*
	*    // Feed the QSF instance with the application context
	*    qsf::Qsf qsf(applicationContext);
	*    if (qsf.startup())
	*    {
	*        qsf.run(applicationContext.getApplicationClassName(), std::vector<std::string>());
	*        qsf.shutdown();
	*    }
	*    @endcode
	*
	*    Instead of writing "qsf::Qsf::instance()->getAudioSystem()" to access the audio system,
	*    you can also use the ease of use macro "QSF_AUDIO". The following QSF macros exist inside the "qsf/QsfHelper.h" header:
	*    "QSF_PLATFORM", "QSF_FILE", "QSF_LOG", "QSF_LOCALIZATION", "QSF_JOB", "QSF_CLASS", "QSF_PLUGIN", "QSF_ASSET",
	*    "QSF_SCRIPT", "QSF_RENDERER", "QSF_PARTICLE", "QSF_PHYSICS", "QSF_INPUT", "QSF_AUDIO", "QSF_NETWORK",
	*    "QSF_COMPONENT", "QSF_PROTOTYPE", "QSF_MAP", "QSF_GUI", "QSF_WORKER" and "QSF_APPLICATION"
	*
	*  @note
	*    - There should only exist one QSF instance at one and the same time
	*/
	class QSF_API_EXPORT Qsf : public SystemContainer
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the global QSF instance
		*
		*  @return
		*    The global QSF instance, null pointer in case there's no such global instance, do no destroy the returned instance
		*
		*  @remarks
		*    This QSF class plays a central role and therefore we decided to make it to a singleton. The approach is the same
		*    as the one used by Qt's "QCoreApplication::instance()".
		*/
		static Qsf* instance();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] applicationContext
		*    Application context the QSF instance should use
		*
		*  @remarks
		*    - QSF takes ownership of the given applicationContext
		*/
		explicit Qsf(ApplicationContext& applicationContext);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~Qsf();

		/**
		*  @brief
		*    Return the currently used application context
		*
		*  @return
		*    The currently used application context, do no destroy the returned instance
		*/
		inline ApplicationContext& getApplicationContext();

		/**
		*  @brief
		*    Return the currently used application context
		*
		*  @return
		*    The currently used application context, do no destroy the returned instance
		*/
		inline const ApplicationContext& getApplicationContext() const;

		/**
		*  @brief
		*    Switch to another application context
		*
		*  @param[in] applicationContext
		*    Application context the QSF instance should switch to
		*  @param[in] returnToApplicationContext
		*    Return to the current application context when done with the application we should now switch to?
		*
		*  @remarks
		*    - Qsf takes ownership of the given application context
		*/
		void switchApplicationContext(ApplicationContext& applicationContext, bool returnToApplicationContext);

		/**
		*  @brief
		*    Return whether or not the application context stack is currently empty
		*
		*  @return
		*    "true" if the application context stack is currently empty, else "false"
		*
		*  @note
		*    - If an application gets shutdown, this method can be used to figure out whether or not it's just an application switch or a real application termination
		*/
		inline bool isApplicationContextStackEmpty() const;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Startup QSF
		*
		*  @param[in] serverMode
		*    "true" if the system should start in server mode, else "false"
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Startup the worker system ("qsf::WorkerSystem::onStartup()")
		*    - Startup the platform system ("qsf::PlatformSystem::onStartup()")
		*    - Startup the file system ("qsf::FileSystem::onStartup()")
		*    - Startup the log system ("qsf::LogSystem::onStartup()")
		*    - Startup the localization system ("qsf::LocalizationSystem::onStartup()")
		*    - Startup the job system ("qsf::JobSystem::onStartup()")
		*    - Startup the class system ("qsf::ClassSystem::onStartup()")
		*    - Startup the plugin system ("qsf::PluginSystem::onStartup()")
		*    - Startup the asset system ("qsf::AssetSystem::onStartup()")
		*    - Startup the window system ("qsf::WindowSystem::onStartup()")
		*    - Startup the script system ("qsf::ScriptSystem::onStartup()")
		*    - Startup the renderer system ("qsf::RendererSystem::onStartup()")
		*    - Startup the physics system ("qsf::PhysicsSystem::onStartup()")
		*    - Startup the input system ("qsf::InputSystem::onStartup()")
		*    - Startup the audio system ("qsf::AudioSystem::onStartup()")
		*    - Startup the network system ("qsf::NetworkSystem::onStartup()")
		*    - Startup the message system ("qsf::MessageSystem::onStartup()")
		*    - Startup the component system ("qsf::ComponentSystem::onStartup()")
		*    - Startup the link system ("qsf::LinkSystem::onStartup()")
		*    - Startup the prototype system ("qsf::PrototypeSystem::onStartup()")
		*    - Startup the map system ("qsf::MapSystem::onStartup()")
		*    - Startup the GUI system ("qsf::GuiSystem::onStartup()")
		*    - Startup the action execution system ("qsf::ActionExecutionSystem::onStartup()")
		*    - Load application context plugins (set via "qsf::ApplicationContext::setPlugins()")
		*
		*  @note
		*    - Server mode means that e.g. there's no need for audio & visual stuff because there's no display or sound hardware in the first place
		*/
		bool startup(bool serverMode = false);

		/**
		*  @brief
		*    Run an QSF application
		*
		*  @param[in] applicationClassName
		*    CAMP reflection ASCII class name of the application to run (e.g. "sample::Application")
		*  @param[in] programOptions
		*    List of UTF-8 program options, the first entry is always the absolute executable filename
		*  @param[in] parametersPTree
		*    Optional application-specific parameters as a generic Boost property tree
		*
		*  @note
		*    - Do only call this method in case "startup()" returned successfully
		*    - In case there's a next application context the current context (see "qsf::Qsf::getApplicationContext()")
		*      will be switched to this new application context and the next application context will be reset to a null pointer
		*/
		void run(const std::string& applicationClassName, const std::vector<std::string>& programOptions, const boost::property_tree::ptree* parametersPTree = nullptr);

		/**
		*  @brief
		*    Return the running application instance
		*
		*  @return
		*    Pointer to the running application instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - The application instance pointer is set and also unset within the "run()"-method
		*/
		inline Application* getApplication() const;

		/**
		*  @brief
		*    Shutdown QSF
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Clear map ("qsf::MapSystem::clear()" followed by "qsf::MapSystem::performGarbageCollection()")
		*    - Unload all plugins ("qsf::PluginSystem::unloadAllPlugins()")
		*    - Shutdown the action execution system ("qsf::ActionExecutionSystem::onShutdown()")
		*    - Shutdown the GUI system ("qsf::GuiSystem::onShutdown()")
		*    - Shutdown the map system ("qsf::MapSystem::onShutdown()")
		*    - Shutdown the prototype system ("qsf::PrototypeSystem::onShutdown()")
		*    - Shutdown the link system ("qsf::LinkSystem::onShutdown()")
		*    - Shutdown the component system ("qsf::ComponentSystem::onShutdown()")
		*    - Shutdown the message system ("qsf::MessageSystem::onShutdown()")
		*    - Shutdown the network system ("qsf::NetworkSystem::onShutdown()")
		*    - Shutdown the audio system ("qsf::AudioSystem::onShutdown()")
		*    - Shutdown the input system ("qsf::InputSystem::onShutdown()")
		*    - Shutdown the physics system ("qsf::PhysicsSystem::onShutdown()")
		*    - Shutdown the renderer system ("qsf::RendererSystem::onShutdown()")
		*    - Shutdown the script system ("qsf::ScriptSystem::onShutdown()")
		*    - Shutdown the window system ("qsf::WindowSystem::onShutdown()")
		*    - Shutdown the asset system ("qsf::AssetSystem::onShutdown()")
		*    - Shutdown the plugin system ("qsf::PluginSystem::onShutdown()")
		*    - Shutdown the class system ("qsf::ClassSystem::onShutdown()")
		*    - Shutdown the job system ("qsf::JobSystem::onShutdown()")
		*    - Shutdown the localization system ("qsf::LocalizationSystem::onShutdown()")
		*    - Shutdown the log system ("qsf::LogSystem::onShutdown()")
		*    - Shutdown the file system ("qsf::FileSystem::onShutdown()")
		*    - Shutdown the platform system ("qsf::PlatformSystem::onShutdown()")
		*    - Shutdown the worker system ("qsf::WorkerSystem::onStartup()")
		*    - Destroy the project manager instance, in case there's one
		*    - Destroy the debug group manager instance, in case there's one
		*    - Destroy the settings group manager instance, in case there's one
		*    - Destroy the string hash manager instance
		*
		*  @note
		*    - Do only call this method in case "startup()" returned successfully
		*/
		void shutdown();

		//[-------------------------------------------------------]
		//[ System                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the worker system instance
		*
		*  @return
		*    The worker system instance, do no destroy the returned instance
		*/
		inline WorkerSystem& getWorkerSystem() const;

		/**
		*  @brief
		*    Return the platform system instance
		*
		*  @return
		*    The platform system instance, do no destroy the returned instance
		*/
		inline PlatformSystem& getPlatformSystem() const;

		/**
		*  @brief
		*    Return the file system instance
		*
		*  @return
		*    The file system instance, do no destroy the returned instance
		*/
		inline FileSystem& getFileSystem() const;

		/**
		*  @brief
		*    Return the log system instance
		*
		*  @return
		*    The log system instance, do no destroy the returned instance
		*/
		inline LogSystem& getLogSystem() const;

		/**
		*  @brief
		*    Return the localization system instance
		*
		*  @return
		*    The localization system instance, do no destroy the returned instance
		*/
		inline LocalizationSystem& getLocalizationSystem() const;

		/**
		*  @brief
		*    Return the job system instance
		*
		*  @return
		*    The job system instance, do no destroy the returned instance
		*/
		inline JobSystem& getJobSystem() const;

		/**
		*  @brief
		*    Return the class system instance
		*
		*  @return
		*    The class system instance, do no destroy the returned instance
		*/
		inline ClassSystem& getClassSystem() const;

		/**
		*  @brief
		*    Return the plugin system instance
		*
		*  @return
		*    The plugin system instance, do no destroy the returned instance
		*/
		inline PluginSystem& getPluginSystem() const;

		/**
		*  @brief
		*    Return the asset system instance
		*
		*  @return
		*    The asset system instance, do no destroy the returned instance
		*/
		inline AssetSystem& getAssetSystem() const;

		/**
		*  @brief
		*    Return the window system instance
		*
		*  @return
		*    The window system instance, do no destroy the returned instance
		*/
		inline WindowSystem& getWindowSystem() const;

		/**
		*  @brief
		*    Return the script system instance
		*
		*  @return
		*    The script system instance, do no destroy the returned instance
		*/
		inline ScriptSystem& getScriptSystem() const;

		/**
		*  @brief
		*    Return the renderer system instance
		*
		*  @return
		*    The renderer system instance, do no destroy the returned instance
		*/
		inline RendererSystem& getRendererSystem() const;

		/**
		*  @brief
		*    Return the physics system instance
		*
		*  @return
		*    The physics system instance, do no destroy the returned instance
		*/
		inline PhysicsSystem& getPhysicsSystem() const;

		/**
		*  @brief
		*    Return the input system instance
		*
		*  @return
		*    The input system instance, do no destroy the returned instance
		*/
		inline InputSystem& getInputSystem() const;

		/**
		*  @brief
		*    Return the audio system instance
		*
		*  @return
		*    The audio system instance, do no destroy the returned instance
		*/
		inline AudioSystem& getAudioSystem() const;

		/**
		*  @brief
		*    Return the network system instance
		*
		*  @return
		*    The network system instance, do no destroy the returned instance
		*/
		inline NetworkSystem& getNetworkSystem() const;

		/**
		*  @brief
		*    Return the message system instance
		*
		*  @return
		*    The message system instance, do no destroy the returned instance
		*/
		inline MessageSystem& getMessageSystem() const;

		/**
		*  @brief
		*    Return the component system instance
		*
		*  @return
		*    The component system instance, do no destroy the returned instance
		*/
		inline ComponentSystem& getComponentSystem() const;

		/**
		*  @brief
		*    Return the prototype system instance
		*
		*  @return
		*    The prototype system instance, do no destroy the returned instance
		*/
		inline PrototypeSystem& getPrototypeSystem() const;

		/**
		*  @brief
		*    Return the link system instance
		*
		*  @return
		*    The link system instance, do no destroy the returned instance
		*/
		inline LinkSystem& getLinkSystem() const;

		/**
		*  @brief
		*    Return the map system instance
		*
		*  @return
		*    The map system instance, do no destroy the returned instance
		*/
		inline MapSystem& getMapSystem() const;

		/**
		*  @brief
		*    Return the GUI system instance
		*
		*  @return
		*    The GUI system instance, do no destroy the returned instance
		*/
		inline GuiSystem& getGuiSystem() const;

		/**
		*  @brief
		*    Return the script action system instance
		*
		*  @return
		*    The script action system instance, do no destroy the returned instance
		*/
		inline ActionExecutionSystem& getActionExecutionSystem() const;

		//[-------------------------------------------------------]
		//[ Manager                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the settings group manager instance
		*
		*  @return
		*    The settings group manager instance, do no destroy the returned instance
		*
		*  @todo
		*    TODO(co) Find a better place for the settings group manager
		*/
		SettingsGroupManager& getSettingsGroupManager();

		/**
		*  @brief
		*    Return the debug group manager instance
		*
		*  @return
		*    The debug group manager instance, do no destroy the returned instance
		*
		*  @todo
		*    TODO(co) Find a better place for the debug group manager
		*/
		DebugGroupManager& getDebugGroupManager();

		#ifdef QSF_PROFILING
			/**
			*  @brief
			*    Return the profiling manager instance
			*
			*  @return
			*    The profiling manager instance, do no destroy the returned instance
			*/
			ProfilingManager& getProfilingManager();
		#endif

		//[-------------------------------------------------------]
		//[ Ease-of-use access                                    ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the main prototype manager instance
		*
		*  @return
		*    The main prototype manager instance, do no destroy the returned instance
		*
		*  @note
		*    - This ease-of-use method was added to make accessing the main prototype manager, which are a part of "qsf::PrototypeSystem", more comfortable
		*    - Please do not use this method if you can avoid it, use concrete prototype manager instances instead
		*/
		PrototypeManager& getMainPrototypeManager() const;

		/**
		*  @brief
		*    Return the main map instance
		*
		*  @return
		*    The main map instance, do no destroy the returned instance
		*
		*  @note
		*    - This ease-of-use method was added to make accessing the main map, which are a part of "qsf::MapSystem", more comfortable
		*    - Please do not use this method if you can avoid it, use concrete map instances instead
		*/
		Map& getMainMap() const;

		/**
		*  @brief
		*    Return the debug draw manager instance
		*
		*  @return
		*    The debug draw manager instance, do no destroy the returned instance
		*
		*  @note
		*    - This ease-of-use method was added to make committing debug draw requests, which are a part of "qsf::MapSystem", more comfortable
		*    - Please do not use this method if you can avoid it, use the debug draw manager of a concrete map instance instead
		*/
		DebugDrawManager& getDebugDrawManager() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::SystemContainer methods           ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getSystemContainerName() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::SystemContainer methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual void logSystemMessage(const std::string& message) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Run an QSF application, internal part
		*
		*  @param[in] applicationClassName
		*    CAMP reflection ASCII class name of the application to run (e.g. "sample::Application")
		*  @param[in] programOptions
		*    List of UTF-8 program options, the first entry is always the absolute executable filename
		*  @param[in] parametersPTree
		*    Application-specific parameters as a generic Boost property tree
		*/
		void runInternal(const std::string& applicationClassName, const std::vector<std::string>& programOptions, const boost::property_tree::ptree& parametersPTree);

		/**
		*  @brief
		*    Load application context plugins (set via "qsf::ApplicationContext::setPlugins()")
		*
		*  @param[in] splashScreen
		*    Showed splash screen instance
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool loadApplicationContextPlugins(SplashScreen& splashScreen);


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static Qsf* mGlobalQSFInstance;	///< The global QSF instance, null pointer in case there's no such global instance, do no destroy the returned instance


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::shared_ptr<ApplicationContext>				mApplicationContext;		///< Application context the QSF instance should use
		std::stack<std::shared_ptr<ApplicationContext>> mApplicationContextStack;	///< Stack of next application context instances the QSF instance should use
		Application*									mApplication;				///< Pointer to the running application instance, can be a null pointer
		// System instances
		WorkerSystem*			mWorkerSystem;			///< Worker system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		PlatformSystem*			mPlatformSystem;		///< Platform system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		FileSystem*				mFileSystem;			///< File system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		LogSystem*				mLogSystem;				///< Log system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		LocalizationSystem*		mLocalizationSystem;	///< Localization system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		JobSystem*				mJobSystem;				///< Job system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		ClassSystem*			mClassSystem;			///< Class system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		PluginSystem*			mPluginSystem;			///< Plugin system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		AssetSystem*			mAssetSystem;			///< Asset system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		WindowSystem*			mWindowSystem;			///< Window system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		ScriptSystem*			mScriptSystem;			///< Script system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		RendererSystem*			mRendererSystem;		///< Renderer system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		PhysicsSystem*			mPhysicsSystem;			///< Physics system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		InputSystem*			mInputSystem;			///< Input system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		AudioSystem*			mAudioSystem;			///< Audio system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		NetworkSystem*			mNetworkSystem;			///< Network system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		MessageSystem*			mMessageSystem;			///< Message system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		ComponentSystem*		mComponentSystem;		///< Component system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		PrototypeSystem*		mPrototypeSystem;		///< Prototype system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		LinkSystem*				mLinkSystem;			///< Link system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		MapSystem*				mMapSystem;				///< Map system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		GuiSystem*				mGuiSystem;				///< GUI system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		ActionExecutionSystem*	mActionExecutionSystem;	///< Action execution system instance, always valid, we're responsible for destroying the instance in case we no longer need it
		// Manager instances
		SettingsGroupManager*	mSettingsGroupManager;	///< Settings group manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		DebugGroupManager*		mDebugGroupManager;		///< Debug group manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		#ifdef QSF_PROFILING
			ProfilingManager*	mProfilingManager;		///< Profiling manager instance, always valid, we're responsible for destroying the instance in case we no longer need it
		#endif
		// Internal
		SplashScreen*			mSplashScreen;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/Qsf-inl.h"

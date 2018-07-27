// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/plugin/Plugin.h>
#include <qsf/plugin/SystemContainer.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class ModSystem;
	class FreeplaySystem;
	class HealthSystem;
	class FireSystem;
	class ActivitySystem;
}
namespace qsf
{
	class System;
	namespace game
	{
		class AchievementSystem;
	}
}


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
	*    EMERGENCY 5 plugin implementation
	*/
	class EM5_API_EXPORT Plugin : public qsf::Plugin, public qsf::SystemContainer
	{


	//[-------------------------------------------------------]
	//[ Qt definitions (MOC)                                  ]
	//[-------------------------------------------------------]
	// Empty fake Q_OBJECT macro to silence the lupdate Qt tool ("Class '<x>' lacks Q_OBJECT macro" warnings)
	#ifndef Q_OBJECT
		#define Q_OBJECT
		#define QSF_FAKE_Q_OBJECT
	#endif
	Q_OBJECT
	#ifdef QSF_FAKE_Q_OBJECT
		#undef Q_OBJECT
		#undef QSF_FAKE_Q_OBJECT
	#endif


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the global EMERGENCY 5 plugin instance
		*
		*  @return
		*    The global EMERGENCY 5 plugin instance, null pointer in case there's no such global instance, do no destroy the returned instance
		*
		*  @remarks
		*    This EMERGENCY 5 plugin class plays a central role and therefore we decided to make it to a singleton. The approach is the same
		*    as the one used by Qt's "QCoreApplication::instance()".
		*/
		inline static Plugin* instance();

		/**
		*  @brief
		*    Return the global EMERGENCY 5 plugin instance; throws an exception when it is not initialized
		*/
		inline static Plugin& getInstanceSafe();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		Plugin();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Plugin();

		//[-------------------------------------------------------]
		//[ System                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the mod system instance
		*
		*  @return
		*    The mod system instance, do no destroy the returned instance
		*/
		inline ModSystem& getModSystem() const;

		/**
		*  @brief
		*    Return the freeplay system instance
		*
		*  @return
		*    The freeplay system instance, do no destroy the returned instance
		*/
		inline FreeplaySystem& getFreeplaySystem() const;

		/**
		*  @brief
		*    Return the health system instance
		*
		*  @return
		*    The health system instance, do no destroy the returned instance
		*/
		inline HealthSystem& getHealthSystem() const;

		/**
		*  @brief
		*    Return the fire system instance
		*
		*  @return
		*    The fire system instance, do no destroy the returned instance
		*/
		inline FireSystem& getFireSystem() const;

		/**
		*  @brief
		*    Return the activity system instance
		*
		*  @return
		*    The activity system instance, do no destroy the returned instance
		*/
		inline ActivitySystem& getActivitySystem() const;

		/**
		*  @brief
		*    Return the achievement system instance
		*
		*  @return
		*    The achievement system instance, do no destroy the returned instance
		*/
		inline qsf::game::AchievementSystem& getAchievementSystem() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Plugin methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		inline virtual const char* getVendor() const override;
		inline virtual const char* getDescription() const override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::SystemContainer methods           ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getSystemContainerName() const override;


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
		*/
		virtual bool onInstall() override;

		/**
		*  @brief
		*    Startup EMERGENCY 5 plugin
		*
		*  @return
		*    "true" if all went fine, else "false"
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Startup the Freeplay system ("em5::FreeplaySystem::onStartup()")
		*    - Startup the Health system ("em5::HealthSystem::onStartup()")
		*    - Startup the Fire system ("em5::FireSystem::onStartup()")
		*    - Startup the Achievement system ("qsf::game::AchievementSystem::onStartup()")
		*/
		virtual bool onStartup() override;

		/**
		*  @brief
		*    Shutdown EMERGENCY 5 plugin
		*
		*  @remarks
		*    The implementation does the following tasks:
		*    - Shutdown the Fire system ("em5::FireSystem::onShutdown()")
		*    - Shutdown the Health system ("em5::HealthSystem::onShutdown()")
		*    - Shutdown the Freeplay system ("em5::FreeplaySystem::onShutdown()")
		*    - Shutdown the Achievement system ("qsf::game::AchievementSystem::onShutdown()")
		*
		*  @note
		*    - Do only call this method in case "startup()" returned successfully
		*/
		virtual void onShutdown() override;

		/**
		*  @brief
		*    Uninstall the plugin
		*
		*  @note
		*    - Only called in case "onInstall()" returned successfully
		*/
		virtual void onUninstall() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Install CAMP reflection system classes
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool installCampClasses();


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static Plugin* mGlobalPluginInstance;	///< The global EMERGENCY 5 plugin instance, null pointer in case there's no such global instance, do no destroy the returned instance


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// System instances
		ModSystem*			mModSystem;
		FreeplaySystem*		mFreeplaySystem;
		HealthSystem*		mHealthSystem;
		FireSystem*			mFireSystem;
		ActivitySystem*		mActivitySystem;
		qsf::game::AchievementSystem* mAchievementSystem;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/plugin/Plugin-inl.h"

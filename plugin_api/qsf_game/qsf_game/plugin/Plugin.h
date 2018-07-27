// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/plugin/Plugin.h>
#include <qsf/plugin/SystemContainer.h>
#include <qsf/base/System.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class CommandSystem;
		class WeatherSystem;
		class EquipmentSystem;
		class CharacterAnimationSystem;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF game plugin implementation
		*/
		class QSF_GAME_API_EXPORT Plugin : public qsf::Plugin, public SystemContainer
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
			*    Return the global QSF Game plugin instance
			*
			*  @return
			*    The global QSF Game plugin instance, null pointer in case there's no such global instance, do no destroy the returned instance
			*
			*  @remarks
			*    This QSF Game plugin class plays a central role and therefore we decided to make it to a singleton. The approach is the same
			*    as the one used by Qt's "QCoreApplication::instance()".
			*/
			inline static Plugin* instance();

			// Like instance() but throws an exception when it is not initialized
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
			*    Return the command system instance
			*
			*  @return
			*    The command system instance, do no destroy the returned instance
			*/
			inline CommandSystem& getCommandSystem() const;

			/**
			*  @brief
			*    Return the weather system instance
			*
			*  @return
			*    The weather system instance, do no destroy the returned instance
			*/
			inline WeatherSystem& getWeatherSystem() const;

			/**
			*  @brief
			*    Return the equipment system instance
			*
			*  @return
			*    The equipment system instance, do no destroy the returned instance
			*/
			inline EquipmentSystem& getEquipmentSystem() const;

			/**
			*  @brief
			*    Return the character animation system instance
			*
			*  @return
			*    The character animation system instance, do no destroy the returned instance
			*/
			inline CharacterAnimationSystem& getCharacterAnimationSystem() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Plugin methods                    ]
		//[-------------------------------------------------------]
		public:
			inline virtual const char* getName() const override;
			inline virtual const char* getVendor() const override;
			inline virtual const char* getDescription() const override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Plugin methods                 ]
		//[-------------------------------------------------------]
		protected:
			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onInstall() override;
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onUninstall() override;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::SystemContainer methods           ]
		//[-------------------------------------------------------]
		public:
			inline virtual const char* getSystemContainerName() const override;


		//[-------------------------------------------------------]
		//[ Private static data                                   ]
		//[-------------------------------------------------------]
		private:
			static Plugin* mGlobalPluginInstance;	///< The global QSF Game plugin instance, null pointer in case there's no such global instance, do no destroy the returned instance


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// System instances
			CommandSystem*	 mCommandSystem;
			WeatherSystem*	 mWeatherSystem;
			EquipmentSystem* mEquipmentSystem;
			CharacterAnimationSystem* mCharacterAnimationSystem;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/plugin/Plugin-inl.h"

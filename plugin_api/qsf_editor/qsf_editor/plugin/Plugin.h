// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/plugin/Plugin.h>

#include <QtCore/qobject.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinaryMapBackup;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF editor plugin implementation
		*/
		class Plugin : public QObject, public qsf::Plugin
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Return the global QSF editor plugin instance
			*
			*  @return
			*    The global QSF editor plugin instance, null pointer in case there's no such global instance, do no destroy the returned instance
			*
			*  @remarks
			*    This QSF editor plugin class plays a central role and therefore we decided to make it to a singleton. The approach is the same
			*    as the one used by Qt's "QCoreApplication::instance()".
			*/
			inline static Plugin* instance();

			/**
			*  @brief
			*    Return the global QSF editor plugin instance; throws an exception when it is not initialized
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
			inline virtual ~Plugin();

			/**
			*  @brief
			*    Get the map backup instance
			*/
			BinaryMapBackup& getMapBackup();


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
			static Plugin* mGlobalPluginInstance;	///< The global QSF editor plugin instance, null pointer in case there's no such global instance, do no destroy the instance
			BinaryMapBackup* mMapBackup;			///< Backup of the map when switching to another application


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/plugin/Plugin-inl.h"

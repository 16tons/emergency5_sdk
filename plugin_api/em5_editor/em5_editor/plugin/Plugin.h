// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/Export.h"

#include <qsf/plugin/Plugin.h>
#include <qsf/message/MessageProxy.h>

#include <QtCore/qobject.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    EMERGENCY 5 editor plugin implementation
		*/
		class EM5EDITOR_API_EXPORT Plugin : public QObject, public qsf::Plugin
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
			*    Return the global EMERGENCY 5 editor plugin instance
			*
			*  @return
			*    The global EMERGENCY 5 editor plugin instance, null pointer in case there's no such global instance, do no destroy the returned instance
			*
			*  @remarks
			*    This EMERGENCY 5 editor plugin class plays a central role and therefore we decided to make it to a singleton. The approach is the same
			*    as the one used by Qt's "QCoreApplication::instance()".
			*/
			inline static Plugin* instance();

			/**
			*  @brief
			*    Return the global EMERGENCY 5 editor plugin instance; throws an exception when it is not initialized
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
			void onPreNewEmptyMap(const qsf::MessageParameters& parameters);


		//[-------------------------------------------------------]
		//[ Private static data                                   ]
		//[-------------------------------------------------------]
		private:
			static Plugin* mGlobalPluginInstance;	///< The global EMERGENCY 5 editor plugin instance, null pointer in case there's no such global instance, do no destroy the returned instance


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			qsf::MessageProxy mOnPreNewEmptyMapMessageProxy;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5_editor/plugin/Plugin-inl.h"

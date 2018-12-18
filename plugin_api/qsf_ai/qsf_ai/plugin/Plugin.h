// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"

#include <qsf/base/System.h>
#include <qsf/plugin/Plugin.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Artificial Intelligence (AI) plugin implementation
		*/
		class QSF_AI_API_EXPORT Plugin : public qsf::Plugin
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
				*    Return the global AI plugin instance
				*
				*  @return
				*    The global AI plugin instance, null pointer in case there's no such global instance, do no destroy the returned instance
				*
				*  @remarks
				*    This AI plugin class plays a central role and therefore we decided to make it to a singleton. The approach is the same
				*    as the one used by Qt's "QCoreApplication::instance()".
				*/
				static Plugin* instance();

				// Like instance() but throws an exception when it is not initialized
				static Plugin& getInstanceSafe();


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
				/** This contains the AI systems that need a dedicated update call each tick.
				*
				*  @brief
				*    Return the list of systems
				*
				*  @return
				*    Reference to the internal list of systems, do not manipulate the list or destroy the systems
				*/
				inline const std::vector<System*>& getSystems() const;


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
			//[ Private static data                                   ]
			//[-------------------------------------------------------]
			private:
				static Plugin* mGlobalPluginInstance;	///< The global AI plugin instance, null pointer in case there's no such global instance, do no destroy the returned instance


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
			private:
				std::vector<System*> mSystems;	///< The systems, do not destroy the instances


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/plugin/Plugin-inl.h"

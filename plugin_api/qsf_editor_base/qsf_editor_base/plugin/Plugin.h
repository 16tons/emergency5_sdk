// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/plugin/Plugin.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    QSF editor base plugin implementation
			*/
			class QSF_EDITOR_BASE_API_EXPORT Plugin : public qsf::Plugin
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


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor_base/plugin/Plugin-inl.h"

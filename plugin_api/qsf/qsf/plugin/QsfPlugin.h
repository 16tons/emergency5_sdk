// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/plugin/Plugin.h"


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
	*    QSF plugin implementation
	*/
	class QSF_API_EXPORT QsfPlugin : public Plugin
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the global QSF plugin instance
		*
		*  @return
		*    The global QSF plugin instance, null pointer in case there's no such global instance, do no destroy the returned instance
		*
		*  @remarks
		*    This QSF plugin class plays a central role and therefore we decided to make it to a singleton. The approach is the same
		*    as the one used by Qt's "QCoreApplication::instance()".
		*/
		inline static QsfPlugin* instance();

		/**
		*  @brief
		*    Return the global QSF plugin instance; throws an exception when it is not initialized
		*/
		inline static QsfPlugin& getInstanceSafe();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		QsfPlugin();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~QsfPlugin();


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
		virtual bool onInstall() override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onUninstall() override;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static QsfPlugin* mGlobalPluginInstance;	///< The global QSF plugin instance, null pointer in case there's no such global instance, do no destroy the returned instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/plugin/QsfPlugin-inl.h"

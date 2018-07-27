// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "game_plugin/multiplayer/DataCacheItemFactoryListener.h"

#include <qsf/plugin/Plugin.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace user
{
	class GameListener;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    User-defined game plugin sample implementation
	*/
	class Plugin : public qsf::Plugin
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
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		GameListener*	mGameListener;		///< Listener to game relevant functions. Always valid, we're responsible for destroying the instance
		multiplayer::DataCacheItemFactoryListener mDataCacheItemFactoryListener;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "game_plugin/plugin/Plugin-inl.h"

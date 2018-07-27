// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/application/store/StoreManager.h"


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
		*    Store manager implementation that does nothing at all
		*/
		class StoreManagerNull : public StoreManager
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			StoreManagerNull();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~StoreManagerNull();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::game::StoreManager methods        ]
		//[-------------------------------------------------------]
		public:
			virtual void onStartup() override;
			virtual void onShutdown() override;

			// No need to override the other virtual methods, base class implementation will do


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf

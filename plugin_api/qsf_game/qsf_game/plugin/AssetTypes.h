// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/asset/type/AssetTypes.h>
#include <qsf/base/NamedIdentifier.h>


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
		*    Static QSF game plugin asset types collection class
		*/
		class QSF_GAME_API_EXPORT AssetTypes : public qsf::AssetTypes
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class Plugin;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// Weather
			static const NamedIdentifier WEATHER_STATE;				///< Asset type name "weather_state"
			static const NamedIdentifier WEATHER_CONFIGURATION;		///< Asset type name "weather_configuration"

			// Asset categories
			static const NamedIdentifier SCRIPT_CATEGORY_COMMAND;	///< Script asset category name "command"

			// Character animation controller
			static const NamedIdentifier CHARACTER_ANIMATION_SET;	///< Asset type name "character animation set"


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Default constructor
			*/
			AssetTypes();

			/**
			*  @brief
			*    Destructor
			*
			*  @brief
			*    - Not virtual by intent
			*/
			~AssetTypes();

			/**
			*  @brief
			*    Register the asset types
			*/
			static void registerAssetTypes();

			/**
			*  @brief
			*    Unregister the asset types
			*/
			static void unregisterAssetTypes();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf

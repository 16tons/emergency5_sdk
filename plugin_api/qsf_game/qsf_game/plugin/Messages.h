// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

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
		*    Static QSF game messages collection class
		*/
		class QSF_GAME_API_EXPORT Messages
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    "AchievementCompleted" message, emitted when an achievement was completed
			*
			*  @remarks
			*    - Filters:
			*        (1): The achievement ID, which is its hashed name
			*    - No parameters
			*    - No response
			*/
			static const NamedIdentifier QSF_GAME_ACHIEVEMENT_COMPLETED;

			/**
			*  @brief
			*    "AchievementUpdated" message, emitted when an achievement's progress was updated
			*
			*  @remarks
			*    - Filters:
			*        (1): The achievement ID, which is its hashed name
			*    - Parameters:
			*        "previousProgress": The previous progress
			*    - No response
			*/
			static const NamedIdentifier QSF_GAME_ACHIEVEMENT_UPDATED;

			/**
			*  @brief
			*    "ClickOnMiniMap" message, emitted when an achievement was completed
			*
			*  @remarks
			*    - No Filters
			*    - Parameters:
			*        "worldPosition": The position in map coordinates where the click on the minimap was done
			*    - Response:
			*        When a response exists the camera isn't moved to the clicked position
			*/
			static const NamedIdentifier QSF_GAME_ON_MINIMAP_CLICKED;

			/**
			*  @brief
			*    "EntityHidden" message, emitted when an hidden component is active
			*
			*  @remarks
			*    - Filters:
			*        (1): The entity ID, which has set hidden (uint64)
			*    - No parameters
			*    - No response
			*/
			static const NamedIdentifier QSF_GAME_ENTITY_HIDDEN;

			//[-------------------------------------------------------]
			//[ Command system                                        ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    "PostCommandSet" message, emitted after a new command has been set
			*
			*  @remarks
			*    - No filters
			*    - No parameters
			*    - No response
			*/
			static const qsf::NamedIdentifier QSF_GAME_POST_COMMAND_SET;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Default constructor
			*/
			Messages();

			/**
			*  @brief
			*    Destructor
			*
			*  @brief
			*    - Not virtual by intent
			*/
			~Messages();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf

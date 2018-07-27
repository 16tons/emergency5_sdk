// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"
#include "qsf_game/characteranimation/CharacterAnimationSetManager.h"

#include <qsf/base/System.h>
#include <qsf/reflection/CampClass.h>


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
		*    Character animation system
		*/
		// TODO(sw) Find a better name or put the manager of the character animation sets into another system
		class QSF_GAME_API_EXPORT CharacterAnimationSystem : public System
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline CharacterAnimationSystem();

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~CharacterAnimationSystem();

			inline CharacterAnimationSetManager& getCharacterAnimationSetManager();

			/**
			*  @brief
			*    Gets called on game startup
			*/
			void onGameStartup();

			/**
			*  @brief
			*    Gets called on game shutdown
			*/
			void onGameShutdown();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::System methods                    ]
		//[-------------------------------------------------------]
		public:
			inline virtual const char* getName() const override;
			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onStartup(bool serverMode) override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			CharacterAnimationSetManager	mCharacterAnimationSetManager;	///< Instance of the character animation set manager


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/characteranimation/CharacterAnimationSystem-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::CharacterAnimationSystem)

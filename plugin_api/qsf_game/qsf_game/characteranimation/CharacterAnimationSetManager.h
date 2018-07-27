// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"
#include "qsf_game/characteranimation/CharacterAnimationSet.h"
#include "qsf_game/characteranimation/CharacterAnimationDefinition.h"

#include <qsf/base/manager/Manager.h>

#include <stack>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class AssetSystem;
}


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
		*    Character animation set manager
		*/
		class QSF_GAME_API_EXPORT CharacterAnimationSetManager : public qsf::Manager
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef std::unordered_map<std::string, CharacterAnimationSet> CharacterAnimationSetMap;
			typedef std::unordered_map<std::string, CharacterAnimationDefinition> CharacterAnimationDefinitionMap;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			CharacterAnimationSetManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CharacterAnimationSetManager();

			/**
			*  @brief
			*    Gets an character animation set by its name
			*
			*  @note
			*    Do not destroy the returned instance
			*/
			CharacterAnimationSet* getCharacterAnimationSetByName(const std::string& animationSetName);

			const CharacterAnimationSetMap& getElements();

			const CharacterAnimationDefinition* getCharacterAnimationDefinition(const std::string& name) const;

			/**
			*  @brief
			*    Load the assets
			*/
			void loadAssets();

			/**
			*  @brief
			*    Startup of the manager
			*/
			void startup();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void onAssetsMounted(const qsf::Assets& assets);
			void onAssetsUnmounted(const qsf::Assets& assets);

			/**
			*  @brief
			*    Insert tree into the event pool
			*/
			void loadCharacterAnimationSet(const std::string& poolName, const boost::property_tree::ptree& poolPTree);
			void loadCharacterAnimationDefinition(const std::string& poolName, const boost::property_tree::ptree& poolPTree);

			/**
			*	@brief
			*	  Clear the animation sets
			*/
			void clearAnimationSets();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			CharacterAnimationSetMap		mCharacterAnimationSets;	///< Map of character animation sets (key: character animation set name)
			CharacterAnimationDefinitionMap	mCharacterAnimationDefinitions;	///< Map of character animation definitions (key: character animation definition name)
			qsf::AssetSystem&				mAssetSystem;				///< Asset system instance, do not destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf

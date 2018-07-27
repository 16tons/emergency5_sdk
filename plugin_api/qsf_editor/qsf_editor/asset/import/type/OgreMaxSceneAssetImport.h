// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/AssetImport.h"

#include <qsf/base/NamedIdentifier.h>

#include <boost/container/flat_set.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class PrototypeManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    OgreMax scene asset import class
		*
		*  @remarks
		*    OGRE OgreMax scene (".scene" text file) assumptions to simplify the import process:
		*    - "environment" information stored inside the scene file is ignored
		*    - On "node" has exactly one "entity"
		*    - The "entity" sub-entities information is not relevant, meaning no per-entity material overloading
		*/
		class QSF_EDITOR_API_EXPORT OgreMaxSceneAssetImport : public AssetImport
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef boost::container::flat_set<GlobalAssetId> DirectGlobalAssetIdDependencies;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] prototypeManager
			*    Prototype manager to fill with prototypes basing on the OgreMax scene entities
			*  @param[in] assetPackage
			*    Asset package instance new imported assets should be registered in, instance must stay valid as long as this asset import instance exists
			*  @param[in] ogreResourceGroup
			*    UTF-8 name of the OGRE resource group to use
			*/
			inline OgreMaxSceneAssetImport(PrototypeManager& prototypeManager, AssetPackage& assetPackage, const std::string& ogreResourceGroup);

			/**
			*  @brief
			*    Destructor
			*
			*  @note
			*    - Not virtual by default
			*/
			inline ~OgreMaxSceneAssetImport();

			inline const DirectGlobalAssetIdDependencies& getDirectGlobalAssetIdDependencies() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::base::AssetImport methods ]
		//[-------------------------------------------------------]
		public:
			virtual GlobalAssetId importByAbsoluteFilename(const std::string& absoluteFilename) const override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			PrototypeManager&						mPrototypeManager;	///< Prototype manager to fill with prototypes basing on the OgreMax scene entities
			std::string								mOgreResourceGroup;	///< UTF-8 name of the OGRE resource group to use
			mutable DirectGlobalAssetIdDependencies	mDirectGlobalAssetIdDependencies;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/import/type/OgreMaxSceneAssetImport-inl.h"

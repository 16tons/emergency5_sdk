// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/material/BaseMaterialPropertyListing.h"

#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/base/manager/Manager.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class MaterialManager;
	class MaterialBlueprint;
	namespace editor
	{
		class EditedMaterialInstance;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		typedef StringHash MaterialId;	///< The material ID is a result of "qsf::StringHash(<material name>)"


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		class MaterialEditorManager : public Manager
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const std::string BASE_MATERIAL_PROPERTY_LISTING_DIRECTORY;
			static const std::string BASE_MATERIAL_PROPERTY_LISTING_FILEEXTENSION;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			MaterialEditorManager();
			virtual ~MaterialEditorManager();

			//[-------------------------------------------------------]
			//[ Materials                                             ]
			//[-------------------------------------------------------]
			EditedMaterialInstance* cloneMaterial(const std::string& sourceName, const std::string& destinationName);
			void deleteMaterial(const std::string& name);
			EditedMaterialInstance* getMaterial(const std::string& name);
			void getMaterialNames(std::vector<std::string>& materialNames);
			std::vector<std::pair<uint32, std::string>> getAllBaseMaterialNames() const;
			void changeBaseMaterialForMaterial(const std::string& name, qsf::MaterialBlueprint* baseMaterial);

			//[-------------------------------------------------------]
			//[ General                                               ]
			//[-------------------------------------------------------]
			void getModifiedMaterialNames(std::vector<std::string>& materialNames);
			bool hasModifiedMaterials();
			void saveAll();
			void shutdownEditing(bool save);

			//[-------------------------------------------------------]
			//[ Internal                                              ]
			//[-------------------------------------------------------]
			const BaseMaterialPropertyListing* getBaseMaterialPropertyListing(MaterialId materialId) const;
			bool baseMaterialNeedsCompilation(const StringHash& name) const;
			bool doesMaterialFileNeedCompilation(const std::string& absoluteFilename) const;
			bool isMaterialFileCompiled(const std::string& absoluteFilename) const;
			bool buildLocalizedMaterial(GlobalAssetId globalAssetId, const std::string& localization) const;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			MaterialManager& getMaterialManager();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::map<uint32, BaseMaterialPropertyListing>  mBaseMaterials;
			std::map<std::string, EditedMaterialInstance*> mEditedMaterials;
			MaterialManager*							   mMaterialManager;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf

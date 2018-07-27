// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/Material.h"
#include "qsf/base/manager/ElementManager.h"
#include "qsf/asset/AssetSystemTypes.h"
#include "qsf/forward/BoostPTree.h"

#include <iosfwd>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Material;
	class FileCache;
	class AssetSystem;
	class MaterialBlueprintManager;
	namespace detail
	{
		class OgreHlmsManagerListener;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash MaterialBlueprintId;	///< The material blueprint ID is a result of "qsf::StringHash(<material blueprint name>)"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material manager
	*/
	class QSF_API_EXPORT MaterialManager : public ElementManager<MaterialId, Material, detail::DirectAllocator<Material>, ConsecutiveIdGenerator<uint32>>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialSystem;	///< The material system is the master of this class
		friend class Material;			///< Must be able to access "qsf::MaterialManager::mOgreIdStringToMaterial"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Create a new material
		*
		*  @param[in] materialName
		*    Name of the new material to create
		*  @param[in] materialBlueprintId
		*    ID of the material blueprint the new material should use
		*
		*  @return
		*    The new material instance, null pointer on error (maybe the material name is already used?)
		*/
		Material* createMaterial(const std::string& materialName, MaterialBlueprintId materialBlueprintId);

		/**
		*  @brief
		*    Create a new material by cloning an existing one
		*
		*  @param[in] materialName
		*    Name of the new material to create
		*  @param[in] sourceMaterialId
		*    ID of the material to clone
		*
		*  @return
		*    The new material instance, null pointer on error (maybe the material name is already used?)
		*/
		Material* cloneMaterial(const std::string& materialName, MaterialId sourceMaterialId);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get a material instance by using "Ogre::IdString(<material name>)"
		*
		*  @param[in] ogreIdString
		*    Result of "Ogre::IdString(<material name>)"
		*
		*  @return
		*    The material instance, null pointer on error
		*/
		inline Material* getMaterialByOgreIdString(uint32 ogreIdString) const;

		Material* loadMaterialFromStream(std::istream& stream, const std::string& materialName, const std::string& absoluteFilename);
		Material* loadMaterialFromFilename(const std::string& materialName, const std::string& filename);
		Material* loadMaterialFromAbsoluteFilename(const std::string& materialName, const std::string& absoluteFilename);

		inline FileCache& getMaterialFileCache() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Constructor
		*/
		explicit MaterialManager(const MaterialBlueprintManager& materialBlueprintManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MaterialManager();

		void onAssetsMounted(const Assets& assets);
		void onAssetsUnmounted(const Assets& assets);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::unordered_map<uint32, Material*> OgreIdStringToMaterial;	///< "Ogre::IdString" to "qsf::Material" mapping


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const MaterialBlueprintManager& mMaterialBlueprintManager;	///< Reference to the material blueprint manager, do not destroy it
		AssetSystem&					mAssetSystem;				///< Reference to the asset system, do not destroy it
		FileCache&						mFileCache;					///< Caches all material files
		OgreIdStringToMaterial			mOgreIdStringToMaterial;	// TODO(co) Update in case of material destruction


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/MaterialManager-inl.h"

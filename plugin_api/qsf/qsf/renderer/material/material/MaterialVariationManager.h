// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/asset/AssetSystemTypes.h"
#include "qsf/base/GetUninitialized.h"
#include "qsf/base/manager/Manager.h"
#include "qsf/base/manager/PagedAllocator.h"
#include "qsf/renderer/material/material/MaterialProperties.h"

#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class Entity;
		class SubEntity;
	}
}
namespace qsf
{
	class Entity;
	class MaterialManager;
	class MaterialPropertyValue;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	typedef StringHash MaterialId;			///< The material ID is a result of "qsf::StringHash(<material name>)"
	typedef StringHash MaterialPropertyId;	///< Material property identifier, internally just a POD "uint32", result of hashing the property name


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material variation manager
	*/
	class QSF_API_EXPORT MaterialVariationManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class Map;	//	Must be able to call "qsf::MaterialVariationManager::destroyMaterialVariationForDestroyedEntityById()"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<GlobalAssetId> Materials;
		typedef std::unordered_map<uint64, Materials> EntityMaterialOverwrites;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialManager
		*    Used material manager instance
		*/
		explicit MaterialVariationManager(MaterialManager& materialManager);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~MaterialVariationManager();

		/**
		*  @brief
		*    Brute force destruction of all created material variations
		*/
		void destroyAllMaterialVariations();

		/**
		*  @brief
		*    Check whether or not the given entity is using material variations
		*
		*  @param[in] entityId
		*    ID of the entity to check for
		*
		*  @return
		*    "true" if the given entity is using material variations, else "false"
		*/
		bool hasEntityMaterialVariations(uint64 entityId) const;

		void setOgreSubEntityMaterial(Ogre::v1::Entity& ogreEntity, size_t subEntityIndex, MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue, GlobalAssetId materialGlobalAssetId = getUninitialized<GlobalAssetId>());

		/**
		*  @brief
		*    Overwrite overall entity material
		*
		*  @param[in] entity
		*    Entity to overwrite materials from
		*  @param[in] materials
		*    List of global material asset IDs, one for each sub-mesh in ascending order. Not overloaded sub-entities will be set to the material of the used sub-mesh.
		*/
		void overwriteEntityMaterials(const Entity& entity, const Materials& materials);

		std::string getOriginalMaterialName(uint64 entityId, const Ogre::v1::Entity& ogreEntity, size_t subEntityIndex) const;

		//[-------------------------------------------------------]
		//[ Property                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set a material property value by global material asset ID
		*
		*  @param[in] globalAssetId
		*    Global asset ID of the material to set a property value of
		*  @param[in] materialPropertyId
		*    ID of the material property to set the value
		*  @param[in] materialPropertyValue
		*    Value to set the given material property to
		*
		*  @return
		*    "true" if all went fine, else "false" (material or property does not exist?)
		*/
		bool setMaterialPropertyValue(GlobalAssetId globalAssetId, MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue);

		/**
		*  @brief
		*    Set a material property value by material name
		*
		*  @param[in] materialId
		*    ID of the material to set a property value of
		*  @param[in] materialPropertyId
		*    ID of the material property to set the value
		*  @param[in] materialPropertyValue
		*    Value to set the given material property to
		*
		*  @return
		*    "true" if all went fine, else "false" (material or property does not exist?)
		*/
		bool setMaterialPropertyValue(MaterialId materialId, MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue);

		/**
		*  @brief
		*    Set a material property value by using a given entity ID and optional material name
		*
		*  @param[in] entity
		*    Entity where to set a property value of
		*  @param[in] materialPropertyId
		*    ID of the material property to set the value, e.g. "TintPaletteIndex"
		*  @param[in] materialPropertyValue
		*    Value to set the given material property to
		*  @param[in] materialGlobalAssetId
		*    Optional global asset ID of the material to set a property value of
		*
		*  @note
		*    - Call "qsf::MaterialVariationManager::resetEntityMaterialPropertyValues()" when you no longer need per-instance material variations, which have an negative impact on the performance
		*/
		void setEntityMaterialPropertyValue(const Entity& entity, MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue, GlobalAssetId materialGlobalAssetId = getUninitialized<GlobalAssetId>());

		/**
		*  @brief
		*    Set a material property value by using a given ogre entity and optional material name
		*
		*  @param[in] ogreEntity
		*    Ogre entity where to set a property value of
		*  @param[in] materialPropertyId
		*    ID of the material property to set the value, e.g. "TintPaletteIndex"
		*  @param[in] materialPropertyValue
		*    Value to set the given material property to
		*  @param[in] materialGlobalAssetId
		*    Optional global asset ID of the material to set a property value of
		*
		*  @note
		*    - Call "qsf::MaterialVariationManager::resetEntityMaterialPropertyValues()" when you no longer need per-instance material variations, which have an negative impact on the performance
		*/
		void setEntityMaterialPropertyValue(Ogre::v1::Entity& ogreEntity, MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue, GlobalAssetId materialGlobalAssetId = getUninitialized<GlobalAssetId>());

		/**
		*  @brief
		*    Get an original material property value by using an entity
		*
		*  @param[in] entity
		*    Entity where to set a property value of
		*  @param[in] materialPropertyId
		*    ID of the material property to get the value, e.g. "TintPaletteIndex"
		*  @param[in] subEntityIndex
		*    index of ogre subEntity, most entities should the same material for all subEntities
		*
		*  @return
		*    Value to get of the given material property
		*/
		const MaterialPropertyValue& getOriginalMaterialPropertyValue(const Entity& entity, MaterialPropertyId materialPropertyId, uint32 subEntityIndex = 0) const;

		/**
		*  @brief
		*    Reset all material properties of the given entity to default settings
		*
		*  @param[in] entity
		*    Entity where to reset the properties from
		*  @param[in] removeEntityMaterialOverwrites
		*    If "true", also remove entity material overwrites
		*
		*  @note
		*    - Warning: Check if there are other parts using material variation manager for this entity
		*/
		void resetEntityMaterialPropertyValues(const Entity& entity, bool removeEntityMaterialOverwrites = true);

		/**
		*  @brief
		*    Reset all material properties of the given ogre entity to default settings
		*
		*  @param[in] ogreEntity
		*    Ogre entity where to reset the properties from
		*/
		void resetEntityMaterialPropertyValues(Ogre::v1::Entity& ogreEntity);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateOgreEntityMaterial(Ogre::v1::Entity& ogreEntity, MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue, GlobalAssetId materialGlobalAssetId = getUninitialized<GlobalAssetId>());
		void resetOgreEntityMaterial(Ogre::v1::Entity& ogreEntity);
		bool getCloneMaterialName(const Ogre::v1::Entity& ogreEntity, const Ogre::v1::SubEntity& ogreSubEntity, std::string& outMaterialName) const;
		bool getCloneMaterialName(const Ogre::v1::Entity& ogreEntity, const std::string& originalMaterialName, std::string& outMaterialName) const;

		/**
		*  @brief
		*    Destroy material variation for destroyed entity provided by ID
		*
		*  @param[in] entityId
		*    ID of the already destroyed entity
		*/
		void destroyMaterialVariationForDestroyedEntityById(uint64 entityId);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		// Material variation data we store per QSF entity
		struct ChangedMaterial
		{
			MaterialId		   materialId;
			MaterialProperties changedMaterialProperties;
			ChangedMaterial(MaterialId _materialId) :
				materialId(_materialId)
			{ }
		};
		typedef std::vector<ChangedMaterial> ChangedMaterials;
		struct PerEntityData
		{
			ChangedMaterials mChangedMaterials;
		};
		typedef PagedAllocator<PerEntityData, 128> PerEntityDataAllocator;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MaterialManager&							mMaterialManager;
		std::unordered_map<uint64, PerEntityData&>	mGeneratedEntityMaterials;	///< Map of QSF entity ID to material variation data stored for this entity
		PerEntityDataAllocator						mPerEntityDataAllocator;
		EntityMaterialOverwrites					mEntityMaterialOverwrites;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/MaterialVariationManager-inl.h"

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/MaterialTexture.h"
#include "qsf/renderer/material/material/MaterialProperties.h"

#include <OGRE/OgreHlmsDatablock.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class MaterialBlueprint;
	class HlmsBlueprintDatablock;
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
	typedef StringHash MaterialId;	///< The material ID is a result of "qsf::StringHash(<material name>)"


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Material
	*/
	class QSF_API_EXPORT Material : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MaterialManager;					///< Must be able to access "qsf::Material::mOgreIdStringHash"
		friend class JsonMaterialSerializer;			///< Fills the data
		friend class HlmsBlueprintDatablock;			///< Must be able to set "qsf::Material::mHlmsBlueprintDatablock"
		friend class detail::OgreHlmsManagerListener;	///< Must be able to set "qsf::Material::mHlmsBlueprintDatablock"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] materialId
		*    The material ID
		*/
		inline explicit Material(MaterialId materialId);

		/**
		*  @brief
		*    Destructor
		*/
		~Material();

		/**
		*  @brief
		*    Return the material ID
		*
		*  @return
		*    The material ID
		*/
		inline MaterialId getMaterialId() const;

		/**
		*  @brief
		*    Return the material blueprint
		*
		*  @return
		*    The material blueprint, null pointer on error, do not destroy the instance
		*/
		inline MaterialBlueprint* getMaterialBlueprint() const;

		/**
		*  @brief
		*    Return the material parent
		*
		*  @return
		*    The material parent, null pointer on error, do not destroy the instance, can be identical to the material blueprint
		*/
		inline Material* getMaterialParent() const;

		/**
		*  @brief
		*    Set the material parent
		*
		*  @param[in] materialParent
		*    The material parent, can be a null pointer
		*
		*  @note
		*    - All property values will be reset
		*/
		void setMaterialParent(Material* materialParent);

		//[-------------------------------------------------------]
		//[ Property                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the material properties
		*
		*  @return
		*    The material properties
		*/
		inline const MaterialProperties& getMaterialProperties() const;

		/**
		*  @brief
		*    Return the material properties as sorted vector
		*
		*  @return
		*    The material properties as sorted vector
		*/
		inline const MaterialProperties::SortedPropertyVector& getSortedPropertyVector() const;

		/**
		*  @brief
		*    Return a material property by its ID
		*
		*  @param[in] materialPropertyId
		*    ID of the material property to return
		*
		*  @return
		*    The requested material property, null pointer on error, don't destroy the returned instance
		*/
		inline const MaterialProperty* getPropertyById(MaterialPropertyId materialPropertyId) const;

		/**
		*  @brief
		*    Set a material property value by its ID
		*
		*  @param[in] materialPropertyId
		*    ID of the material property to set the value from
		*  @param[in] materialPropertyValue
		*    The material property value to set
		*  @param[in] usage
		*    The material property usage
		*
		*  @return
		*    "true" if a material property change has been detected, else "false"
		*/
		inline bool setPropertyById(MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue, MaterialProperty::Usage usage = MaterialProperty::Usage::DYNAMIC);

		//[-------------------------------------------------------]
		//[ Internal derived data                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the material OGRE HLMS macroblock
		*
		*  @return
		*    The material OGRE HLMS macroblock
		*/
		const Ogre::HlmsMacroblock& getOgreHlmsMacroblock();

		/**
		*  @brief
		*    Return the material OGRE HLMS blendblock
		*
		*  @return
		*    The material OGRE HLMS blendblock
		*/
		const Ogre::HlmsBlendblock& getOgreHlmsBlendblock();

		/**
		*  @brief
		*    Return the material textures
		*
		*  @param[in] casterPass
		*    Caster pass?
		*
		*  @return
		*    The material textures
		*/
		const MaterialTextures& getMaterialTextures(bool casterPass);


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		typedef std::vector<Material*> ChildMaterials;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		MaterialId				mMaterialId;				///< Material ID
		uint32					mOgreIdStringHash;
		MaterialBlueprint*		mMaterialBlueprint;			///< Material blueprint, can be a null pointer, do not destroy the instance
		Material*				mMaterialParent;			///< Material parent, can be a null pointer, do not destroy the instance, can be identical to the material blueprint
		ChildMaterials			mChildMaterials;			///< Direct child materials, don't destroy the instances
		MaterialProperties		mMaterialProperties;
		Ogre::HlmsMacroblock	mOgreHlmsMacroblock;
		Ogre::HlmsBlendblock	mOgreHlmsBlendblock;
		MaterialTextures		mMaterialTextures[2];		///< Index 0 for default pass, index 1 for shadow map pass
		bool					mMaterialTexturesDirty;
		HlmsBlueprintDatablock* mHlmsBlueprintDatablock;	///< HLMS blueprint datablock representing this material instance inside OGRE, can be a null pointer, don't destroy the instance


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void unregisterFromMaterialParent();
		bool setPropertyByIdInternal(MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue, MaterialProperty::Usage usage, bool changeOverwrittenState);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/material/material/Material-inl.h"

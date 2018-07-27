// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Material::Material(MaterialId materialId) :
		mMaterialId(materialId),
		mOgreIdStringHash(getUninitialized<uint32>()),
		mMaterialBlueprint(nullptr),
		mMaterialParent(nullptr),
		mMaterialTexturesDirty(true),
		mHlmsBlueprintDatablock(nullptr)
	{
		// Nothing to do in here
	}

	inline MaterialId Material::getMaterialId() const
	{
		return mMaterialId;
	}

	inline MaterialBlueprint* Material::getMaterialBlueprint() const
	{
		return mMaterialBlueprint;
	}

	inline Material* Material::getMaterialParent() const
	{
		return mMaterialParent;
	}

	inline const MaterialProperties& Material::getMaterialProperties() const
	{
		return mMaterialProperties;
	}

	inline const MaterialProperties::SortedPropertyVector& Material::getSortedPropertyVector() const
	{
		return mMaterialProperties.getSortedPropertyVector();
	}

	inline const MaterialProperty* Material::getPropertyById(MaterialPropertyId materialPropertyId) const
	{
		return mMaterialProperties.getPropertyById(materialPropertyId);
	}

	inline bool Material::setPropertyById(MaterialPropertyId materialPropertyId, const MaterialPropertyValue& materialPropertyValue, MaterialProperty::Usage usage)
	{
		return setPropertyByIdInternal(materialPropertyId, materialPropertyValue, usage, true);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

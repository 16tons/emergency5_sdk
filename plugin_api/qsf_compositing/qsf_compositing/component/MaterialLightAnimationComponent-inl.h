// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline MaterialLightAnimationComponent::~MaterialLightAnimationComponent()
		{
			// Nothing here
		}

		inline const AssetProxy& MaterialLightAnimationComponent::getMaterial() const
		{
			return mLightMaterial;
		}

		inline void MaterialLightAnimationComponent::setMaterial(const AssetProxy& material)
		{
			mLightMaterial = material;
		}

		inline bool MaterialLightAnimationComponent::getLightMaterialIsInTopParent() const
		{
			return mLightMaterialIsInTopParent;
		}

		inline void MaterialLightAnimationComponent::setLightMaterialIsInTopParent(bool lightMaterialIsInTopParent)
		{
			mLightMaterialIsInTopParent = lightMaterialIsInTopParent;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf

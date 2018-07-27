// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline LightControllerComponent::LightControllerComponent(Prototype* prototype) :
			Component(prototype),
			mLightPosition(LIGHTPOSITION_UNDEFINED),
			mLightMaterialIsInTopParent(true),
			mLightMaterialPosition(LIGHTMATERIALPOSITION_TOPLEVEL_PARENT),
			mInternalUseEmissiveMap(false),
			mRendererUseEmissiveMap(false),
			mForceEmissiveMapUpdate(false)
		{
			// Not active by default
			setActive(false);

			// Get rid of this expensive thing as soon as possible
			setNonvisibleUpdateTimeout(qsf::Time::fromSeconds(0.2f));
		}

		inline LightControllerComponent::~LightControllerComponent()
		{
			// Nothing to do in here
		}

		inline LightControllerComponent::LightPosition LightControllerComponent::getLightPosition() const
		{
			return mLightPosition;
		}

		inline const AssetProxy& LightControllerComponent::getLightMaterial() const
		{
			return mLightMaterial;
		}

		inline bool LightControllerComponent::getLightMaterialIsInTopParent() const
		{
			return mLightMaterialIsInTopParent;
		}

		inline void LightControllerComponent::setLightMaterialIsInTopParent(bool lightMaterialIsInTopParent)
		{
			mLightMaterialIsInTopParent = lightMaterialIsInTopParent;
		}

		inline void LightControllerComponent::setForceEmissiveMapUpdate(bool forceUpdate)
		{
			mForceEmissiveMapUpdate = forceUpdate;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf

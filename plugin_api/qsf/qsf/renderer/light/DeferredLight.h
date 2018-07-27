// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/light/LightComponent.h"

#include <OGRE/OgreSimpleRenderable.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class OgreCompositorPassFactoryDeferredLight;
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
	*    Deferred light geometry
	*
	*  @note
	*    - Each instance matches a normal light
	*    - Should not be created by the user
	*/
	class DeferredLight : public Ogre::Renderable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		DeferredLight(OgreCompositorPassFactoryDeferredLight& ogreCompositorPassFactoryDeferredLight, LightComponent& lightComponent);
		virtual ~DeferredLight();

		/**
		*  @brief
		*    Update the information from the light that matches this one
		*/
		void updateFromParent(bool firstUpdate = false);

		/**
		*  @brief
		*    Update the information that is related to the material
		*/
		void updateMaterial();

		/**
		*  @brief
		*    Does this light cast shadows?
		*/
		bool getCastChadows() const;

		void fillLightMatrix(const Ogre::Camera& ogreCamera, Ogre::Matrix4& ogreMatrix) const;


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::Renderable methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void getRenderOperation(Ogre::v1::RenderOperation& ogreRenderOperation, bool casterPass) override;
		virtual void getWorldTransforms(Ogre::Matrix4* xform) const override;
		virtual const Ogre::LightList& getLights() const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Create geometry for this light
		*/
		uint32 rebuildGeometry();

		void updateInternalMaterial();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		OgreCompositorPassFactoryDeferredLight&	mOgreCompositorPassFactoryDeferredLight;
		Ogre::v1::RenderOperation				mRenderOp;
		Ogre::Matrix4							mTransform;
		LightComponent&							mLightComponent;	///< The light component that this deferred light renders, don't destroy the instance
		bool									mIgnoreWorld;		///< Mode to ignore world orientation/position
		LightComponent::LightType				mLightType;
		float									mRadius;			///< Bounding sphere radius
		float									mHeight;			///< Cone height used for spot lights
		uint32									mCombination;		///< Material combination
		std::string								mMaterialName;
		MaterialId								mMaterialId;
		GlobalAssetId							mGlobalTextureAssetId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_compositing/Export.h"

#include <qsf/renderer/light/LightAnimationComponent.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Material light animation component class
		*/
		class QSF_COMPOSITING_API_EXPORT MaterialLightAnimationComponent : public LightAnimationComponent
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::compositing::MaterialLightAnimationComponent" unique component ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] prototype
			*    The prototype this component is in, no null pointer allowed
			*/
			explicit MaterialLightAnimationComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~MaterialLightAnimationComponent();

			/**
			*  @brief
			*    Return the material asset which defines the emissive map for the light
			*
			*  @return
			*    The material asset to use (e.g. "sample/materials/spot/bat"), do no destroy the returned instance
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline const AssetProxy& getMaterial() const;

			/**
			*  @brief
			*    Set the material asset which defines the emissive map for the light
			*
			*  @param[in] material
			*    The material asset to use (e.g. "sample/material/spot/bat")
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline void setMaterial(const AssetProxy& material);

			/**
			*  @brief
			*    Return if the material is located in the top parent entity or the entity which the component is created into
			*
			*  @return
			*    "True" when the material is located in the top parent otherwise "False"
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline bool getLightMaterialIsInTopParent() const;

			/**
			*  @brief
			*    Set if the light material is located in the top parent entity or in the entity which the component is created into
			*
			*  @param[in] lightMaterialIsInTopParent
			*    "True" when the material is located in the top parent otherwise "False"
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline void setLightMaterialIsInTopParent(bool lightMaterialIsInTopParent);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Component methods                 ]
		//[-------------------------------------------------------]
		public:
			virtual void serialize(BinarySerializer& serializer) override;


		//[--------------------------------------------------------]
		//[ Protected virtual qsf::LightAnimationComponent methods ]
		//[--------------------------------------------------------]
		protected:
			virtual void setLightActive(bool active) override;
			virtual void setLightIntensityFactor(float intensity) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
		//[-------------------------------------------------------]
		protected:
			virtual Component* getMovableObjectRenderingListenerComponent() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			Entity& getLightMaterialHolder() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			AssetProxy	mLightMaterial;					///< The light material to be controlled by the light animation
			bool		mLightMaterialIsInTopParent;	///< Specifies in which entity the material is located (true = in top parent entity, false = in the entity in which the component is created)


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_compositing/component/MaterialLightAnimationComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::compositing::MaterialLightAnimationComponent)

// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/PropertyHelper.h>
#include <qsf/component/Component.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>
#include <qsf/renderer/map/JobProxyMovableObjectRenderingListener.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Component class which controls a light, this can be a mesh or an qsf::LightComponent
		*/
		class QSF_GAME_API_EXPORT LightControllerComponent : public Component, public JobProxyMovableObjectRenderingListener
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::game::LightControllerComponent" unique component ID
			static const uint32 ACTIVE;			///< "Active" unique class property ID inside the class

			// Named light position because the qsf::LightComponent has already an enum named "LightType"
			enum LightPosition
			{
				LIGHTPOSITION_UNDEFINED,
				LIGHTPOSITION_HEAD,
				LIGHTPOSITION_BRAKE,
				LIGHTPOSITION_LEFT_BLINKER,
				LIGHTPOSITION_RIGHT_BLINKER,
				LIGHTPOSITION_BLUE,
				_NUM_LIGHTPOSITION
			};

			enum LightMaterialPosition
			{
				LIGHTMATERIALPOSITION_DIRECT,
				LIGHTMATERIALPOSITION_PARENT,
				LIGHTMATERIALPOSITION_TOPLEVEL_PARENT
			};


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
			inline explicit LightControllerComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~LightControllerComponent();

			inline LightPosition getLightPosition() const;
			void setLightPosition(LightPosition lightPosition);

			QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(LightMaterialPosition, LightMaterialPosition, mLightMaterialPosition)

			/**
			*  @brief
			*    Return the emissive material asset to use as lights
			*
			*  @return
			*    The material asset to use (e.g. "sample/materials/spot/bat"), do no destroy the returned instance
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*    - The material asset instance is used to get material name of the emissive map of the entity which stands for the lights
			*/
			inline const AssetProxy& getLightMaterial() const;

			/**
			*  @brief
			*    Set the emissive material asset to use as lights
			*
			*  @param[in] material
			*    The material asset to use (e.g. "sample/material/spot/bat")
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*    - The material asset instance is used to get material name of the emissive map of the entity which stands for the sign lights
			*/
			void setLightMaterial(const AssetProxy& material);

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
			*    "true" when the material is located in the top parent otherwise "false"
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline void setLightMaterialIsInTopParent(bool lightMaterialIsInTopParent);

			/**
			*  @brief
			*    On next synchronization of the internal and render state of the emissive map the update is forced regardless if the internal and the render state differs
			*/
			inline void setForceEmissiveMapUpdate(bool forceUpdate);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onSetActive(bool active) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
		//[-------------------------------------------------------]
		protected:
			virtual Component* getMovableObjectRenderingListenerComponent() override;
			virtual void onObjectRendering(const Component& component, const Ogre::Camera& ogreCamera) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void internalSetActive(bool active);
			Entity& getLightMaterialHolder() const;
			void onOneShotInit();
			void updateInternalLightMaterial();
			void findLightMaterial();
			void synchronizeInternalAndRendererUseEmissiveMap();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			LightPosition			mLightPosition;
			// TODO(sw) This property is obsolete -> replaced by mLightMaterialPosition
			bool					mLightMaterialIsInTopParent;
			LightMaterialPosition	mLightMaterialPosition;
			AssetProxy				mLightMaterial;					///< The light material to be controlled by this component
			JobProxy				mOneShotInitLightsJobProxy;		///< Job proxy to do a one shot initialization of the light state, because currently when the initialization is called (when e.g. a unit is spawned) some components aren't yet initialized (mostly the link component which is used to disable the emissive map for e.g. the head lights)
			// Internal and renderer use emissive map
			AssetProxy				mInternalLightMaterial;
			bool					mInternalUseEmissiveMap;
			bool					mRendererUseEmissiveMap;
			bool					mForceEmissiveMapUpdate;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/component/base/LightControllerComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::LightControllerComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::LightControllerComponent::LightPosition)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::LightControllerComponent::LightMaterialPosition)

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_compositing/Export.h"

#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/map/JobProxyMovableObjectRenderingListener.h>


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
		*    Tintable mesh component class
		*/
		class QSF_COMPOSITING_API_EXPORT TintableMeshComponent : public MeshComponent, public JobProxyMovableObjectRenderingListener
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;		///< "qsf::compositing::TintableMeshComponent" unique component ID
			static const uint32 TINT_PALETTE;		///< "TintPalette" unique class property ID inside the class
			static const uint32 TINT_PALETTE_INDEX;	///< "TintPaletteIndex" unique class property ID inside the class
			static const uint32 METALLIC_CAR_PAINT;	///< "MetallicCarPaint" unique class property ID inside the class


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
			inline explicit TintableMeshComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~TintableMeshComponent();

			/**
			*  @brief
			*    Return the tint palette texture asset to use
			*
			*  @return
			*    The tint palette texture asset to use (e.g. "sample/texture/palette/house"), do no destroy the returned instance
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline const AssetProxy& getTintPalette() const;

			/**
			*  @brief
			*    Set the tint palette texture asset to use
			*
			*  @param[in] tintPalette
			*    The tint palette texture asset to use (e.g. "sample/texture/palette/house")
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			void setTintPalette(const AssetProxy& tintPalette);

			/**
			*  @brief
			*    Return the color index inside the tint palette texture asset to use
			*
			*  @return
			*    The color index inside the tint palette texture asset to use
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline int getTintPaletteIndex() const;

			/**
			*  @brief
			*    Set the color index inside the tint palette texture asset to use
			*
			*  @param[in] tintPaletteIndex
			*    Color index inside the tint palette texture asset to use
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			void setTintPaletteIndex(int tintPaletteIndex);

			/**
			*  @brief
			*    Return if we inherit the tint color from parents
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline bool getInheritTintColor() const;

			/**
			*  @brief
			*    Set to inherit the tint color from parents
			*
			*  @param[in] inheritTintColor
			*    true: inherit the tint color, false: don't inherit tint colors
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline void setInheritTintColor(bool inheritTintColor);

			/**
			*  @brief
			*    Return if we have set metallic car paint
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline bool getMetallicCarPaint() const;

			/**
			*  @brief
			*    Set if we want metallic car paint
			*
			*  @param[in] metallicCarPaint
			*    if we want metallic car paint or not
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			void setMetallicCarPaint(bool metallicCarPaint);

			// TODO(co) The private implementation detail was made public in SVN revision 11236 ""changed TintableMeshComponent::updateMaterial() from private to public (updateMaterial() needs to be called after the mesh and materials were switched in vehicle damage visualization)
			// Please make it private again. Privacy is a good thing to keep the overall project complexity under control.
			void updateMaterial(bool forceUpdate = false);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
		//[-------------------------------------------------------]
		protected:
			virtual Component* getMovableObjectRenderingListenerComponent() override;
			virtual void onObjectRendering(const Component& component, const Ogre::Camera& ogreCamera) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::MeshComponent methods          ]
		//[-------------------------------------------------------]
		protected:
			virtual void onOgreEntityChanged() override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onStartup() override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			inline void makeMaterialDirty();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			AssetProxy	mTintPalette;		///< The tint palette texture asset to use (e.g. "sample/texture/palette/house")
			int			mTintPaletteIndex;	///< Color index inside the tint palette texture asset to use
			bool		mInheritTintColor;	///< Inherit tint color from parents or not
			bool		mMetallicCarPaint;	///< Metallic car paint

			// Internal
			bool mMaterialDirty;	///< Is the renderer material dirty and needs to be updated next time it's used?
			bool mMaterialChanged;	///< Has the material been changed by this component at all?


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
#include "qsf_compositing/component/TintableMeshComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::compositing::TintableMeshComponent)

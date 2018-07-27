// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/reflection/type/CampQsfAssetProxy.h"
#include "qsf/reflection/type/CampGlmVec2.h"
#include "qsf/reflection/type/CampGlmVec3.h"
#include "qsf/reflection/type/CampQsfColor4.h"
#include "qsf/renderer/component/RendererComponent.h"
#include "qsf/debug/DebugDrawProxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class HardwareOcclusionQuery;
	namespace v1
	{
		class Billboard;
		class BillboardSet;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Flare component class
	*/
	class QSF_API_EXPORT FlareComponent : public RendererComponent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class FlareManager;
		friend class FlareOcclusionBillboardSet;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32		 COMPONENT_ID;		///< "qsf::FlareComponent" unique component ID
		static const uint32		 BILLBOARD_TYPE;	///< "BillboardType" unique class property ID inside the class
		static const uint32		 COLOR;				///< "Color" unique class property ID inside the class
		static const uint32		 INTENSITY;			///< "Intensity" unique class property ID inside the class
		static const uint32		 DIRECTION;			///< "Direction" unique class property ID inside the class
		static const uint32		 WIDTH_HEIGHT;		///< "WidthHeight" unique class property ID inside the class
		static const uint32		 OCCLUSION_SCALE;	///< "OcclusionScale" unique class property ID inside the class
		static const std::string USER_ANY_KEY;		///< OGRE user any key "qsf::FlareComponent"

		/**
		*  @brief
		*    The billboard type
		*/
		enum BillboardType
		{
			POINT         = 1,	///< Standard point flare (default), always faces the camera completely and is always upright
			ORIENTED      = 2,	///< Flare is oriented around a shared direction vector (used as Y axis) and only rotate around this to face the camera
			PERPENDICULAR = 3	///< Flare is perpendicular to a shared direction vector (used as Z axis, the facing direction) and X, Y axis are determined by a shared up-vector
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
		inline explicit FlareComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FlareComponent();

		inline BillboardType getBillboardType() const;
		void setBillboardType(BillboardType billboardType);

		/**
		*  @brief
		*    Return the color of the flare
		*
		*  @return
		*    The color of the flare, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const Color4& getColor() const;

		/**
		*  @brief
		*    Set the color of the flare
		*
		*  @param[in] color
		*    The color of the flare
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setColor(const Color4& color);

		/**
		*  @brief
		*    Return the intensity of the flare
		*
		*  @return
		*    The intensity of the flare
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getIntensity() const;

		/**
		*  @brief
		*    Set the intensity of the flare
		*
		*  @param[in] intensity
		*    The intensity of the flare
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setIntensity(float intensity);

		/**
		*  @brief
		*    Return the width and height of the flare
		*
		*  @return
		*    The width and height of the flare, do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec2& getWidthHeight() const;

		/**
		*  @brief
		*    Set the width and height of the flare
		*
		*  @param[in] widthHeight
		*    The width and height of the flare
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setWidthHeight(const glm::vec2& widthHeight);

		/**
		*  @brief
		*    Get/set direction where the billboard gets rotated in oriented mode
		*/
		inline const glm::vec3& getDirection() const;
		void setDirection(const glm::vec3& direction);

		inline float getOcclusionScale() const;
		void setOcclusionScale(float occlusionScale);

		inline void setOcclusionQueriesEnabled(bool occlusionQueriesEnabled);
		void setAngleVisibilityFactor(float angleVisibilityFactor);

		inline float getVisibilityAngle() const;
		void setVisibilityAngle(float visibilityAngle);

		inline uint32 getTextureIndex() const;
		void setTextureIndex(uint32 textureIndex);

		/**
		*  @brief
		*    Return the material asset to be use as flare material
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
		*    Set the material asset to be use as flare material
		*
		*  @param[in] material
		*    The material asset to use (e.g. "sample/material/spot/bat")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setMaterial(const AssetProxy& material);

		/**
		*  @brief
		*    Returns the number of texture rows the atlas consists of
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline uint16 getAtlasNumberOfTextureRows() const;

		/**
		*  @brief
		*    Set the number of texture rows inside the atlas texture
		*
		*  @param[in] atlasTextureRowCount
		*    The number of texture rows the atlas consists of
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setAtlasNumberOfTextureRows(uint16 atlasTextureRowCount);

		/**
		*  @brief
		*    Returns the number of textures per texture row of the atlas
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline uint16 getAtlasNumberOfTexturesPerTextureRow() const;

		/**
		*  @brief
		*    Set the number of textures per texture row of the atlas
		*
		*  @param[in] atlasTextureSliceCount
		*    The number of textures per texture row inside the atlas
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setAtlasNumberOfTexturesPerTextureRow(uint16 atlasTextureSliceCount);

		/**
		*  @brief
		*    Set the intensity factor
		*
		*  @note
		*    - This used e.g. by the light animation component
		*/
		void setIntensityFactor(float intensityFactor);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	public:
		inline virtual Ogre::SceneNode* getOgreSceneNode() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual uint32 getDefaultRenderQueueId() const override;
		inline virtual bool isGridManaged() const override;


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
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const std::string QUERY_AREA_MATERIAL_NAME;		///< Name of OGRE material used for query area
		static const std::string QUERY_VISIBLE_MATERIAL_NAME;	///< Name of OGRE material used for query visible


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateOgreBillboardType() const;
		void updateOgreGeometry() const;
		void updateDirection() const;
		void updateOcclusionQuery();

		/**
		*  @brief
		*    Set the color of the OGRE billboard flare
		*/
		void setOgreBillboardFlareColour() const;

		void updateOgreOcclusionGeometry() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		BillboardType					mBillboardType;						///< Billboard type
		Color4							mColor;								///< The color of the flare
		float							mIntensity;							///< The intensity of the flare
		glm::vec2						mWidthHeight;						///< The width and height of the flare
		glm::vec3						mDirection;							///< Direction of rotation in oriented mode
		float							mOcclusionScale;					///< Occlusion scale
		// Material settings
		AssetProxy						mMaterialAssetProxy;				///< Asset proxy for the material to be used for the flare
		uint8							mAtlasStackCount;					///< The number of texture rows inside the material
		uint8							mAtlasSliceCount;					///< The number of textures per row inside the material
		// Internal only
		Ogre::SceneNode*				mOgreSceneNode;						///< OGRE scene node instance, can be a null pointer
		Ogre::v1::Billboard*			mOgreBillboardFlare;				///< OGRE billboard for the flare, can be a null pointer
		Ogre::v1::BillboardSet*			mOgreBillboardSetFlare;				///< OGRE billboard set for the flare, can be a null pointer
		Ogre::v1::BillboardSet*			mOgreBillboardSetQueryArea;			///< OGRE billboard set for the flare query area, can be a null pointer
		Ogre::v1::BillboardSet*			mOgreBillboardSetQueryVisible;		///< OGRE billboard set for the flare query visibility, can be a null pointer
		Ogre::HardwareOcclusionQuery*	mOgreHardwareOcclusionQueryArea;	///< OGRE hardware occlusion query instance for area, can be a null pointer
		Ogre::HardwareOcclusionQuery*	mOgreHardwareOcclusionQueryVisible;	///< OGRE hardware occlusion query instance for visibility, can be a null pointer
		bool							mDoOcclusionQuery;					///< Requesting new queries allowed?
		bool							mOcclusionQueriesEnabled;			///< Requesting new queries allowed? (set by "qsf::FlareManager")
		float							mVisibilityFactor;					///< Visibility factor [0 .. 1] = [not visible at all .. fully visible]
		float							mAngleVisibilityFactor;				///< Visibility factor by angle [0 .. 1] = [not visible at all .. fully visible]
		float							mVisibilityAngle;
		uint32							mTextureIndex;						///< Flare texture index
		float							mIntensityFactor;					///< Intensity factor, is used to change the intensity via an light animation
		// Internal only
		DebugDrawProxy					mDebugDrawProxy;					///< Debug draw proxy for orientation visualization


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/flare/FlareComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::FlareComponent)
QSF_CAMP_TYPE_NONCOPYABLE(qsf::FlareComponent::BillboardType)

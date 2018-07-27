// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/component/RendererComponent.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Ray;
	class Terrain;
	class TerrainGlobalOptions;
	class TerrainGroup;
}
namespace qsf
{
	class ParameterGroup;
	class CameraComponent;
	class RenderWindow;
	class TerrainDefinition;
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
	*    Terrain component class
	*
	*  @remarks
	*    == OGRE terrain global color map ==
	*    Inside the OGRE "dat" terrain format, OGRE can store the global terrain color map.
	*
	*    Sadly, this global color map is stored as uncompressed RGB without mipmaps. While this is fine for editing, it's far from optimal for best possible loading times and runtime performance.
	*    Additionally, we're using the alpha channel of the global terrain color map to be able to cut holes into the terrain. This information would get lost when storing the global color map
	*    directly inside the OGRE "dat" terrain format. In retail build, we noticed random crashes when using the global terrain color map stored inside the OGRE "dat" terrain format.
	*
	*    In order to overcome all those issues with the global OGRE terrain color map, we just don't store it inside the OGRE "dat" terrain format. When loading the OGRE terrain, we provide it
	*    with an optimized cached asset global color map. For editing the terrain color map, we keep the uncompressed source asset as tif. It's the job of the editor to generate the optimized
	*    cached asset global color map.
	*/
	class QSF_API_EXPORT TerrainComponent : public RendererComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "qsf::TerrainComponent" unique component ID
		static const uint32 HEIGHT_MAP_SIZE;			///< "HeightMapSize" unique class property ID inside the class
		static const uint32 COLOR_MAP_SIZE;				///< "ColorMapSize" unique class property ID inside the class
		static const uint32 BLEND_MAP_SIZE;				///< "BlendMapSize" unique class property ID inside the class
		static const uint32 TERRAIN_WORLD_SIZE;			///< "TerrainWorldSize" unique class property ID inside the class
		static const uint32 SKIRT_SIZE;					///< "SkirtSize" unique class property ID inside the class
		static const uint32 MAX_PIXEL_ERROR;			///< "MaxPixelError" unique class property ID inside the class
		static const uint32 TERRAIN_CHUNKS_PER_EDGE;	///< "TerrainChunksPerEdge" unique class property ID inside the class
		static const uint32 TERRAIN_ASSET;				///< "TerrainAsset" unique class property ID inside the class
		static const uint8  RENDER_QUEUE_GROUP_ID;


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
		inline explicit TerrainComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TerrainComponent();

		/**
		*  @brief
		*    Get the height map size of terrain
		*
		*  @return
		*    Height map size
		*/
		inline uint32 getHeightMapSize() const;

		/**
		*  @brief
		*    Set the height map size of terrain
		*
		*  @param[in] heightMapSize
		*    Height map size (requirement: 2^n + 1)
		*/
		void setHeightMapSize(uint32 heightMapSize);

		/**
		*  @brief
		*    Get the color map size of terrain
		*
		*  @return
		*    Color map size
		*/
		inline uint32 getColorMapSize() const;

		/**
		*  @brief
		*    Set the color map size of terrain
		*
		*  @param[in] colorMapSize
		*    Color map size (requirement: 2^n)
		*/
		void setColorMapSize(uint32 colorMapSize);

		/**
		*  @brief
		*    Get the blend map size of terrain
		*
		*  @return
		*    Blend map size
		*/
		inline uint32 getBlendMapSize() const;

		/**
		*  @brief
		*    Set the blend map size of terrain
		*
		*  @param[in] blendMapSize
		*    Blend map size (requirement: 2^n)
		*/
		void setBlendMapSize(uint32 blendMapSize);

		/**
		*  @brief
		*    Get the terrain chunks per edge
		*
		*  @return
		*    Terrain chunks per edge
		*/
		inline int getTerrainChunksPerEdge() const;

		/**
		*  @brief
		*    Set the terrain chunks per edge
		*
		*  @param[in] terrainChunksPerEdge
		*    Terrain chunks per edge (requirement: 2^n, tested with 1, 2, 4, 8, 16)
		*/
		void setTerrainChunksPerEdge(int terrainChunksPerEdge);

		/**
		*  @brief
		*    Get the terrain world size
		*
		*  @return
		*    Terrain size
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getTerrainWorldSize() const;

		/**
		*  @brief
		*    Set terrain world size
		*
		*  @param[in] terrainWorldSize
		*    Terrain size
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setTerrainWorldSize(float terrainWorldSize);

		/**
		*  @brief
		*    Get the skirt size of terrain
		*
		*  @return
		*    Skirt size
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getSkirtSize() const;

		/**
		*  @brief
		*    Set the skirt of terrain
		*
		*  @param[in] skirtSize
		*    Size of skirt
		*
		*  @remarks
		*    Set the skirt of terrain. This means, to build a skirt around
		*    the terrain pieces to hide cracks in terrain between different details.
		*    Don't use to high values, because that slows down the rendering because
		*    of the pixel fill rate. A value of 2 could be optimal.
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setSkirtSize(float skirtSize);

		/**
		*  @brief
		*    Get the maximum pixel error
		*
		*  @return
		*    The maximum pixel error
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getMaxPixelError() const;

		/**
		*  @brief
		*    Set the maximum pixel error
		*
		*  @param[in] maxPixelError
		*    The maximum pixel error
		*
		*  @note
		*    - This is used to specify the LOD to be used
		*    - Connected to the CAMP reflection system
		*/
		void setMaxPixelError(float maxPixelError);

		/**
		*  @brief
		*    Get segments of a height map
		*
		*  @return
		*    Number of segments
		*
		*  @note
		*    - Segments are 2^n-1
		*    - TODO(np) Segment count is needed for Terrain Editing. It is only a getter
		*               So should it connected to the CAMP reflection system?
		*/
		int getTerrainSegments() const;

		inline const AssetProxy& getTerrainAsset() const;
		void setTerrainAsset(const AssetProxy& assetProxy);

		inline bool getEditing() const;
		void setEditing(bool isEditing);

		TerrainDefinition* getTerrainDefinition();	// If the instance doesn't exist yet because e.g. the terrain component isn't running, the instance will be created and loaded
		inline const TerrainDefinition* getTerrainDefinition() const;

		//[-------------------------------------------------------]
		//[ Collision                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Find the intersection point of terrain component by a ray generated by using a given render window and a mouse position
		*
		*  @param[in]  renderWindow
		*    Render window to use for the ray generation
		*  @param[in]  xPosition
		*    X mouse position inside the render window to use for the ray generation
		*  @param[in]  yPosition
		*    Y mouse position inside the render window to use for the ray generation
		*  @param[out] position
		*    If not a null pointer, if the a hit was found, we get the position of intersecting
		*
		*  @return
		*    True if a hit with the terrain component was found
		*/
		bool getTerrainHitByRenderWindow(const RenderWindow& renderWindow, int xPosition, int yPosition, glm::vec3* position) const;

		/**
		*  @brief
		*    Find the intersection point of terrain component by a ray
		*
		*  @param[in]  ray
		*    Represent the ray for intersection // TODO(co) Which coordinate system?
		*  @param[out] position
		*    If not a null pointer, if the a hit was found, we get the position of intersecting
		*
		*  @return
		*    True if a hit with the terrain component was found
		*/
		bool getTerrainHitByRay(const Ogre::Ray& ray, glm::vec3* position) const;

		/**
		*  @brief
		*    Return whether a ray intersects the terrain bounding
		*
		*  @param[in]  ray
		*    Represent the world space ray for intersection
		*  @param[out] closestDistance
		*    If not a null pointer, receives the distance along the ray at which it intersects, not touched in case there's no hit
		*
		*  @return
		*    True if a hit with the terrain component bounding was found
		*/
		bool getTerrainHitBoundingBoxByRay(Ogre::Ray& ray, float* closestDistance = nullptr) const;

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the terrain globals
		*
		*  @return
		*    Terrain globals, can be a null pointer, do not destroy the instance
		*/
		inline Ogre::TerrainGlobalOptions* getTerrainGlobals() const;

		/**
		*  @brief
		*    Get the OGRE terrain group
		*
		*  @return
		*    OGRE terrain group, can be a null pointer, do not destroy the instance
		*/
		inline Ogre::TerrainGroup* getOgreTerrainGroup() const;

		/**
		*  @brief
		*    Get the OGRE terrain
		*
		*  @return
		*    OGRE terrain, can be a null pointer, do not destroy the instance
		*/
		Ogre::Terrain* getOgreTerrain() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void setOgreSceneNodeVisibility(bool visible) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onSettingsPropertyChanged(const ParameterGroup& parameterGroup, uint32 property);
		void updateOgreTerrainMaxPixelError();
		void onAssetsMounted(const Assets& assets);
		void onAssetsUnmounted(const Assets& assets);
		void onAssetChanged(const Asset& asset);
		void loadTerrainDefinition();
		void defineTerrain();
		void removeAllOgreTerrains();
		void buildHeightMap();
		void setPosition(const glm::vec3& position);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		uint32						mHeightMapSize;
		uint32						mColorMapSize;
		uint32						mBlendMapSize;
		int							mTerrainChunksPerEdge;			///< Size of chunks per edge. E.g. 8 means, there are 8x8 chunks
		float						mTerrainWorldSize;
		float						mSkirtSize;
		float						mMaxPixelError;
		AssetProxy					mTerrainAsset;
		// Internal only
		Ogre::TerrainGlobalOptions* mOgreTerrainGlobalOptions;		///< OGRE terrain globals instance, can be a null pointer, to not destroy the instance
		Ogre::TerrainGroup*			mOgreTerrainGroup;				///< OGRE terrain group instance, can be a null pointer
		TerrainDefinition*			mTerrainDefinition;				///< Can be a null pointer, we're responsible for destroying the instance
		std::set<GlobalAssetId>		mGlobalTerrainAssetIds;			///< Global asset IDs that affect this component
		bool						mIsEditing;


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
#include "qsf/renderer/terrain/TerrainComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::TerrainComponent)

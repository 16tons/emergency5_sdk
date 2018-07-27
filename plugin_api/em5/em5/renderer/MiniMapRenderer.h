// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/renderer/MiniMapIcons.h"

#include <qsf/math/Color4.h>
#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/audio/AudioProxy.h>
#include <qsf/reflection/CampClass.h>
#include <qsf/renderer/component/CameraComponent.h>

#include <Rocket/Core/Types.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <boost/container/flat_map.hpp>

#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Color4;
	class AssetProxy;
	class PolygonComponent;
}
namespace Ogre
{
	class RenderTexture;
	class Camera;
	class Node;
	class ManualObject;
	class SceneManager;
	class SceneNode;
	class Vector3;
	class ColourValue;
	class CompositorWorkspace;
	namespace v1
	{
		class Entity;
		class Billboard;
		class BillboardSet;
	}
}
namespace Rocket
{
	namespace Core
	{
		struct Texture;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Definitions                                           ]
	//[-------------------------------------------------------]
	enum MiniMapIconBucket
	{
		MINIMAP_ICONBUCKET_FIRE,
		MINIMAP_ICONBUCKET_VICTIMS,
		MINIMAP_ICONBUCKET_UNITS,
		MINIMAP_ICONBUCKET_VEHICLES,
		MINIMAP_ICONBUCKET_OTHER,
		MINIMAP_ICONBUCKET_OTHER_UNIT,
		MINIMAP_ICONBUCKET_PING_ANIMATED,
		MINIMAP_ICONBUCKET_PING,
		MINIMAP_ICONBUCKET_BLINK,
		_MINIMAP_ICONBUCKET_MAX
	};


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Minimap rendering class
	*
	*  @note
	*    - For rendering inside the game, not the minimap creation in the editor
	*    - One entity can only have one icon on the minimap
	*/
	class EM5_API_EXPORT MiniMapRenderer : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint64 PATH_VISUALIZATION_DESTINATION_ICON_ENTITY_ID;	///< Path visualization destination icon dummy entity ID
		typedef uint64 IconId;	///< Usually the icon ID is identical to the entity ID (but there are some icon types which don't have a corresponding entity to start with)
		typedef std::unordered_map<IconId, Ogre::v1::Billboard*> RendererIcons;	///< The essence used by the renderer to create the visible result (basically OGRE billboard), synchronized with the given visual icon data

		/**
		*  @brief
		*    The icon state data contains everything needed for deriving the renderer icon (basically OGRE billboard set, the thing seen by the user)
		*/
		struct IconState
		{
			uint64			entityId;			///< ID of the entity the icon represents, for visual mini map playback just used as key
			uint32			playerId;			///< ID of the player the entity the icon represents belongs to, can be used for e.g. icon color changes or filtering
			MiniMapIconType miniMapIconType;	///< The mini map icon type determines the icon image to be used
			glm::vec3		position;			///< World space position of the icon (not icon GUI 2D position)
			float			size;				///< Icon size (not final GUI size), can be the result of a fixed icon size animation over time
			qsf::Color4		color;				///< Icon color (usually white)
			bool			visible;			///< "qsf::game::HiddenComponent::isEntityVisible()"
			bool			dirty;				///< Set this to "true" to enforce reset of renderer icon (e.g. when only minimap icon type has hanged)
		};
		typedef std::unordered_map<uint64, IconState> IconStates;	///< Key = entity ID

		/**
		*  @brief
		*    The live icon state holds the icon state of the current game (independent of the stuff the user sees on the mini map), this includes visual icon data as well as icon animation related data
		*/
		struct LiveIconState : public IconState
		{
			float fixedSize;	///< The icon size as specified from the outside, without animation incorporated
			float elapsedTime;
			bool  inAnimation;
		};
		typedef std::unordered_map<uint64, LiveIconState> LiveIconStates;	///< Key = entity ID

		struct MiniMapPolygon
		{
			Ogre::v1::Entity*	mSceneEntity;
			Ogre::SceneNode*	mSceneNode;
			uint64				mEntityId;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit MiniMapRenderer(const glm::vec2& viewportSize);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MiniMapRenderer();

		inline const LiveIconStates& getLiveIconStates() const;
		void setExternalIconStates(const IconStates* iconStates);

		inline qsf::Map* getMap();
		bool setMap(qsf::Map& map, const qsf::AssetProxy& miniMapAssetProxy);
		void setViewportSize(const glm::vec2& viewportSize);
		void update(float timeDelta);
		void render();
		Ogre::v1::BillboardSet* getBillboardSetByMiniMapIconType(MiniMapIconType type) const;

		/**
		*  @brief
		*    Sets an asset and its size for the minimap icons
		*
		*  @param[in] asset
		*    The asset to be used for the minimap icons (must be an texture asset)
		*  @param[in] atlasSize
		*    The size of the icon atlas (size must be > 0)
		*  @param[in] iconSize
		*    The size of an icon inside the atlas (size must be > 0 and < atlasSize)
		*  @param[in] scaling
		*    The scale factor for the icons (must be > 0.0f)
		*
		*  @note
		*    - If one of the above parameters is invalid (e.g. asset is unknown) the default values are set instead
		*/
		void setMinimapIconAssetAndSizeAndScale(const qsf::AssetProxy& asset, int atlasSize, int iconSize, float scaling);

		/**
		*  @brief
		*    Resets the used icon atlas to default values
		*/
		void resetMinimapIconAssetAndSizeAndScale();

		//[-------------------------------------------------------]
		//[ View                                                  ]
		//[-------------------------------------------------------]
		inline Rocket::Core::Texture* getMiniMapTexture() const;

		inline void setPathVisualizationEntityId(uint64 entityId);

		void setCamera(qsf::CameraComponent* camera);
		inline bool getCameraQuadVisible() const;
		void setCameraQuadVisible(bool visible);

		bool isVisible(MiniMapIconBucket iconBucket) const;
		void toggleVisibility(MiniMapIconBucket iconBucket);

		void moveCameraToTarget(const glm::vec3& worldPosition);

		inline float getZoomFactor() const;
		void setZoomFactor(float zoomFactor);

		inline float getMinZoomFactor() const;
		inline void setMinZoomFactor(float zoomFactor);

		inline float getMaxZoomFactor() const;
		inline void setMaxZoomFactor(float zoomFactor);

		inline void zoomIn();
		inline void zoomOut();

		inline bool getUseViewportSizeBlackBars() const;
		inline void setUseViewportSizeBlackBars(bool useViewportSizeBlackBars);

		// Use gray icons for units of different player?
		inline bool getUnitOfDifferentPlayerHandling() const;
		inline void setUnitOfDifferentPlayerHandling(bool unitOfDifferentPlayerHandling);

		inline bool getViewportFollowsGameCamera() const;
		inline void setViewportFollowsGameCamera(bool viewportFollowsGameCamera);

		glm::vec3 getWorldPositionFromViewportPosition(const glm::vec2& viewportPosition) const;

		inline const glm::vec2& getCurrentMinimapCameraPosition() const;
		void moveMinimapCamera(const glm::vec2& newPosition);

		//[-------------------------------------------------------]
		//[ Controller                                            ]
		//[-------------------------------------------------------]
		// Icon
		bool addIcon(uint64 entityId, MiniMapIconType type, float scaleOrMaxSize, const qsf::Color4& color = qsf::Color4::WHITE);
		void removeIcon(uint64 entityId);

		// Polygon
		bool addPolygon(uint64 entityId, const qsf::Color4& color, bool updatePosition);
		bool addPolygon(uint64 entityId, const qsf::Color4& color, const std::vector<glm::vec3>& vertices);
		void removePolygon(uint64 entityId);

		// Ping/highlight
		void addPingAtWorldPosition(const glm::vec3& worldPosition, bool cyclesAnimation, bool playSound);
		void addHighlightAtWorldPosition(const glm::vec3& worldPosition, float scaleOrMaxSize);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool addIconDynamicInternal(uint64 entityId, MiniMapIconType miniMapIconType, float scaleOrMaxSize, const qsf::Color4& color, const glm::vec3& worldPosition);
		bool addIconInternal(uint64 entityId, MiniMapIconType type, float scaleOrMaxSize, const qsf::Color4& color, const glm::vec3& worldPosition = glm::vec3());
		void buildPolygon(const qsf::PolygonComponent& polygonComponent, const qsf::Color4& color, bool updatePosition);
		void buildPolygon(uint64 entityId, const std::vector<glm::vec3>& vertices, const qsf::Transform& transform, const qsf::Color4& color, bool updatePosition);
		void removeAllPolygons();
		void createBackground();
		void createCameraQuad();
		void updateCameraQuad();
		void updateCamera();
		void updateViewport(const glm::vec2& targetPosition);
		void updateIcons(float timeDelta);
		void updateLiveIconStatePosition(const qsf::Entity& entity, LiveIconState& liveIconState);
		void updateLiveIconStateVisibility(const qsf::Entity& entity, LiveIconState& liveIconState);
		void createPathVisualization();
		void updatePathVisualization();
		float getIconScaleFactor() const;	///< Calculates the icon scale factor, to show the icon pixel-precise on max zoom level
		void viewportSizeUpdated();
		void createRenderTarget();
		void destroyRenderTarget();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const IconStates* mExternalIconStates;	///< External icon states, can be a null pointer, we don't own the memory

		// Map
		qsf::Map*	mMap;
		glm::vec3	mMapCenter;		///< Map center in world space
		glm::vec3	mMapSize;		///< Map size in world space

		// Minimap icons atlas
		int mMinimapIconAtlasSize;
		int mMinimapIconSize;
		float mIconScaleFactor;

		// Mini-map camera
		glm::vec2	mCameraPosition;
		glm::vec2	mCameraWindowSize;
		glm::vec2	mMaximumCameraWindowSize;
		float		mZoomFactor;
		float		mMinZoomFactor;
		float		mMaxZoomFactor;

		// Viewport
		bool		mUseViewportSizeBlackBars;
		glm::vec2	mViewportSize;

		// Game camera
		qsf::WeakPtr<qsf::CameraComponent>	mGameCamera;
		float								mLastCameraDistance;
		glm::quat							mLastCameraOrientation;
		float								mLastMinimapZoom;

		// Rendering
		Rocket::Core::Texture*	   mRocketTexture;
		Ogre::RenderTexture*	   mRenderTarget;
		Ogre::CompositorWorkspace* mOgreCompositorWorkspace;
		Ogre::v1::BillboardSet*	   mDynamicBillboardSet[_MINIMAP_ICONBUCKET_MAX];
		Ogre::SceneManager*		   mSceneManager;

		// Background
		Ogre::v1::Entity* mBackgroundEntity;

		// Path visualization
		uint64				mPathVisualizationEntityId;
		Ogre::ManualObject* mOgreManualObjectPathVisualization;
		bool				mPathVisualizationDestinationIconEnabled;
		glm::vec3			mPathVisualizationDestinationIconPosition;

		// Camera visualization
		Ogre::SceneNode*  mCameraMeshNode;
		Ogre::v1::Entity* mCameraEntity;
		Ogre::Camera*	  mCamera;
		bool			  mCameraQuadVisible;

		// Icons and polygons
		RendererIcons  mRendererIcons;
		LiveIconStates mLiveIconStates;
		std::unordered_map<uint64, MiniMapPolygon>	mPolygons;

		// Misc
		float			mPingGrowthTime;
		float			mPingPinVanishTime;
		uint64			mPingIdCounter;
		qsf::AudioProxy	mPingAudioProxy;
		bool			mUnitOfDifferentPlayerHandling;
		bool			mViewportFollowsGameCamera;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/renderer/MiniMapRenderer-inl.h"

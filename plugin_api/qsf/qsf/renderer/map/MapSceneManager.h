// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/Time.h"
#include "qsf/renderer/map/SceneManagerBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
	class FrameStats;
	class ManualObject;
}
namespace qsf
{
	class Map;
	class FlareManager;
	class FlareComponent;
	class FlareOcclusionBillboardSet;
	class OgreManualObjectCacheManager;
	class MovableObjectRenderingListenerManager;
	class CompositorWorkspaceConfiguratorManager;
	class ParticleUniverseParticleSystemInstanceManager;
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
	*    Specialized QSF OGRE map scene manager base class (no factory by intent)
	*/
	class QSF_API_EXPORT MapSceneManager : public SceneManagerBase
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct MovableObjectWithListener
		{
			Ogre::MovableObject* ogreMovableObject;
			Ogre::Camera*		 ogreCamera;
			MovableObjectWithListener() :
				ogreMovableObject(nullptr),
				ogreCamera(nullptr) {}
			MovableObjectWithListener(Ogre::MovableObject* _ogreMovableObject, Ogre::Camera* _ogreCamera) :
				ogreMovableObject(_ogreMovableObject),
				ogreCamera(_ogreCamera) {}
		};
		typedef Ogre::FastArray<MovableObjectWithListener> MovableObjectWithListenerArray;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		MapSceneManager(const Ogre::String& instanceName, size_t numWorkerThreads, Ogre::InstancingThreadedCullingMethod threadedCullingMethod);
		virtual ~MapSceneManager();

		inline Map& getMap() const;
		void setMap(Map& map);

		// This is more or less a debugging feature because when moving the camera one can get into new cells which have not updated transforms
		inline bool getUpdateAnimationsAndTransforms() const;
		inline void setUpdateAnimationsAndTransforms(bool updateAnimationsAndTransforms);

		inline bool getAllowShowBoundingBoxes() const;
		inline void setAllowShowBoundingBoxes(bool allowShowBoundingBoxes);
		FlareOcclusionBillboardSet* createFlareOcclusionBillboardSet(FlareComponent& flareComponent, Ogre::HardwareOcclusionQuery& mOgreHardwareOcclusionQuery);

		//[-------------------------------------------------------]
		//[ Manager                                               ]
		//[-------------------------------------------------------]
		inline MovableObjectRenderingListenerManager& getMovableObjectRenderingListenerManager() const;
		inline OgreManualObjectCacheManager& getOgreManualObjectCacheManager() const;
		inline FlareManager& getFlareManager() const;
		inline ParticleUniverseParticleSystemInstanceManager& getParticleUniverseParticleSystemInstanceManager() const;
		void clearObjectCache();

		//[-------------------------------------------------------]
		//[ Frame rendering                                       ]
		//[-------------------------------------------------------]
		void renderFrame(Ogre::Camera* ogreCamera, Ogre::RenderTarget& ogreRenderTarget, bool isSecondEye = false);
		void renderFrame(Ogre::Camera* ogreCamera, Ogre::RenderTarget& ogreRenderTarget, Ogre::CompositorWorkspace& ogreCompositorWorkspace, bool isSecondEye = false);
		inline bool isSceneRenderingInProgress() const;
		inline Ogre::Camera* getMainOgreCamera() const;

		/**
		*  @brief
		*    Return the timestamp of last visibility determination
		*
		*  @return
		*    The timestamp
		*/
		inline const Time& getLastRenderedTimestamp() const;

		/**
		*  @brief
		*    Only contains OGRE movable objects of the primary rendering (e.g. shadow map rendering excluded)
		*/
		inline const MovableObjectWithListenerArray& getMovableObjectWithListenerArray() const;

		/**
		*  @brief
		*    Get the frame statistics
		*/
		inline const Ogre::FrameStats& getFrameStats() const;

		/**
		*  @brief
		*    Get the number of OGRE movable objects rendered in the last update call
		*/
		inline uint32 getNumberOfRenderedOgreMovableObjects() const;

		/**
		*  @brief
		*    Get the number of batches rendered in the last update call
		*/
		inline uint32 getNumberOfRenderedBatches() const;

		/**
		*  @brief
		*    Get the number of triangles rendered in the last update call
		*/
		inline uint32 getNumberOfRenderedTriangles() const;


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::SceneManager methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void destroyCamera(Ogre::Camera* cam) override;
		virtual void destroyAllEntities() override;
		virtual Ogre::v1::Entity* createEntity(const Ogre::String& meshName, const Ogre::String& groupName = Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::SceneMemoryMgrTypes sceneType = Ogre::SCENE_DYNAMIC) override;
		virtual Ogre::v1::Entity* createEntity(const Ogre::v1::MeshPtr& pMesh, Ogre::SceneMemoryMgrTypes sceneType = Ogre::SCENE_DYNAMIC) override;
		virtual void _cullPhase01(Ogre::Camera* camera, const Ogre::Camera* lodCamera, Ogre::Viewport* vp, uint8 firstRq, uint8 lastRq) override;
		virtual void _renderPhase02(Ogre::Camera* camera, const Ogre::Camera* lodCamera, Ogre::Viewport* vp, Ogre::uint8 firstRq, Ogre::uint8 lastRq, bool includeOverlays) override;
		virtual_l1 void updateSceneGraph() override;
		virtual const Ogre::Pass* _setPass(const Ogre::Pass* pass, bool evenIfSuppressed = false, bool shadowDerivation = true) override;
		virtual void _notifyMovableObjectDestroyed(Ogre::MovableObject& movableObject) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void pushOgreManualObjectLine(const Ogre::Vector3& start, const Ogre::Vector3& end, const Ogre::ColourValue& ogreColourValue = Ogre::ColourValue::White);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MapSceneManager methods        ]
	//[-------------------------------------------------------]
	protected:
		virtual void pushOgreManualObjectLines();


	//[-------------------------------------------------------]
	//[ Protected virtual Ogre::SceneManager methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual void setViewport(Ogre::Viewport* vp) override;
		virtual_l1 void renderSingleObject(Ogre::Renderable* rend, const Ogre::Pass* pass, bool lightScissoringClipping, bool doLightIteration) override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		bool mDisableTerrainLod;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void beginFrameRendering(Ogre::Camera* ogreCamera, bool isSecondEye = false);
		void updateSceneGraphInternal(bool isSecondEye);
		void endFrameRendering();


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static uint32 TotalNumberOfRenderedFrames;	///< Over all instances global frame counter


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Map*					  mMap;								///< Owner map instance, needs to be set directly after construction so considered to be always valid, do not destroy the instance
		Ogre::ObjectMemoryManager mLodEntityMemoryManager;
		bool					  mUpdateAnimationsAndTransforms;	///< For debugging
		bool					  mAllowShowBoundingBoxes;			///< For debugging
		// Manager
		CompositorWorkspaceConfiguratorManager&			mCompositorWorkspaceConfiguratorManager;
		Ogre::ManualObject*								mDebugOgreManualObjectLines;
		MovableObjectRenderingListenerManager*			mMovableObjectRenderingListenerManager;			///< Movable object rendering listener manager instance, always valid, we're responsible for destroying the instance
		OgreManualObjectCacheManager*					mOgreManualObjectCacheManager;					///< OGRE manual object cache manager, always valid, we're responsible for destroying the instance
		FlareManager*									mFlareManager;									///< Flare manager instance, always valid, we're responsible for destroying the instance
		ParticleUniverseParticleSystemInstanceManager*	mParticleUniverseParticleSystemInstanceManager;
		// Frame rendering
		bool							mSceneRenderingInProgress;
		Ogre::Camera*					mMainOgreCamera;			///< Main OGRE camera used during rendering, not the shadow mapping camera
		Time							mLastRenderedTimestamp;
		MovableObjectWithListenerArray	mMovableObjectWithListenerArray;
		Ogre::FrameStats*				mFrameStats;
		uint32							mNumberOfRenderedOgreMovableObjects;
		uint32							mNumberOfRenderedBatches;	///< The number of batches rendered in the last update call
		uint32							mNumberOfRenderedTriangles;	///< The number of triangles rendered in the last update call
		bool							mFirstPushOgreManualObjectLineCall;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/map/MapSceneManager-inl.h"

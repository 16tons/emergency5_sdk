// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <OGRE/Compositor/Pass/OgreCompositorPass.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
	class Vector3;
	class RenderQueue;
	class MovableObject;
}
namespace qsf
{
	class LightComponent;
	class GridSceneManager;
	class OgreCompositorPassDefDeferredLight;
	class OgreCompositorPassFactoryDeferredLight;
	namespace compositing
	{
		class CompositingDebugGroup;
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
	*    Deferred light OGRE compositor pass
	*
	*  @remarks
	*    This is the class that will send the actual render calls of the spheres (point lights),
	*    cones (spotlights) and quads (directional lights) after the GBuffer has been constructed.
	*/
	class OgreCompositorPassDeferredLight : public Ogre::CompositorPass
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		OgreCompositorPassDeferredLight(OgreCompositorPassFactoryDeferredLight& ogreCompositorPassFactoryDeferredLight, const OgreCompositorPassDefDeferredLight* definition, Ogre::CompositorNode* parentNode, const Ogre::CompositorChannel& target, Ogre::SceneManager* sceneManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreCompositorPassDeferredLight();


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::CompositorPass methods           ]
	//[-------------------------------------------------------]
	public:
		virtual void execute(const Ogre::Camera* lodCamera) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void fillOgreRenderQueue(Ogre::RenderQueue& ogreRenderQueue, Ogre::Camera& ogreCamera);
		void renderLight(Ogre::RenderQueue& ogreRenderQueue, LightComponent& lightComponent);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		GridSceneManager*						mGridSceneManager;
		compositing::CompositingDebugGroup&		mCompositingDebugGroup;
		OgreCompositorPassFactoryDeferredLight&	mOgreCompositorPassFactoryDeferredLight;
		Ogre::MovableObject*					mOgreMovableObject;	///< OGRE movable object, can be a null pointer, we're responsible for destroying the instance in case we no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

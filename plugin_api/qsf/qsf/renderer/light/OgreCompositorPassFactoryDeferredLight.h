// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/compositor/OgreCompositorPassFactory.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class IndexData;
		class VertexData;
	}
}
namespace qsf
{
	class LightMaterialGenerator;
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
	*    Deferred light OGRE compositor pass factory pluginable
	*/
	class OgreCompositorPassFactoryDeferredLight : public OgreCompositorPassFactory
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class DeferredLight;
		friend class OgreCompositorPassDeferredLight;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;	///< "qsf::compositing::OgreCompositorPassFactoryDeferredLight" unique pluginable view ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit OgreCompositorPassFactoryDeferredLight(OgreCompositorPassProvider* ogreCompositorPassProvider);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OgreCompositorPassFactoryDeferredLight();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::OgreCompositorPassFactory methods ]
	//[-------------------------------------------------------]
	protected:
		virtual Ogre::CompositorPassDef* addPassDef(Ogre::uint32 rtIndex, Ogre::CompositorNodeDef* parentNodeDef) override;
		virtual Ogre::CompositorPass* addPass(const Ogre::CompositorPassDef* definition, Ogre::Camera* defaultCamera, Ogre::CompositorNode* parentNode, const Ogre::CompositorChannel& target, Ogre::SceneManager* sceneManager) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		LightMaterialGenerator* mLightMaterialGenerator;	///< The material generator for the light geometry, always valid, destroy the instance if you no longer need it
		// Vertex and index data
		Ogre::v1::VertexData*	mDirectionalOgreVertexData;
		Ogre::v1::VertexData*	mPointOgreVertexData;
		Ogre::v1::IndexData*	mPointOgreIndexData;
		Ogre::v1::VertexData*	mSpotOgreVertexData;
		Ogre::v1::IndexData*	mSpotOgreIndexData;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::OgreCompositorPassFactoryDeferredLight)

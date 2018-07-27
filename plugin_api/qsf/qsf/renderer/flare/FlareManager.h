// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/platform/PlatformTypes.h"

#include <OGRE/OgreMovableObject.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class FlareComponent;
	namespace detail
	{
		class FlareOgreMovableObjectListener;
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
	*    Flare manager
	*/
	class FlareManager : public Ogre::MovableObject::Listener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class detail::FlareOgreMovableObjectListener;	// Needs to call "qsf::FlareManager::objectRendering()"


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit FlareManager(Map& map);
		virtual ~FlareManager();


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::MovableObject::Listener methods  ]
	//[-------------------------------------------------------]
	public:
		virtual bool objectRendering(const Ogre::MovableObject* movableObject, Ogre::RenderQueue* queue, Ogre::Camera* camera, const Ogre::Camera* lodCamera) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onObjectRendering(FlareComponent& flareComponent, const Ogre::MovableObject& ogreMovableObject, const Ogre::Camera& ogreCamera);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Map& mMap;	///< Owner map, do not destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

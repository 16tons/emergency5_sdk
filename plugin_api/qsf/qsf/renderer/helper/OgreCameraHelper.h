// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Viewport;
	class Camera;
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
	*    Static helper class providing OGRE camera utility functions
	*/
	class QSF_API_EXPORT OgreCameraHelper
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the currently active OGRE camera
		*
		*  @return
		*    The current OGRE camera instance of the main map, or null pointer if none is active
		*
		*  @remarks
		*    You can safely use this function inside the Ogre::RenderQueueListener callback "renderQueueStarted()".
		*/
		static Ogre::Camera* getCurrentOgreCamera();

		/**
		*  @brief
		*    Project a world space position into clip space
		*
		*  @param[in] ogreCamera
		*    The OGRE camera instance to use for the projection
		*  @param[in] worldPosition
		*    World space position to project
		*  @param[out] outClipSpacePosition
		*    Resulting clip space position, with coordinates in [-1.0f, 1.0f]
		*
		*  @return
		*    "true" if projection was successful, "false" if the world space position was behind the camera
		*/
		static bool projectWorldToClipSpace(const Ogre::Camera& ogreCamera, const glm::vec3& worldPosition, glm::vec2& outClipSpacePosition);

		/**
		*  @brief
		*    Convert a clip space position into window coordinates
		*
		*  @param[in] ogreViewport
		*    The OGRE viewport instance to use for the conversion
		*  @param[in] clipSpacePosition
		*    Clip space position to convert, with coordinates in [-1.0f, 1.0f]
		*
		*  @return
		*    The resulting window space position, in pixels starting from the upper left viewport corner
		*/
		static glm::vec2 convertClipToWindowSpace(Ogre::Viewport& ogreViewport, const glm::vec2& clipSpacePosition);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

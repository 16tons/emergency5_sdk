// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawProxy.h"
#include "qsf/math/Color3.h"
#include "qsf/math/Color4.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <boost/noncopyable.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
	class ManualObject;
}
namespace qsf
{
	class Map;
	namespace detail
	{
		class FastDebugDrawOgreCameraListener;
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
	*    Fast debug drawing class
	*/
	class QSF_API_EXPORT FastDebugDraw : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class detail::FastDebugDrawOgreCameraListener;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const char* DEFAULT_MATERIAL;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit FastDebugDraw(Map& map);
		~FastDebugDraw();	// Not virtual by intent

		//[-------------------------------------------------------]
		//[ Draw                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Draws a line from point0 to point1 with a chosen color
		*/
		void drawLine(const glm::vec3& point0, const glm::vec3& point1, const Color3& color);

		/**
		*  @brief
		*    Draws a triangle between point0, point1 and point2 with a chosen color and alpha transparency
		*/
		void drawTriangle(const glm::vec3& point0, const glm::vec3& point1, const glm::vec3& point2, const Color4& color);

		/**
		*  @brief
		*    Draws text at worldSpacePosition
		*/
		void drawText(const std::string& text, const glm::vec3& worldSpacePosition);

		//[-------------------------------------------------------]
		//[ Frame rendering start/end                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Finish the debug and remove the listener, now the camera can render its frame again
		*    TODO(tl): Not sure if the definitions (Start, End) are not swapped around
		*/
		bool renderFrameStarted();

		/**
		*  @brief
		*    Get the actual camera and connect it with our listener to ensure
		*    TODO(tl): Not sure if the definitions (Start, End) are not swapped around
		*/
		bool renderFrameEnded();

		//[-------------------------------------------------------]
		//[ Query                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check visibility for points, lines and spheres
		*/
		bool isPointVisibleOnScreen(const glm::vec3& point0) const;
		bool isLineVisibleOnScreen(const glm::vec3& point0, const glm::vec3& point1) const;
		bool isSphereVisibleOnScreen(const glm::vec3& center, float radius) const;

		/**
		*  @brief
		*    Return the ground map height at position
		*/
		float getGroundMapHeight(const glm::vec3& position);

		//[-------------------------------------------------------]
		//[ Camera                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the OGRE camera used for rendering; may be a null pointer
		*/
		Ogre::Camera* getOgreCamera() const;

		/**
		*  @brief
		*    Return the position of our camera
		*/
		const glm::vec3& getCameraPosition() const;

		/**
		*  @brief
		*    Return the rotation of our camera
		*/
		const glm::quat& getCameraRotation() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Ogre::ManualObject*	mOgreManualObjectLines;
		Ogre::ManualObject*	mOgreManualObjectTriangles;
		Map&				mMap;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const float MAXIMUM_DISTANCE;
		static const float MAXIMUM_SQUARED_DISTANCE;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DebugDrawProxy							 mTextDebugDrawProxy;
		Ogre::Camera*							 mOgreCamera;
		detail::FastDebugDrawOgreCameraListener* mFastDebugDrawOgreCameraListener;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

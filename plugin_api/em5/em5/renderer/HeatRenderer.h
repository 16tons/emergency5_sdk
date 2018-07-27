// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/job/JobProxy.h>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class RenderTexture;
	class MovableObject;
	class SceneNode;
	class ManualObject;
}
namespace qsf
{
	class Map;
	class Entity;
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
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Heat renderer
	*/
	class EM5_API_EXPORT HeatRenderer
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const char RENDER_TARGET_NAME[];
		static const char MATERIAL_NAME[];


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
		explicit HeatRenderer(const glm::vec2& renderTargetSize);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HeatRenderer();

		void initViewport();

		void setMap(qsf::Map& map);

		Rocket::Core::Texture* getHeatTexture() const;

		inline void setRenderingEnabled(bool renderingEnabled);

		void setCameraPosition(const glm::vec3& position);
		void setCameraOrientation(const glm::quat& orientation);

		// TODO(mk) It looks like the camera target isn't used at all
		void setCameraTarget(uint64 entityId);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		void drawAABB(const qsf::Entity& entity);
		void setAABBVisible(bool visible);

		void update(float timeDelta);
		void render();

		bool toScreenCoordinatesAABB(const qsf::Entity& entity, glm::vec2& center, glm::vec2& extents) const;
		bool toScreenCoordinatesAABB(Ogre::MovableObject& ogreMovableObject, glm::vec2& center, glm::vec2& extents) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool					mRenderingEnabled;
		qsf::Entity*			mCameraEntity;
		uint64					mTargetEntityId;	// TODO(mk) It looks like the camera target isnt used at all
		bool					mTargetFocusEnabled;
		glm::vec2				mRenderTargetSize;
		Ogre::RenderTexture*	mOgreRenderTexture;
		Ogre::ManualObject*		mOgreManualObject;
		Ogre::SceneNode*		mOgreSceneNode;
		Rocket::Core::Texture*	mRocketTexture;
		qsf::Map*				mMap;

		// Job proxies for regular update
		qsf::JobProxy mAnimationJobProxy;	///< Regular animation job proxy; for updates once a frame


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/renderer/HeatRenderer-inl.h"

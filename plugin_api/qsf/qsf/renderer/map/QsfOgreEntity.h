// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/time/Time.h"

#include <OGRE/OgreEntity.h>

#include <glm/vec3.hpp>


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
	*    Specialized QSF OGRE entity
	*/
	class QSF_API_EXPORT QsfOgreEntity : public Ogre::v1::Entity
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MapSceneManager;
		friend class QsfOgreEntityFactory;
		friend class InstancesBufferManager;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Allows setting a custom bounding box, which is returned instead of the default one
		*/
		void setCustomBoundingBox(const glm::vec3& localMinCorner, const glm::vec3& localMaxCorner);

		/**
		*  @brief
		*    Allows setting a custom bounding box, which is returned instead of the default one
		*
		*  @param[in] localExtends
		*    The extends factor on each axis the original bounding box is extended
		*/
		void setCustomBoundingBoxExtends(const glm::vec3& localExtends);

		/**
		*  @brief
		*    Resets the state that the custom bounding box should be used
		*/
		void resetCustomBoundingBox();

		/**
		*  @brief
		*    Return the timestamp of last visibility in any viewport
		*
		*  @return
		*    The timestamp
		*/
		inline const Time& getLastRenderedTimestamp() const;
		inline const Time& getLastNormalRenderedTimestamp() const;
		inline const Time& getLastShadowRenderedTimestamp() const;


	//[-------------------------------------------------------]
	//[ Public virtual Ogre::MovableObject methods            ]
	//[-------------------------------------------------------]
	public:
		virtual void _updateRenderQueue(Ogre::RenderQueue* queue, Ogre::Camera* camera, const Ogre::Camera* lodCamera) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline QsfOgreEntity(Ogre::IdType id, Ogre::ObjectMemoryManager* objectMemoryManager, Ogre::ObjectMemoryManager* lodObjectMemoryManager, Ogre::SceneManager* manager);
		inline QsfOgreEntity(Ogre::IdType id, Ogre::ObjectMemoryManager* objectMemoryManager, Ogre::ObjectMemoryManager* lodObjectMemoryManager, Ogre::SceneManager* manager, const Ogre::v1::MeshPtr& mesh);
		inline virtual ~QsfOgreEntity();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool		mIsCustomBoundingBoxSet;	///< Indicates if the custom bounding box should be used
		Ogre::Aabb	mCustomBoundingBox;			///< The custom bounding box
		glm::vec3	mLocalExtends;				///< The last set local extends value
		Time		mLastRenderedTimestamp[2];	///< 0 = normal, 1 = shadow rendering


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/map/QsfOgreEntity-inl.h"

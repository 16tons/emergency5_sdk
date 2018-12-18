// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class SceneNode;
	class SceneManager;
	class MovableObject;
	namespace v1
	{
		class Entity;
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
	*    Abstract renderer component class
	*
	*  @note
	*    - The external library "Object-Oriented Graphics Rendering Engine" (OGRE) ( http://www.ogre3d.org/ ) is used
	*/
	class QSF_API_EXPORT RendererComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "qsf::RendererComponent" unique component ID
		static const uint32 ACTIVE;						///< "Active" unique class property ID inside the class
		static const uint32 CAST_SHADOWS;				///< "CastShadows" unique class property ID inside the class
		static const uint32 RENDER_QUEUE_ID;			///< "RenderQueueId" unique class property ID inside the class
		static const uint32 RENDERING_DISTANCE_FACTOR;	///< "RenderingDistanceFactor" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Get the component associated with an OGRE movable object
		*
		*  @param[in] ogreMovableObject
		*    The OGRE movable object
		*
		*  @return
		*    The associated component, or null pointer if none id associated
		*/
		static Component* getComponentByOgreMovableObject(const Ogre::MovableObject& ogreMovableObject);


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
		inline explicit RendererComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~RendererComponent();

		/**
		*  @brief
		*    Return whether or not the render component casts shadows
		*
		*  @return
		*    "true" in case the renderer component casts shadow, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getCastShadows() const;

		/**
		*  @brief
		*    Set whether or not the render component casts shadows
		*
		*  @param[in] castShadows
		*    "true" in case the renderer component casts shadow, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setCastShadows(bool castShadows);

		/**
		*  @brief
		*    Return the render queue ID
		*
		*  @return
		*    Render queue ID, "qsf::getUninitialized<uint32>()" if the default value is not overwritten
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline uint32 getRenderQueueId() const;

		/**
		*  @brief
		*    Set the render queue ID
		*
		*  @param[in] renderQueueId
		*    New render queue ID, "qsf::getUninitialized<uint32>()" to not overwrite the default value
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRenderQueueId(uint32 renderQueueId);

		/**
		*  @brief
		*    Return the rendering distance factor
		*
		*  @return
		*    The rendering distance factor, 1< clip earlier, >1 clip later (negative performance impact)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getRenderingDistanceFactor() const;

		/**
		*  @brief
		*    Set the rendering distance factor
		*
		*  @param[in] renderingDistanceFactor
		*    The rendering distance factor, 1< clip earlier, >1 clip later (negative performance impact)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRenderingDistanceFactor(float renderingDistanceFactor);

		//[-------------------------------------------------------]
		//[ Geometric                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the world radius of the renderer component
		*
		*  @return
		*    The world radius of the renderer component
		*/
		float getWorldRadius() const;

		/**
		*  @brief
		*    Get the axis aligned bounding box in world space
		*
		*  @param[out] minimumPoint
		*    Reference to a vector where to write the bounding box's minimum point
		*  @param[out] maximumPoint
		*    Reference to a vector where to write the bounding box's maximum point
		*
		*  @return
		*    "true" if bounding box points were successfully written, "false" on error
		*/
		bool getWorldAxisAlignedBoundingBox(glm::vec3& minimumPoint, glm::vec3& maximumPoint) const;

		/**
		*  @brief
		*    Get the axis aligned bounding box in local space
		*
		*  @param[out] minimumPoint
		*    Reference to a vector where to write the bounding box's minimum point
		*  @param[out] maximumPoint
		*    Reference to a vector where to write the bounding box's maximum point
		*
		*  @return
		*    "true" if bounding box points were successfully written, "false" on error
		*/
		bool getLocalAxisAlignedBoundingBox(glm::vec3& minimumPoint, glm::vec3& maximumPoint) const;

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Get the OGRE scene manager instance
		*
		*  @return
		*    The OGRE scene manager instance, can be a null pointer, do no destroy the returned instance
		*/
		Ogre::SceneManager* getOgreSceneManager() const;

		/**
		*  @brief
		*    Update the visibility state of the internal OGRE scene node
		*/
		void updateOgreSceneNodeVisibility();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the OGRE scene node instance
		*
		*  @return
		*    The OGRE scene node instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access the OGRE scene node instance if you don't have to
		*    - The default implementation always returns a null pointer
		*/
		inline virtual Ogre::SceneNode* getOgreSceneNode() const;

		/**
		*  @brief
		*    Return the OGRE entity instance, if there are multiple the first one will be returned
		*
		*  @return
		*    The OGRE entity node instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access the OGRE entity node instance if you don't have to
		*    - The default implementation always returns a null pointer
		*/
		inline virtual Ogre::v1::Entity* getOgreEntity() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetActive(bool active) override;
		virtual void onSetDebug(bool debug) override;
		virtual void onSetHidden(bool hidden) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Set the visibility state of the internal OGRE scene node
		*
		*  @param[in] visible
		*    "true" if the OGRE scene node should be visible, else "false"
		*/
		virtual void setOgreSceneNodeVisibility(bool visible);

		/**
		*  @brief
		*    Update the rendering distance state of the internal OGRE movable object
		*/
		virtual void updateOgreObjectRenderingDistance() const;

		/**
		*  @brief
		*    Update the cast shadows state of the internal OGRE movable object
		*/
		virtual void updateOgreObjectCastShadows() const;

		/**
		*  @brief
		*    Return the ID of the default render queue to use
		*/
		virtual uint32 getDefaultRenderQueueId() const;

		/**
		*  @brief
		*    Update the render queue ID state of the internal OGRE movable object
		*
		*  @param[in] renderQueueId
		*    ID of the render queue to use
		*/
		virtual void updateOgreObjectRenderQueue(uint32 renderQueueId);

		/**
		*  @brief
		*    Update the flip culling state of the internal OGRE movable object
		*/
		virtual void updateOgreObjectFlipCulling();

		/**
		*  @brief
		*    Return whether or not the renderer component is grid managed
		*
		*  @return
		*    "true" if the renderer component is grid managed, else "false"
		*
		*  @note
		*    - By default, renderer components are not grid managed
		*/
		inline virtual bool isGridManaged() const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Create cell child scene node
		*
		*  @return
		*    The created cell child scene node instance, null pointer on error, destroy the instance if you no longer need it via the OGRE scene manager
		*
		*  @note
		*    - Transform component for scene node transform initialization is used when possible
		*/
		Ogre::SceneNode* createCellChildSceneNode();

		/**
		*  @brief
		*    Update the states controlled by this renderer component of the internal OGRE object
		*
		*  @note
		*    - You might want to call this method in derived classes after creating the OGRE object instance
		*/
		void updateOgreObject();

		/**
		*  @brief
		*    Check component flags to determine if this renderer component should be visible right now
		*
		*  @return
		*    "true" if the component should be visible, "false" if it should be hidden
		*/
		inline bool shouldBeVisible() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setOgreSceneNodeScale(Ogre::SceneNode& ogreSceneNode, const glm::vec3& scale);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		bool   mCastShadows;				///< "true" in case the renderer component casts shadow, else "false"
		uint32 mRenderQueueId;				///< ID of the render queue to use, "qsf::getUninitialized<uint32>()" to not overwrite the default value
		uint32 mDefaultRenderQueueId;		///< ID of the default render queue to use
		float  mRenderingDistanceFactor;	///< The rendering distance factor, 1< clip earlier, >1 clip later (negative performance impact)
		// Derived data
		bool mFlipCulling;	///< Flip OGRE renderable culling?


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
#include "qsf/renderer/component/RendererComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::RendererComponent)

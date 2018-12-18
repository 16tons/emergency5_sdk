// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/math/Ray.h"

#include <boost/noncopyable.hpp>

#include <string>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Layer;
	class Entity;
	class Transform;
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
	*    Entity helper class
	*
	*  @remarks
	*    Usage example:
	*    @code
	*    // qsf::Entity& qsfEntity = ...
	*    std::string name = qsf::EntityHelper(qsfEntity).getName();
	*    @endcode
	*/
	class QSF_API_EXPORT EntityHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] entity
		*    Entity to use, must stay valid as long as this entity helper instance exists
		*/
		explicit inline EntityHelper(Entity& entity);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		inline ~EntityHelper();

		/**
		*  @brief
		*    Return the encapsulated entity
		*
		*  @return
		*    The encapsulated entity
		*/
		inline Entity& getEntity() const;

		/**
		*  @brief
		*    Return the name of an entity by using the metadata component
		*
		*  @return
		*    UTF-8 name of the entity, can be an empty string (e.g. when there's no metadata component)
		*/
		std::string getName() const;

		//[-------------------------------------------------------]
		//[ Layer                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the layer the entity is registered to
		*
		*  @return
		*    The layer the entity is registered to, can be a null pointer, do not destroy the instance
		*
		*  @remarks
		*    Every entity which has a metadata component is automatically inside a layer. It's valid
		*    that an entity has no metadata component and hence is inside no layer (but this situation
		*    should be an exception and no common practice).
		*/
		Layer* getLayer() const;

		/**
		*  @brief
		*    Return the ID of the layer the entity is registered to
		*
		*  @return
		*    The ID of the layer the entity is registered to, "qsf::getUninitialized<uint32>()" if invalid (which is meaning that the entity has no metadata component)
		*
		*  @see
		*    - "qsf::EntityHelper::getLayer()"
		*/
		uint32 getLayerId() const;

		//[-------------------------------------------------------]
		//[ Transform                                             ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the world space transform of the entity
		*
		*  @return
		*    The world space transform of the entity
		*/
		const Transform& getTransform() const;

		/**
		*  @brief
		*    Return the world space position of the entity
		*
		*  @return
		*    The world space position of the entity, null vector in case the entity has no transform component
		*/
		const glm::vec3& getPosition() const;

		/**
		*  @brief
		*    Set the world space position of the entity
		*
		*  @param[in] position
		*    The world space position of the entity
		*
		*  @note
		*    - Creates an "qsf::TransformComponent" instance in case there's no one, yet
		*/
		void setPosition(const glm::vec3& position) const;

		/**
		*  @brief
		*    Transform a given world space position into a local space position relative to a given world space transform
		*
		*  @param[in] position
		*    World space position to convert to local space.
		*
		*  @return
		*    The resulting local space position, given input position in case the entity has no transform component
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates.
		*/
		glm::vec3 vec3PositionWorldToLocal(const glm::vec3& position) const;

		/**
		*  @brief
		*    Transform a given local space position relative to a given world space transform into a world space position
		*
		*  @param[in] position
		*    Local space position to convert to world space.
		*
		*  @return
		*    The resulting world space position, given input position in case the entity has no transform component
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates.
		*/
		glm::vec3 vec3PositionLocalToWorld(const glm::vec3& position) const;

		/**
		*  @brief
		*    Transform a given world space line into a local space line relative to a given world space transform
		*
		*  @param[in] line
		*    World space line to convert to local space.
		*
		*  @return
		*    The resulting local space line, given input line in case the entity has no transform component
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates.
		*/
		Line lineWorldToLocal(const Line& line) const;

		/**
		*  @brief
		*    Transform a given local space line relative to a given world space transform into a world space line
		*
		*  @param[in] line
		*    Local space line to convert to world space.
		*
		*  @return
		*    The resulting world space line, given input line in case the entity has no transform component
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates.
		*/
		Line lineLocalToWorld(const Line& line) const;

		/**
		*  @brief
		*    Transform a given world space ray into a local space ray relative to a given world space transform
		*
		*  @param[in] ray
		*    World space ray to convert to local space.
		*
		*  @return
		*    The resulting local space ray, given input ray in case the entity has no transform component
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates.
		*/
		Ray rayWorldToLocal(const Ray& ray) const;

		/**
		*  @brief
		*    Transform a given local space ray relative to a given world space transform into a world space ray
		*
		*  @param[in] ray
		*    Local space ray to convert to world space.
		*
		*  @return
		*    The resulting world space ray, given input ray in case the entity has no transform component
		*
		*  @note
		*    - This transform instance is used as reference transformation that defines local space coordinates and is expressed in world space coordinates.
		*/
		Ray rayLocalToWorld(const Ray& ray) const;

		//[-------------------------------------------------------]
		//[ Geometry                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return a suited sphere enclosing the entity
		*
		*  @param[out] center
		*    Receives the world space center of the suited sphere enclosing the entity
		*  @param[out] radius
		*    Receives the world space radius of the suited sphere enclosing the entity
		*
		*  @return
		*    "true" if meaningful information could be retrieved, else "false" if the method returns default values
		*
		*  @note
		*    - The purpose of this ease-of-use method is to return "anything", "anything" is e.g. sufficient for highlighting the entity
		*    - If you need something efficient and exact, don't use this ease-of-use method
		*/
		bool getSphere(glm::vec3& center, float& radius) const;

		/**
		*  @brief
		*    Return a suited axis aligned bounding box enclosing the entity
		*
		*  @param[out] minimumPoint
		*    Reference to a vector where to write the bounding box's world space minimum point
		*  @param[out] maximumPoint
		*    Reference to a vector where to write the bounding box's world space maximum point
		*
		*  @return
		*    "true" if meaningful information could be retrieved, else "false" if the method returns default values
		*
		*  @note
		*    - The purpose of this ease-of-use method is to return "anything", "anything" is e.g. sufficient for highlighting the entity
		*    - If you need something efficient and exact, don't use this ease-of-use method
		*/
		bool getAxisAlignedBoundingBox(glm::vec3& minimumPoint, glm::vec3& maximumPoint) const;

		/**
		*  @brief
		*    Return the bounding box extents of the entity
		*
		*  @param[out] extents
		*    Reference to a vector where to write the bounding box's extents
		*
		*  @return
		*    "true" if meaningful information could be retrieved, else "false" if the method returns default values
		*
		*  @note
		*    - The purpose of this ease-of-use method is to return "anything", "anything" is e.g. sufficient for highlighting the entity
		*    - If you need something efficient and exact, don't use this ease-of-use method
		*/
		bool getBoundingBoxExtents(glm::vec3& extents) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Entity& mEntity;	///< Entity to use, always valid, do not destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/map/EntityHelper-inl.h"

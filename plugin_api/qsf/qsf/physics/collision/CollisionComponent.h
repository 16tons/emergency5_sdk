// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/physics/PhysicsComponent.h"

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class DebugDrawProxy;
	class Color4;
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
	*    Contains collision information for the entity it is attached to
	*
	*  @remarks
	*    We plan to support several native formats and require that they can be converted to one another at this interface.
	*    This allows algorithms that are optimized to only work with a special implementation to use at least a rough version of the other formats.
	*    Such conversions should always overestimate the geometry meaning that it should be guaranteed that the original geometric description is fully contained inside each returned converted format.
	*    Collision components may be attached in two ways.
	*    If they are attached to an object than this is the complete collision representation available.
	*    If they are attached to child entities than all child collision components make up the collision geometry.
	*/
	class QSF_API_EXPORT CollisionComponent : public PhysicsComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::CollisionComponent" unique component ID
		static const uint32 ACTIVE;			///< "Active" unique class property ID inside the class
		static const uint32 GROUND_SPEED;	///< "GroundSpeed" unique class property ID inside the class

		enum Type
		{
			SPHERE,
			AXIS_ALIGNED_BOUNDING_BOX,
			ORIENTED_BOUNDING_BOX,
			TRIANGLE_MESH
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		// Return the native representation type of the collision data
		inline Type getGeometryType() const;

		// Add an adequate debug draw request for the collision geometry to the debug draw proxy passed
		void addDebugDrawing(DebugDrawProxy& debugDrawProxy, const Color4& color, bool drawFlat = false) const;

		const std::string& getGroundSpeed() const;
		void setGroundSpeed(const std::string& groundSpeed);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::CollisionComponent methods        ]
	//[-------------------------------------------------------]
	public:
		/** Request to retrieve the collision information in a specific format.
		* If this is not the native format it usually requires a conversion.
		* Returns whether the format was converted and the reference parameters were written or not.
		* All positions are already converted to global coordinates.
		*/
		//@{
		virtual bool getAsSphere(glm::vec3& center, float& radius) const = 0;
		virtual bool getAsAxisAlignedBoundingBox(glm::vec3& corner1, glm::vec3& corner2) const = 0;
		// The anchor point is the center of the box unlike the position of an entity that is usually at the center of the ground plane
		// The rotation is applied around that anchor point.
		// The extensions define the side length in local coordinates in all three dimensions.
		virtual bool getAsOrientedBoundingBox(glm::vec3& anchorPoint, glm::quat& rotation, glm::vec3& extensions) const = 0;
		//@}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline CollisionComponent(Prototype* prototype, Type type);

		// Helper function to access the position and rotation of an attached transform component to create final positions in derived classes.
		// Writes the values to the optional parameters if they are not null and returns whether the transform component was successfully accessed.
		bool tryGetRootTransform(glm::vec3* position, glm::quat* rotation = nullptr) const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Type		mType;			///< The native representation of the collision geometry
		std::string mGroundSpeed;	///< Ground speed


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


	// inline implementation
	inline CollisionComponent::CollisionComponent(Prototype* prototype, Type type) :
		PhysicsComponent(prototype),
		mType(type),
		mGroundSpeed("Barricade")
	{}

	inline CollisionComponent::Type CollisionComponent::getGeometryType() const
	{
		return mType;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::CollisionComponent);

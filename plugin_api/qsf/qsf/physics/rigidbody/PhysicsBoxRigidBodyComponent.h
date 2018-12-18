// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/physics/rigidbody/PhysicsRigidBodyComponent.h"

#include <glm/glm.hpp>


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
	*    Physics box rigid body component class
	*/
	class QSF_API_EXPORT PhysicsBoxRigidBodyComponent : public PhysicsRigidBodyComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::PhysicsBoxRigidBodyComponent" unique component ID
		static const uint32 HALF_EXTENTS;	///< "HalfExtents" unique class property ID inside the class


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
		inline explicit PhysicsBoxRigidBodyComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PhysicsBoxRigidBodyComponent();

		/**
		*  @brief
		*    Return the box half extents
		*
		*  @return
		*    The box half extents
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const glm::vec3& getHalfExtents() const;

		/**
		*  @brief
		*    Set the box half extents
		*
		*  @param[in] halfExtents
		*    The box half extents to set
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setHalfExtents(const glm::vec3& halfExtents);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PhysicsRigidBodyComponent methods ]
	//[-------------------------------------------------------]
	protected:
		virtual btCollisionShape* createBulletCollisionShape() const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		glm::vec3 mHalfExtents;	///< Extension of the collision box as half side lengths from the midpoint


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
#include "qsf/physics/rigidbody/PhysicsBoxRigidBodyComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PhysicsBoxRigidBodyComponent)

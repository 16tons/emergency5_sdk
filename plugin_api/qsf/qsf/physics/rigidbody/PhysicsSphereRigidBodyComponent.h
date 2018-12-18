// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/physics/rigidbody/PhysicsRigidBodyComponent.h"


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
	*    Physics sphere rigid body component class
	*/
	class QSF_API_EXPORT PhysicsSphereRigidBodyComponent : public PhysicsRigidBodyComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::PhysicsSphereRigidBodyComponent" unique component ID
		static const uint32 RADIUS;			///< "Radius" unique class property ID inside the class


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
		inline explicit PhysicsSphereRigidBodyComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PhysicsSphereRigidBodyComponent();

		/**
		*  @brief
		*    Return the sphere radius
		*
		*  @return
		*    The sphere radius
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getRadius() const;

		/**
		*  @brief
		*    Set the sphere radius
		*
		*  @param[in] radius
		*    The sphere radius to set
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRadius(float radius);


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
		float mRadius;	///< Radius


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
#include "qsf/physics/rigidbody/PhysicsSphereRigidBodyComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PhysicsSphereRigidBodyComponent)

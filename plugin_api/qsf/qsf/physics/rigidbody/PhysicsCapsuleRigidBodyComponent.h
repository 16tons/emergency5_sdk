// Copyright (C) 2012-2017 Promotion Software GmbH


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
	*    Physics capsule rigid body component class
	*/
	class QSF_API_EXPORT PhysicsCapsuleRigidBodyComponent : public PhysicsRigidBodyComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::PhysicsCapsuleRigidBodyComponent" unique component ID
		static const uint32 RADIUS;			///< "Radius" unique class property ID inside the class
		static const uint32 HEIGHT;			///< "Height" unique class property ID inside the class


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
		inline explicit PhysicsCapsuleRigidBodyComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PhysicsCapsuleRigidBodyComponent();

		/**
		*  @brief
		*    Return the capsule radius
		*
		*  @return
		*    The capsule radius
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getRadius() const;

		/**
		*  @brief
		*    Set the capsule radius
		*
		*  @param[in] radius
		*    The capsule radius to set
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRadius(float radius);

		/**
		*  @brief
		*    Return the capsule height
		*
		*  @return
		*    The capsule height
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getHeight() const;

		/**
		*  @brief
		*    Set the capsule height
		*
		*  @param[in] height
		*    The capsule height to set
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setHeight(float height);


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
		float mHeight;	///< Height


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
#include "qsf/physics/rigidbody/PhysicsCapsuleRigidBodyComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PhysicsCapsuleRigidBodyComponent)

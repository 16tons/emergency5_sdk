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
	*    Physics cylinder rigid body component class
	*/
	class QSF_API_EXPORT PhysicsCylinderRigidBodyComponent : public PhysicsRigidBodyComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::PhysicsCylinderRigidBodyComponent" unique component ID
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
		inline explicit PhysicsCylinderRigidBodyComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PhysicsCylinderRigidBodyComponent();

		/**
		*  @brief
		*    Return the cylinder radius
		*
		*  @return
		*    The cylinder radius
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getRadius() const;

		/**
		*  @brief
		*    Set the cylinder radius
		*
		*  @param[in] radius
		*    The cylinder radius to set
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setRadius(float radius);

		/**
		*  @brief
		*    Return the cylinder height
		*
		*  @return
		*    The cylinder height
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getHeight() const;

		/**
		*  @brief
		*    Set the cylinder height
		*
		*  @param[in] height
		*    The cylinder height to set
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
#include "qsf/physics/rigidbody/PhysicsCylinderRigidBodyComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PhysicsCylinderRigidBodyComponent)

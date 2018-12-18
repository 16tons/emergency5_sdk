// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"


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
	*    Physics component base class
	*/
	class QSF_API_EXPORT PhysicsComponent : public Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "qsf::PhysicsComponent" unique component ID
		static const uint32 COLLISION_FILTER_GROUP;		///< "CollisionFilterGroup" unique class property ID inside the class
		static const uint32 COLLISION_MASK;				///< "CollisionMask" unique class property ID inside the class
		static const uint32 COLLISION_TYPE_ID;			///< "CollisionTypeId" unique class property ID inside the class
		static const uint32 COLLISION_FRICTION;			///< "CollisionFriction" unique class property ID inside the class
		static const uint32 COLLISION_ROLLINGFRICTION;	///< "CollisionRollingFriction" unique class property ID inside the class

		static short mDefaultCollisionFilterGroup;
		static short mDefaultCollisionMask;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PhysicsComponent();

		// Exported bullet collision configuration options
		//@{
		inline short getCollisionFilterGroup() const;
		void setCollisionFilterGroup(short filter);

		inline short getCollisionMask() const;
		void setCollisionMask(short mask);

		inline uint32 getCollisionTypeId() const;
		void setCollisionTypeId(uint32 collisionTypeId);

		inline float getFriction() const;
		void setFriction(float friction);

		inline float getRollingFriction() const;
		void setRollingFriction(float rollingFriction);
		//@}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit PhysicsComponent(Prototype* prototype);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::PhysicsComponent methods       ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Method is called in case the collision filter group or collision mask property has been changed
		*/
		virtual void onCollisionFilterGroupOrCollisionMaskChanged() = 0;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Connected to the CAMP reflection system
		short	mCollisionFilterGroup;	///< The collision filter group with which this shape is registered in the world. Data type required by bullet.
		short	mCollisionMask;			///< The collision mask used to define with which other entities this item may collide. Data type required by bullet.
		uint32	mCollisionTypeId;		///< ID of the high level collision type, if not "qsf::getUninitialized<uint32>()" it will set the low level cryptic "CollisionFilterGroup" and "CollisionMask" properties using pre-defined values
		float	mFriction;				///< The friction which this shape will have
		float	mRollingFriction;		///< The rolling friction which this shape will have


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void applyCollisionType();


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
#include "qsf/physics/PhysicsComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PhysicsComponent)

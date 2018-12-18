// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/physics/collision/BulletCollisionComponent.h"

#include <BulletCollision/CollisionShapes/btSphereShape.h>

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
	*    Bullet collision component in the shape of an sphere
	*/
	class QSF_API_EXPORT BulletSphereCollisionComponent : public BulletCollisionComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;
		static const uint32 RADIUS_PROPERTY_ID;
		static const float DEFAULT_RADIUS;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline BulletSphereCollisionComponent(Prototype* prototype);

		/** The main property of a sphere is its radius.
		*/
		//@{
		inline float getRadius() const; // No reference return by design because we convert from the Bullet format
		void setRadius(float radius);
		//@}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::BulletCollisionComponent methods ]
	//[-------------------------------------------------------]
	protected:
		virtual glm::vec3 convertPosition(const btVector3& bulletPosition) const override;
		virtual btVector3 convertPosition(const glm::vec3& qsfPosition) const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		btSphereShape mSphereShape;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::BulletSphereCollisionComponent);


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/physics/collision/BulletSphereCollisionComponent-inl.h"

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/physics/collision/BulletCollisionComponent.h"

#include <BulletCollision/CollisionShapes/btCylinderShape.h>

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
	class QSF_API_EXPORT BulletCylinderCollisionComponent : public BulletCollisionComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;
		static const uint32 RADIUS_PROPERTY_ID;
		static const uint32 HEIGHT;
		static const float DEFAULT_RADIUS;
		static const float DEFAULT_HEIGHT;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline BulletCylinderCollisionComponent(Prototype* prototype);

		inline float getRadius() const;
		void setRadius(float radius);

		inline float getHeight() const;
		void setHeight(float height);


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
		float mRadius;	///< Radius
		float mHeight;	///< Height
		btCylinderShape mCylinderShape;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::BulletCylinderCollisionComponent);


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/physics/collision/BulletCylinderCollisionComponent-inl.h"

// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/physics/collision/BulletCollisionComponent.h"

#include <BulletCollision/CollisionShapes/btBoxShape.h>

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
	*    Bullet collision component in the shape of an oriented box
	*/
	class QSF_API_EXPORT BulletBoxCollisionComponent : public BulletCollisionComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;
		static const uint32 HALF_EXTENTS_PROPERTY_ID;

		static const glm::vec3 DEFAULT_HALF_EXTENTS;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit BulletBoxCollisionComponent(Prototype* prototype);

		/** The main property of a box is its extension in the three dimensions.
		* Bullet uses the term half extents to signal that this is calculated from the center to the border thus the whole box side length is twice the value for each axis.
		*/
		//@{
		inline glm::vec3 getHalfExtents() const; // no reference return by design because we convert from the bullet format
		void setHalfExtents(const glm::vec3& halfExtents);
		glm::vec3 getShapeExtents() const; // Gets the complete shape extents
		//@}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::BulletCollisionComponent methods ]
	//[-------------------------------------------------------]
	protected:
		virtual glm::vec3 convertPosition(const btVector3& bulletPosition) const override;
		virtual btVector3 convertPosition(const glm::vec3& qsfPosition) const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		virtual bool onStartup() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		btBoxShape	mBoxShape;
		glm::vec3	mOldScale;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::BulletBoxCollisionComponent);


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/physics/collision/BulletBoxCollisionComponent-inl.h"

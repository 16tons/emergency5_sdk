// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/physics/collision/BulletCollisionComponent.h"

#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>

#include <vector>
#include <memory>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class btTriangleMesh;


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
	*    Provides collision information in the form of a static triangle mesh
	*
	*  @remarks
	*    This is one of the most complex collision structures and should only be used if no other simpler shape fits.
	*    We always use meshes that are derived from some other source like a graphics mesh.
	*    In this case we transfer the mesh during the onStartup phase.
	*    Since we don't know which component is started first we use an optional delayed startUp here.
	*    We need the mesh transferred first (which should be done during the startup of the providing component) and then the collision component started  (which transfers the mesh to bullet).
	*    In case the order is the other way around we wait for the setter to be called to pass the mesh to the bullet world.
	*
	*    TODO(vs) We could do it completely different if we just listen to changes in the connected components here and update the collision mesh on every change.
	*    Then we could serialize the resulting mesh with this component.
	*    But I fear the triangulation effort might become noticeable if we do it on every minor change to a polygon.
	*    It would be ideal if we could trigger it when we leave the polygon edit mode for example.
	*/
	class QSF_API_EXPORT BulletTriangleMeshCollisionComponent : public BulletCollisionComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		BulletTriangleMeshCollisionComponent(Prototype* prototype);
		virtual ~BulletTriangleMeshCollisionComponent();

		// helper to query whether the mesh was set.
		// This is used to decide upon a delayed initialization
		bool wasMeshInitialized() const;

		// Set the mesh from the data passed.
		// The vertices need to be in local coordinates.
		void setMesh(const std::vector<btVector3>& vertices, const std::vector<uint16>& indices);

		// Set the mesh from the data passed. And applies local scaling of the shape
		// The vertices need to be in local coordinates.
		void setMesh(const std::vector<btVector3>& vertices, const std::vector<uint16>& indices, const glm::vec3& scaling);


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
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// The final bullet collision shape
		btTriangleMesh*						  mTriangleMesh;
		std::auto_ptr<btBvhTriangleMeshShape> mCollisionShape; // The dynamic allocation seems necessary because delaying the build process until we have the information leads to strange errors
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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::BulletTriangleMeshCollisionComponent);

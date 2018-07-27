// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/nodes/PathComponent.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"

#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class btVector3;
namespace Ogre
{
	namespace v1
	{
		class Mesh;
		class Entity;
	}
}
namespace qsf
{
	class CameraComponent;
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
	*    Procedural path mesh component class
	*
	*  @note
	*    - The procedural path mesh component takes care that the procedural OGRE mesh is generated and tells the mesh component about the procedural mesh
	*    - Technically quite similar to "qsf::StreetSectionComponent"
	*
	*  @todo
	*    - TODO(np) If the node points are to close, the street becomes corrupt. Need to implement exceptional cases
	*    - TODO(co) When saving the OGRE mesh, do also automatically generate mesh LODs by e.g. reducing the number of segments within lower LODs
	*/
	class QSF_API_EXPORT PathMeshComponent : public PathComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;				///< "qsf::PathMeshComponent" unique component ID
		static const uint32 BASE_MESH;					///< "BaseMesh" unique class property ID inside the class
		static const uint32 FLIP_X_AXIS;				///< "FlipXAxis" unique class property ID inside the class
		static const uint32 FLIP_Y_AXIS;				///< "FlipYAxis" unique class property ID inside the class
		static const uint32 BLEND_OUT_LENGTH_AT_END;	///< "BlendOutLengthAtEnd" unique class property ID inside the class
		static const uint32 BLEND_OUT_LENGTH_AT_START;	///< "BlendOutLengthAtStart" unique class property ID inside the class
		static const uint32 COLLISION_TOP_OFFSET;		///< "CollisionTopOffset" unique class property ID inside the class
		static const uint32 COLLISION_BOTTOM_OFFSET;	///< "CollisionBottomOffset" unique class property ID inside the class
		static const uint32 COLLISION_LEFT_OFFSET;		///< "CollisionLeftOffset" unique class property ID inside the class
		static const uint32 COLLISION_RIGHT_OFFSET;		///< "CollisionRightOffset" unique class property ID inside the class

		// TODO(co) For now, we have no procedural mesh LODs due to the memory consumption issues of EMERGENCY 5
		static const uint32 PATHMESH_NUM_LODS = 1;
		// Not constant by intent, usually controlled by a high level quality control
		static int PATHMESH_SEGMENTS_MIN[PATHMESH_NUM_LODS];	// The minimum segments when the path is straight. This value should have a minimum of 4 to prevent artefacts in curve creation.
		static int PATHMESH_SEGMENTS_MAX[PATHMESH_NUM_LODS];	// The maximum segments when the curve becomes spiky


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
		inline explicit PathMeshComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~PathMeshComponent();

		/**
		*  @brief
		*    Return the base mesh asset to use
		*
		*  @return
		*    The base mesh asset to use (e.g. "sample/mesh/default/chain"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const AssetProxy& getBaseMesh() const;

		/**
		*  @brief
		*    Set the base mesh asset to use
		*
		*  @param[in] baseMesh
		*    The base mesh asset to use (e.g. "sample/mesh/default/chain")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setBaseMesh(const AssetProxy& baseMesh);

		/**
		*  @brief
		*    Return whether or not the x axis should be flipped
		*
		*  @return
		*    "true" if the x axis should be flipped, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getFlipXAxis() const;

		/**
		*  @brief
		*    Set whether or not the x axis should be flipped
		*
		*  @param[in] flipXAxis
		*    "true" if the x axis should be flipped, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setFlipXAxis(bool flipXAxis);

		/**
		*  @brief
		*    Return whether or not the y axis should be flipped
		*
		*  @return
		*    "true" if the y axis should be flipped, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getFlipYAxis() const;

		/**
		*  @brief
		*    Set whether or not the y axis should be flipped
		*
		*  @param[in] flipYAxis
		*    "true" if the y axis should be flipped, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setFlipYAxis(bool flipYAxis);

		/**
		*  @brief
		*    Return the length of the blend out at the start of the path
		*
		*  @return
		*    Length of the blend out
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getBlendOutLengthAtStart() const;

		/**
		*  @brief
		*    Set how long the blend out is at the start of the path
		*
		*  @param[in] blendOutLength
		*    Length of the blend out
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setBlendOutLengthAtStart(float blendOutLengthAtStart);

		/**
		*  @brief
		*    Return the length of the blend out at the end of the path
		*
		*  @return
		*    Length of the blend out
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getBlendOutLengthAtEnd() const;

		/**
		*  @brief
		*    Set how long the blend out is at the end of the path
		*
		*  @param[in] blendOutLength
		*    Length of the blend out
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setBlendOutLengthAtEnd(float blendOutLengthAtEnd);

		/**
		*  @brief
		*    Return the left, right, bottom, top offset of the collision blocker polygon
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getCollisionLeftOffset() const;
		inline float getCollisionRightOffset() const;
		inline float getCollisionBottomOffset() const;
		inline float getCollisionTopOffset() const;

		/**
		*  @brief
		*    Set the left, right, bottom, top offset of the collision blocker polygon
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setCollisionLeftOffset(float collisionLeftOffset);
		void setCollisionRightOffset(float collisionRightOffset);
		void setCollisionBottomOffset(float collisionBottomOffset);
		void setCollisionTopOffset(float collisionTopOffset);

		/**
		*  @brief
		*    Mark the procedural path mesh to be dirty
		*
		*  @note
		*   - Please do only call this method if you have to and you know what you're doing
		*/
		void dirtyMesh();

		/**
		*  @brief
		*    Creates a list of nodes which describes the path mesh as an polygon
		*
		*  @param[out] nodes
		*    Contains the list of nodes which describes the path mesh as an polygon
		*  @param[out] polygonHeight
		*    Contains the height of the polygon
		*/
		void getNodesAndHeightForPolygon(std::vector<Node>& nodes, float& polygonHeight);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::NodesComponent methods            ]
	//[-------------------------------------------------------]
	public:
		virtual void setNode(std::size_t index, const Node& value) override;
		virtual void setNodeToPosition(uint32 nodeIndex, const glm::vec3& newPosition) override;
		virtual void addPosition(const glm::vec3& position) override;
		virtual void setNodes(const std::vector<Node>& nodes) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Update the procedural path mesh if required
		*/
		void updateMesh();

		/**
		*  @brief
		*    This method gets called before a compositor workspace gets rendered
		*
		*  @param[in] cameraComponent
		*    Used camera component
		*/
		void onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent);

		/**
		*  @brief
		*    This method is called after the map has been saved
		*/
		// TODO(co) Remove procedural geometry file caching all together? It is constantly causing issues up to crashes.
		// void onPostMapSave();

		/**
		*  @brief
		*    Build the base mesh
		*
		*  @param[in] refresh
		*    Build the base mesh also when it already exists
		*/
		void buildMesh(bool refresh);

		/**
		*  @brief
		*    Set the mesh of the mesh component
		*/
		void setMeshComponentMesh();

		/**
		*  @brief
		*    Internal helper method to create a fitting collision mesh for this path mesh
		*/
		void buildCollisionMeshIfNecessary();

		/**
		*  @brief
		*    Similar to "qsf::OgreMeshInformationHelper::internalGetMeshInformation()" but slimmed down and using Bullet vectors
		*/
		void internalGetMeshInformation(const Ogre::v1::Mesh& mesh, std::vector<btVector3>& vertices, std::vector<uint16>& indices);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		AssetProxy	mBaseMesh;
		bool		mFlipXAxis;
		bool		mFlipYAxis;
		float		mBlendOutLengthAtStart;
		float		mBlendOutLengthAtEnd;
		// Collision polygon
		float		mCollisionLeftOffset;
		float		mCollisionRightOffset;
		float		mCollisionBottomOffset;
		float		mCollisionTopOffset;
		// Internal only
		std::string					mGeneratedOgreMeshNames[3];	///< Name of the OGRE meshes that were created
		bool						mDirtyMesh;					///< Does the procedural path mesh require an update?
		boost::signals2::connection mPreCompositorWorkspaceUpdateConnection;
		// boost::signals2::connection mPostMapSaveConnection;	// TODO(co) Remove procedural geometry file caching all together? It is constantly causing issues up to crashes.


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
#include "qsf/component/nodes/PathMeshComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::PathMeshComponent)

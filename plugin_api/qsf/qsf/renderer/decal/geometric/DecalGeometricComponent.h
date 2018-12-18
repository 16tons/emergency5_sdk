// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/decal/DecalComponent.h"
#include "qsf/renderer/decal/geometric/DecalGeometricMeshGenerator.h"

#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	namespace v1
	{
		class Entity;
	}
}
namespace qsf
{
	class CameraComponent;
	class DecalGeometricComponent;
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
	*    Geometric decal component class
	*
	*  @note
	*    - This class creates a geometric meshes, that represents the decals
	*/
	class QSF_API_EXPORT DecalGeometricComponent : public DecalComponent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class DecalGeometricReceiverComponent;
		friend class DecalGeometricMeshGenerator;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::DecalGeometricComponent" unique component ID
		static const uint32 USE_AS_LOCAL;	///< "UseAsLocal" unique class property ID inside the class


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
		inline explicit DecalGeometricComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~DecalGeometricComponent();

		/**
		*  @brief
		*    Mark the procedural decal mesh to be dirty
		*
		*  @note
		*   - Please do only call this method if you have to and you know what you're doing
		*/
		void dirtyMesh();

		/**
		*  @brief
		*    Get whether the component is used locally only
		*
		*  @return
		*    Local use
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*    - Local decals will be attached to the same OGRE scene node the mesh is attached to, global decals will be attached to the root OGRE scene node
		*/
		inline bool getUseAsLocal() const;

		/**
		*  @brief
		*    Set whether the component is used locally only
		*
		*  @param[in] local
		*    Local use
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*
		*  @see
		*    - "qsf::DecalGeometricComponent::getUseAsLocal()"
		*/
		inline void setUseAsLocal(bool local);


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
		inline virtual bool implementsOnComponentPropertyChange() const override { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Update the procedural decal mesh if required
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
		*    Build the base mesh
		*
		*  @param[in] refresh
		*    Build the base mesh also when it already exists
		*/
		void buildMesh(bool refresh);

		/**
		*  @brief
		*    Destroy the OGRE mesh
		*/
		void destroyOgreMesh();

		/**
		*  @brief
		*    Set the OGRE mesh
		*/
		void setOgreMesh();

		/**
		*  @brief
		*    Add a decal to receiver
		*
		*  @param[in] entity
		*    Entity, that received the decal
		*/
		void addDecalToEntity(Entity& decalReceivingEntity);

		/**
		*  @brief
		*    Remove decal from all receiver
		*/
		void removeDecal();

		/**
		*  @brief
		*    Remove receiver entity
		*
		*  @param[in] entity
		*    Entity, that received the decal
		*/
		void removeReceiverFromEntity(Entity& decalReceivingEntity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DecalGeometricMeshGenerator			mDecalGeometricMeshGenerator;	///< Generator for procedural meshes
		std::string							mGeneratedOgreMeshName;			///< Name of the OGRE mesh that was created	// TODO(np) Might be moved to decal mesh generator manager class
		Ogre::v1::Entity*					mOgreEntity;					///< OGRE entity instance, can be a null pointer
		AssetProxy							mMesh;							///< The mesh asset to use (e.g. "sample/mesh/house/house01")
		bool								mDirtyMesh;						///< Does the procedural decal mesh require an update?
		boost::signals2::connection			mPreCompositorWorkspaceUpdateConnection;
		bool								mUseAsLocal;					///< Does the component interact for entity only
		boost::container::flat_set<uint64>	mReceiver;						///< Set of entities that received decals


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
#include "qsf/renderer/decal/geometric/DecalGeometricComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::DecalGeometricComponent)

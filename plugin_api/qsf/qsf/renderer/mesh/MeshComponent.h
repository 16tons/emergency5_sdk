// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/component/RendererComponent.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"
#include "qsf/debug/DebugDrawProxy.h"
#include "qsf/job/JobProxy.h"

#include <boost/container/flat_set.hpp>


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
	class SkeletonDebug;
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
	*    Mesh component class
	*/
	class QSF_API_EXPORT MeshComponent : public RendererComponent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class RendererSettingsGroup;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;			///< "qsf::MeshComponent" unique component ID
		static const uint32 MESH;					///< "Mesh" unique class property ID inside the class
		static const uint32 LOD_FACTOR;				///< "LodFactor" unique class property ID inside the class
		static const uint32 MAX_LOD_DETAIL_INDEX;	///< "MaxLodDetailIndex" unique class property ID inside the class
		static const uint32 MIN_LOD_DETAIL_INDEX;	///< "MinLodDetailIndex" unique class property ID inside the class
		static const uint32 ENTITY;					///< "Entity" unique class property ID inside the class (this is for internal use, there is no such property actually bound to CAMP)
		static const char MESH_PLACEHOLDER_ASSET[];


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Update the local axis aligned bounding box of the OGRE entity by using the current mesh bounds
		*
		*  @param[in] ogreEntity
		*    OGRE entity to update the local AABB from
		*
		*  @remarks
		*    The local AABB of OGRE entities is automatically set inside "Ogre::v1::Entity::_initialise()" by using the current mesh bounds. When
		*    changing those mesh bounds later on by using "Ogre::v1::Mesh::_setBounds()", e.g. entities using the mesh are not updated automatically.
		*    One could enforce an OGRE entity reinitialization in this situation, but this would be total overkill. Use this method instead.
		*/
		static void updateOgreEntityLocalAabbByOgreMesh(Ogre::v1::Entity& ogreEntity);


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
		explicit MeshComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MeshComponent();

		/**
		*  @brief
		*    Return the mesh asset to use
		*
		*  @return
		*    The mesh asset to use (e.g. "sample/mesh/house/house01"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const AssetProxy& getMesh() const;

		/**
		*  @brief
		*    Set the mesh asset to use
		*
		*  @param[in] mesh
		*    The mesh asset to use (e.g. "sample/mesh/house/house01")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setMesh(const AssetProxy& mesh);

		//[-------------------------------------------------------]
		//[ LOD                                                   ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the proportional factor to apply to the distance at which LOD is changed
		*
		*  @return
		*    The proportional factor to apply to the distance at which LOD is changed. Higher values increase the distance at which higher LODs are displayed (2.0 is twice the normal distance, 0.5 is half).
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getLodFactor() const;

		/**
		*  @brief
		*    Set the proportional factor to apply to the distance at which LOD is changed
		*
		*  @param[in] lodFactor
		*    The proportional factor to apply to the distance at which LOD is changed. Higher values increase the distance at which higher LODs are displayed (2.0 is twice the normal distance, 0.5 is half).
		*    Must be 0.001>= (automatically clamped in case of invalid values).
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setLodFactor(float lodFactor);

		/**
		*  @brief
		*    Return the index of the maximum LOD this entity is allowed to use
		*
		*  @return
		*    The index of the maximum LOD this entity is allowed to use (lower indexes are higher detail: index 0 is the original full detail model)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline unsigned short getMaxLodDetailIndex() const;

		/**
		*  @brief
		*    Set the index of the maximum LOD this entity is allowed to use
		*
		*  @param[in] maxLodDetailIndex
		*    The index of the maximum LOD this entity is allowed to use (lower indexes are higher detail: index 0 is the original full detail model)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setMaxLodDetailIndex(unsigned short maxLodDetailIndex);

		/**
		*  @brief
		*    Return the index of the minimum LOD this entity is allowed to use
		*
		*  @return
		*    The index of the minimum LOD this entity is allowed to use (higher indexes are lower detail). Use something like 99 if you want unlimited LODs (the actual LOD will be limited by the number in the mesh)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline unsigned short getMinLodDetailIndex() const;

		/**
		*  @brief
		*    Set the index of the minimum LOD this entity is allowed to use
		*
		*  @param[in] minLodDetailIndex
		*    The index of the minimum LOD this entity is allowed to use (higher indexes are lower detail). Use something like 99 if you want unlimited LODs (the actual LOD will be limited by the number in the mesh)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setMinLodDetailIndex(unsigned short minLodDetailIndex);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set a custom parameter for the OGRE renderable, which may be used to drive calculations for this specific OGRE renderable, like GPU program parameters
		*
		*  @param[in] index
		*    The index with which to associate the value. Note that this
		*    does not have to start at 0, and can include gaps. It also has no direct
		*    correlation with a GPU program parameter index - the mapping between the
		*    two is performed by the ACT_CUSTOM entry, if that is used.
		*  @param[in] value
		*    The value to associate.
		*
		*  @remarks
		*    Calling this method simply associates a numeric index with a 4-dimensional
		*    value for this specific Renderable. This is most useful if the material
		*    which this Renderable uses a vertex or fragment program, and has an
		*    ACT_CUSTOM parameter entry. This parameter entry can refer to the
		*    index you specify as part of this call, thereby mapping a custom
		*    parameter for this renderable to a program parameter.
		*
		*  @note
		*    - Don't use this if you don't have to
		*    - Calls "Ogre::Renderable::setCustomParameter()" for all OGRE sub-entities and all OGRE manual LOD levels (and there all OGRE sub-entities)
		*/
		void setOgreCustomParameter(uint32 index, const glm::vec4& value) const;

		/**
		*  @brief
		*    Remove all OGRE custom parameters
		*/
		void removeAllOgreCustomParameters() const;

		/**
		*  @brief
		*    Return the currently set visibility flags for the mesh
		*
		*  @return
		*    The visibility flags bit mask
		*/
		inline uint32 getVisibilityFlags() const;

		/**
		*  @brief
		*    Set visibility flags for the mesh
		*
		*  @param[in] flags
		*    The visibility flags bit mask to set; when rendering, it is checked against the OGRE compositor node's visibility flags via binary AND
		*/
		void setVisibilityFlags(uint32 flags);

		/**
		*  @brief
		*    Gets the custom bounding box for the internal OGRE entity
		*
		*  @param[out] localMinCorner
		*    Minimum corner position in object space coordinates
		*  @param[out] localMaxCorner
		*    Maximum corner position in object space coordinates
		*/
		inline void getOgreEntityBoundingBox(glm::vec3& localMinCorner, glm::vec3& localMaxCorner) const;

		/**
		*  @brief
		*    Set a custom bounding box for the internal OGRE entity
		*
		*  @param[in] localMinCorner
		*    Minimum corner position in object space coordinates
		*  @param[in] localMaxCorner
		*    Maximum corner position in object space coordinates
		*/
		void setOgreEntityBoundingBox(const glm::vec3& localMinCorner, const glm::vec3& localMaxCorner);

		/**
		*  @brief
		*    Get the custom bounding box for the internal OGRE entity as local extends
		*
		*  @return
		*    The extends factor on each axis the original bounding box is extended
		*/
		inline const glm::vec3& getOgreEntityBoundingBox() const;

		/**
		*  @brief
		*    Set a custom bounding box for the internal OGRE entity
		*
		*  @param[in] localExtends
		*    The extends factor on each axis the original bounding box is extended
		*/
		void setOgreEntityBoundingBox(const glm::vec3& localExtends);

		/**
		*  @brief
		*    Returns if a custom bounding box is set
		*
		*  @return
		*    "true" when set otherwise "false"
		*/
		inline bool isCustomBoundingBoxSet() const;

		/**
		*  @brief
		*    Resets the using of a custom bounding box
		*/
		void resetCustomBoundingBox();

		/**
		*  @brief
		*    Update the local axis aligned bounding box of the OGRE entity by using the current mesh bounds
		*
		*  @note
		*    - Ease-of-use method, see static "qsf::MeshComponent::updateOgreEntityLocalAabbByOgreMesh()" method for details
		*/
		void updateOgreEntityLocalAabbByOgreMesh();

		/**
		*  @brief
		*    Return if skeleton reusage (can also be named as skeleton sharing) is active
		*
		*  @remarks
		*    When this flag is set, it does not mean that this mesh instance reuses an skeleton
		*    It only reuses the skeleton of the parent when
		*     - The entity of this mesh instance has an parent entity
		*     - And the parent entity has an mesh component
		*     - And the mesh of the parent mesh component has an skeleton
		*/
		inline bool getReuseSkeletonFromParent() const;

		/**
		*  @brief
		*    Enables / Disables skeleton reusage (can also be named as skeleton sharing) from the parent mesh
		*
		*  @see
		*    - "qsf::MeshComponent::getReuseSkeletonFromParent()"
		*/
		// TODO(sw) Currently changing this value has only an effect, when a new mesh is set
		// TODO(sw) No switch back to own skeleton when the entity gets unlinked from its parent
		//          Or handling when the parent changes in general
		void setReuseSkeletonFromParent(bool reuseSkeletonFormParent);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	public:
		inline virtual Ogre::SceneNode* getOgreSceneNode() const override;
		inline virtual Ogre::v1::Entity* getOgreEntity() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MeshComponent methods          ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called when the OGRE entity changes
		*
		*  @remarks
		*    If you override this method, be sure to call the base implementation or
		*    the entity property change will not be signaled.
		*/
		virtual void onOgreEntityChanged();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void setOgreSceneNodeVisibility(bool visible) override;
		inline virtual bool isGridManaged() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetDebug(bool debug) override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		inline virtual bool implementsOnComponentPropertyChange() const override { return true; }
		virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Destroy the OGRE mesh
		*/
		void destroyOgreMesh(bool propagateEntityPropertyChange = true);

		/**
		*  @brief
		*    Check if meshAsset can be loaded, list missing dependencies in parameter if not
		*
		*  @param[in] meshAsset
		*    Mesh asset for dependency check
		*  @param[out] missingDependencies
		*    Receives a list of missing dependency global asset IDs
		*
		*  @returns
		*    "true" if mesh asset can be supplied to other, "false" if not
		*/
		bool canUseOgreMesh(const AssetProxy& meshAsset, boost::container::flat_set<GlobalAssetId>& missingDependencies) const;

		/**
		*  @brief
		*    Set the OGRE mesh to use
		*/
		void setOgreMesh();

		/**
		*  @brief
		*    Set the LOD settings to use, internal part
		*/
		void setMeshLodBiasInternal();

		/**
		*  @brief
		*    Updates the mesh according to the current quality
		*/
		void updateMesh();

		void updateCustomBoundingBox();

		//[-------------------------------------------------------]
		//[ Skeleton sharing                                      ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Setups skeleton sharing with the skeleton of the parent entity
		*
		*  @returns
		*    An pointer to the parent ogre entity which the skeleton sharing was setup or an null pointer when no sharing was setup
		*/
		Ogre::v1::Entity* updateSkeletonSharing();

		/**
		*  @brief
		*    Stops sharing the skeleton with the parent. It restores any skeleton sharing with child entities
		*/
		void stopSkeletonSharingWithParent();

		/**
		*  @brief
		*    Setups skeleton sharing with the given child OGRE entity, this instance is the master of the sharing
		*
		*  @param[in] childOgreEntity
		*    The OGRE entity which should share the skeleton with this instance
		*/
		void setupSkeletonSharingUsAsMaster(Ogre::v1::Entity* childOgreEntity);

		/**
		*  @brief
		*    Returns a list of OGRE entities this instance can share the skeleton with (this instance as master)
		*
		*  @param[in] childOgreEntities
		*    Contains all child OGRE entities which can share the skeleton with this instance and have skeleton sharing enabled
		*
		*  @note
		*    - When the child OGRE entity is already sharing its skeleton this sharing is stopped (because sharing is only possible with one other OGRE entity)
		*/
		void getChildOgreEntitiesToShareSkeletonWith(std::vector<Ogre::v1::Entity*>& childOgreEntities);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		AssetProxy			mMesh;					///< The mesh asset to use (e.g. "sample/mesh/house/house01"), no default mesh set by intent; "qsf::StreetSectionComponent" and similar use-cases will get inefficient setting a default mesh first and then shorty after this replacing it by the real mesh
		float				mLodFactor;				///< The proportional factor to apply to the distance at which LOD is changed. Higher values increase the distance at which higher LODs are displayed (2.0 is twice the normal distance, 0.5 is half).
		unsigned short		mMaxLodDetailIndex;		///< The index of the maximum LOD this entity is allowed to use (lower indexes are higher detail: index 0 is the original full detail model)
		unsigned short		mMinLodDetailIndex;		///< The index of the minimum LOD this entity is allowed to use (higher indexes are lower detail). Use something like 99 if you want unlimited LODs (the actual LOD will be limited by the number in the mesh)
		// Internal only
		Ogre::v1::Entity*	mOgreEntity;			///< OGRE entity instance, can be a null pointer
		Ogre::SceneNode*	mOgreSceneNode;			///< OGRE scene node instance, can be a null pointer
		int					mResourceCallbackId;
		uint32				mVisibilityFlags;		///< Visibility flags bit mask; is checked against the OGRE compositor node pass' visibility bit mask using binary AND

		// TODO(sw) Needed do the fact that the ogre entity can change...
		bool				mIsCustomBoundingBoxSet;		///< Indicates if the custom bounding box should be used
		glm::vec3			mCustomBoundingBoxMinCorner;	///< The lower corner of the custom bounding box
		glm::vec3			mCustomBoundingBoxMaxCorner;	///< The upper corner of the custom bounding box
		glm::vec3			mCustomBoundingBoxLocalExtends;	///< The last set local extends value

		// Debug
		SkeletonDebug*		mSkeletonDebug;				///< Skeleton debug, can be a null pointer, destroy the instance if you no longer need it
		DebugDrawProxy		mMeshVertexNormalsDebugDrawProxy;

		//[-------------------------------------------------------]
		//[ Skeleton sharing                                      ]
		//[-------------------------------------------------------]
		bool				mReuseSkeletonFromParent;				///< Indicates if this mesh instance should use the same skeleton as the mesh from the direct parent (skeleton sharing)
		bool				mSetupReuseSkeletonFromParentSuccess;	///< Indicates if setting up skeleton resuage from parent was successful


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
#include "qsf/renderer/mesh/MeshComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::MeshComponent)

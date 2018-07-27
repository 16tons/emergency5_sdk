// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/decal/DecalComponent.h"
#include "qsf/renderer/decal/trace/TraceDecalMeshGenerator.h"
#include "qsf/job/JobProxy.h"

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
	class TraceDecalComponent;
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
	*    Trace decal component class
	*
	*  @remarks
	*    Best used for stuff like dynamic footsteps or wheel traces
	*/
	class QSF_API_EXPORT TraceDecalComponent : public DecalComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::TraceDecalComponent" unique component ID


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
		inline explicit TraceDecalComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~TraceDecalComponent();

		/**
		*  @brief
		*    Mark the decal mesh to be dirty
		*
		*  @note
		*   - Please do only call this method if you have to and you know what you're doing
		*/
		void dirtyMesh();

		/**
		*  @brief
		*    After this is called, the decal will delete the owning entity at any time when the nodes are empty
		*
		*  @remarks
		*    Call this when you no longer actively use the trace decal and you want to detach it from
		*    the generating source and let it fade-out gracefully.
		*/
		void detachAndDeleteWhenNodesEmpty();


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
		*    Update the decal mesh if required
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

		void updateNodeDecay(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		TraceDecalMeshGenerator		mMeshGenerator;
		std::string					mGeneratedOgreMeshName;			///< Name of the OGRE mesh that was created
		Ogre::v1::Entity*			mOgreEntity;					///< OGRE entity instance, can be a null pointer
		AssetProxy					mMesh;							///< The mesh asset to use
		bool						mDirtyMesh;						///< Does the mesh require an update?
		boost::signals2::connection mPreCompositorWorkspaceUpdateConnection;
		JobProxy					mNodeDecayJob;
		bool						mDestroyWhenOutOfNodes;


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
#include "qsf/renderer/decal/trace/TraceDecalComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::TraceDecalComponent)

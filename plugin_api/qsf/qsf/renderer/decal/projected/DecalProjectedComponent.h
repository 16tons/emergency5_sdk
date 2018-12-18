// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/decal/DecalComponent.h"
#include "qsf/renderer/decal/projected/DecalProjectedMeshGenerator.h"

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
	class DecalProjectedComponent;
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
	*    Projected decal component class
	*/
	class QSF_API_EXPORT DecalProjectedComponent : public DecalComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "qsf::DecalProjectedComponent" unique component ID


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
		inline explicit DecalProjectedComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~DecalProjectedComponent();

		/**
		*  @brief
		*    Mark the procedural decal mesh to be dirty
		*
		*  @note
		*   - Please do only call this method if you have to and you know what you're doing
		*/
		void dirtyMesh();


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


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DecalProjectedMeshGenerator	mDecalProjectedMeshGenerator;
		std::string					mGeneratedOgreMeshName;			///< Name of the OGRE mesh that was created	// TODO(np) Might be moved to decal mesh generator manager class
		Ogre::v1::Entity*			mOgreEntity;					///< OGRE entity instance, can be a null pointer
		AssetProxy					mMesh;							///< The mesh asset to use (e.g. "sample/mesh/house/house01")
		bool						mDirtyMesh;						///< Does the procedural decal mesh require an update?
		boost::signals2::connection mPreCompositorWorkspaceUpdateConnection;


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
#include "qsf/renderer/decal/projected/DecalProjectedComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::DecalProjectedComponent)

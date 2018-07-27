// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/manager/Manager.h>

#include <unordered_set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Camera;
}
namespace em5
{
	class BuildingComponent;
}
namespace qsf
{
	class Map;
	class MeshComponent;
	class CameraComponent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 building clipping manager
	*/
	class EM5_API_EXPORT BuildingClippingManager : public qsf::Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class BuildingComponent;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::unordered_set<uint64> EntityIdSet;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		BuildingClippingManager();
		virtual ~BuildingClippingManager();
		inline bool isBuildingClippingEnabled() const;
		void setBuildingClippingEnabled(bool buildingClippingEnabled);
		inline void setVisibilityCheckEnabled(bool visibilityCheckEnabled);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onBuildingComponentRendered(const qsf::MeshComponent& meshComponent, BuildingComponent& buildingComponent, const Ogre::Camera& ogreCamera);
		void endEntityClipping(uint64 entityId) const;

		/**
		*  @brief
		*    This method gets called before a compositor workspace gets rendered
		*
		*  @param[in] cameraComponent
		*    Used camera component
		*/
		void onPreCompositorWorkspaceUpdate(const qsf::CameraComponent& cameraComponent);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Map&	mMap;
		bool		mBuildingClippingEnabled;
		EntityIdSet	mBuildingCameraIntersectionRootEntityIdSet;		///< All currently clipped root entities
		EntityIdSet	mBuildingCameraIntersectionEntityIdSet;			///< All individual currently clipped entities
		EntityIdSet	mBuildingCameraIntersectionWorkingEntityIdSet;	///< For internal update to avoid constant reallocations
		bool		mVisibilityCheckEnabled;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/building/BuildingClippingManager-inl.h"

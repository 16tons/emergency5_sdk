// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/building/BuildingClippingManager.h"
#include "em5/component/building/BuildingComponent.h"

#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/renderer/mesh/MeshComponent.h>
#include <qsf/renderer/light/LightComponent.h>
#include <qsf/QsfHelper.h>

#include <OGRE/OgreEntity.h>
#include <OGRE/OgreCamera.h>
#include <OGRE/OgreSceneManager.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BuildingClippingManager::BuildingClippingManager() :
		mMap(QSF_MAINMAP),
		mBuildingClippingEnabled(false),
		mVisibilityCheckEnabled(true)
	{
		mBuildingCameraIntersectionEntityIdSet.reserve(128);
		setBuildingClippingEnabled(true);
	}

	BuildingClippingManager::~BuildingClippingManager()
	{
		setBuildingClippingEnabled(false);
	}

	void BuildingClippingManager::setBuildingClippingEnabled(bool buildingClippingEnabled)
	{
		if (mBuildingClippingEnabled != buildingClippingEnabled)
		{
			// Was building clipping previously enabled?
			if (mBuildingClippingEnabled)
			{
				// Disconnect our Boost slot from the Boost signal of the QSF map
				mMap.PreCompositorWorkspaceUpdate.disconnect(boost::bind(&BuildingClippingManager::onPreCompositorWorkspaceUpdate, this, _1));
			}

			// Set new state
			mBuildingClippingEnabled = buildingClippingEnabled;

			// Is building clipping now enabled?
			if (mBuildingClippingEnabled)
			{
				// Connect our Boost slot to the Boost signal of the QSF map
				mMap.PreCompositorWorkspaceUpdate.connect(boost::bind(&BuildingClippingManager::onPreCompositorWorkspaceUpdate, this, _1));
			}
			else
			{
				// Restore materials
				for (auto entityId : mBuildingCameraIntersectionEntityIdSet)
				{
					endEntityClipping(entityId);
				}

				// Clear our entity lists
				mBuildingCameraIntersectionRootEntityIdSet.clear();
				mBuildingCameraIntersectionEntityIdSet.clear();
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BuildingClippingManager::onBuildingComponentRendered(const qsf::MeshComponent& meshComponent, BuildingComponent& buildingComponent, const Ogre::Camera& ogreCamera)
	{
		Ogre::SceneManager* ogreSceneManager = meshComponent.getEntity().getMap().getOgreSceneManager();
		QSF_ASSERT(nullptr != ogreSceneManager, "Invalid OGRE scene manager instance", QSF_REACT_NONE);

		// Perform the intersection check between camera and buildings?
		if (mBuildingClippingEnabled && mVisibilityCheckEnabled)
		{
			bool intersecting = false;
			if (buildingComponent.getClippingEnabled())
			{
				const Ogre::v1::Entity* buildingOgreEntity = meshComponent.getOgreEntity();
				if (nullptr != buildingOgreEntity)
				{
					const Ogre::Aabb ogreWorldAabb = buildingOgreEntity->getWorldAabb();
					const float radius = ogreCamera.getNearClipDistance() + 0.1f;

					// Evaluate at the camera position
					const Ogre::Vector3& position1 = ogreCamera.getDerivedPosition();
					intersecting = ogreWorldAabb.intersects(Ogre::Aabb(position1, Ogre::Vector3(radius)));

					if (!intersecting)
					{
						// Evaluate again a bit in front of the camera
						const Ogre::Vector3 position2 = position1 + ogreCamera.getDerivedDirection() * 10.0f;
						intersecting = ogreWorldAabb.intersects(Ogre::Aabb(position2, Ogre::Vector3(radius)));
					}
				}
			}

			// Handle the visibility of the building group
			if (intersecting)
			{
				qsf::Entity& rootEntity = static_cast<qsf::Entity&>(qsf::LinkComponent::getTopmostAncestorPrototype(buildingComponent.getEntity(), qsf::LinkComponent::SELECT_PARENT));
				mBuildingCameraIntersectionRootEntityIdSet.insert(rootEntity.getId());
			}
		}
	}

	void BuildingClippingManager::endEntityClipping(uint64 entityId) const
	{
		const qsf::Entity* entity = mMap.getEntityById(entityId);
		if (nullptr != entity)
		{
			BuildingComponent* buildingComponent = entity->getComponent<BuildingComponent>();
			if (nullptr != buildingComponent)
			{
				// It's a building, disable clipping
				buildingComponent->endClipping();
			}
			else
			{
				// It's not a building, unhide it
				entity->setHidden(false);
			}
		}
	}

	void BuildingClippingManager::onPreCompositorWorkspaceUpdate(const qsf::CameraComponent&)
	{
		// Handle all entities clipped in the previous frame
		for (uint64 rootEntityId : mBuildingCameraIntersectionRootEntityIdSet)
		{
			qsf::Entity* rootEntity = mMap.getEntityById(rootEntityId);
			if (nullptr != rootEntity)
			{
				qsf::EntityVectorScratchBuffer linkedEntities;
				qsf::LinkComponent::getConnectedLinkGroup(*rootEntity, qsf::LinkComponent::PARENT_DEPENDING_EXISTENCE, true, linkedEntities);
				for (qsf::Entity* entity : *linkedEntities)
				{
					const uint64 entityId = entity->getId();
					mBuildingCameraIntersectionWorkingEntityIdSet.insert(entityId);

					BuildingComponent* buildingComponent = entity->getComponent<BuildingComponent>();
					if (nullptr != buildingComponent)
					{
						// It's a building, enable clipping
						if (mBuildingCameraIntersectionEntityIdSet.find(entityId) == mBuildingCameraIntersectionEntityIdSet.cend())
						{
							mBuildingCameraIntersectionEntityIdSet.insert(entityId);
							buildingComponent->startClipping();
						}
						else
						{
							buildingComponent->continueClipping();
						}
					}
					else if (nullptr == entity->getComponent<qsf::LightComponent>())
					{
						// It's not a building or light, hide it
						if (mBuildingCameraIntersectionEntityIdSet.find(entityId) == mBuildingCameraIntersectionEntityIdSet.cend())
						{
							mBuildingCameraIntersectionEntityIdSet.insert(entityId);
							entity->setHidden(true);
						}
					}
				}
			}
		}
		mBuildingCameraIntersectionRootEntityIdSet.clear();

		// Handle all entities not clipped in the previous frame but the frame before the previous frame
		for (uint64 entityId : mBuildingCameraIntersectionEntityIdSet)
		{
			if (mBuildingCameraIntersectionWorkingEntityIdSet.find(entityId) == mBuildingCameraIntersectionWorkingEntityIdSet.cend())
			{
				endEntityClipping(entityId);
			}
		}

		// Swap entity lists
		mBuildingCameraIntersectionEntityIdSet = mBuildingCameraIntersectionWorkingEntityIdSet;
		mBuildingCameraIntersectionWorkingEntityIdSet.clear();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

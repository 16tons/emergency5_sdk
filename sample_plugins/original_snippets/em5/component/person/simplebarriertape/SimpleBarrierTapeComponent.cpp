// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/person/simplebarriertape/SimpleBarrierTapeComponent.h"
#include "em5/game/groundmap/GroundMaps.h"
#include "em5/map/MapHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Jobs.h"

#include <qsf/QsfHelper.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/renderer/RendererSystem.h>
#include <qsf/renderer/map/GridSceneManager.h>
#include <qsf/renderer/helper/OgreAnimationHelper.h>
#include <qsf/renderer/material/MaterialSystem.h>
#include <qsf/renderer/material/material/MaterialProperty.h>
#include <qsf/renderer/material/material/MaterialVariationManager.h>
#include <qsf/debug/request/CircleDebugDrawRequest.h>
#include <qsf/debug/request/SegmentDebugDrawRequest.h>
#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/math/Convert.h>

#include <OGRE/OgreRoot.h>	// TODO(fw): Break down...
#include <OGRE/OgreEntity.h>
#include <OGRE/OgreMeshManager.h>

#include <boost/bind.hpp>
#include <boost/format.hpp>

// TODO(fw): I had to put this towards the end, otherwise the "TRANSPARENT" define (windows header?) conflicts with "qsf::Color4"
#include "em5/component/person/simplebarriertape/SimpleBarrierTapeMeshGenerator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Constants                                             ]
	//[-------------------------------------------------------]
	const float BARRIERTAPE_MAXIMUM_SECTION_LENGTH = 1000.0f;


	//[-------------------------------------------------------]
	//[ Local helper methods                                  ]
	//[-------------------------------------------------------]
	namespace detail
	{
		inline bool updateTransform(qsf::Transform& transform, qsf::Entity& targetEntity, const std::string& targetBoneName)
		{
			glm::vec3 position;
			glm::quat rotation;

			if (targetBoneName.empty())
			{
				qsf::TransformComponent& transformComponent = targetEntity.getComponentSafe<qsf::TransformComponent>();
				position = transformComponent.getPosition();
				rotation = transformComponent.getRotation() * glm::quat(0.0f, 0.0f, 1.0f, 0.0f);	// Rotate by 180 degrees
			}
			else
			{
				const bool result = qsf::OgreAnimationHelper::getBoneWorldTransformation(targetEntity, targetBoneName, position, rotation);
				QSF_CHECK(result, "Could not get transformation from bone '" << targetBoneName << "' of entity " << targetEntity.getId(), return false);

				if (targetBoneName == "tape_bone")
				{
					// tape bones are oriented so that local z-axis points towards the hose direction
					rotation = qsf::Math::getLookAtQuaternion(rotation * qsf::Math::GLM_VEC3_UNIT_Z, qsf::CoordinateSystem::getUp());
				}
				else
				{
					// Other bones are oriented so that local y-axis points towards the hose direction
					rotation = qsf::Math::getLookAtQuaternion(rotation * qsf::Math::GLM_VEC3_UNIT_Y, qsf::CoordinateSystem::getUp());
				}
			}

			transform.setPositionAndRotation(position, rotation);

			return true;
		}

		inline bool dropToGround(qsf::Map& map, glm::vec3& position)
		{
			// Try to find the ground at the given position
			float height = position.y;

			qsf::GroundMapQuery groundMapQuery(map, GroundMaps::FILTER_NO_LIQUIDS);
			if (!groundMapQuery.getHeightAtXZPosition(position.x, position.z, height))
			{
				// Ground was not found, so no change at all
				return false;
			}

			// Change the position y coordinate
			position.y = height;
			return true;
		}
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SimpleBarrierTapeComponent::COMPONENT_ID = qsf::StringHash("em5::SimpleBarrierTapeComponent");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	SimpleBarrierTapeComponent& SimpleBarrierTapeComponent::createBarrierTapeBetweenEntities(qsf::Entity& entity1, const std::string& entity1BoneName, qsf::Entity& entity2, const std::string& entity2BoneName)
	{
		qsf::Entity* barrierTapeEntity = entity1.getMap().createEntity();
		QSF_CHECK(nullptr != barrierTapeEntity, "Failed to create warning tape entity", QSF_REACT_THROW);

		// Create components we will need
		barrierTapeEntity->getOrCreateComponentSafe<qsf::TransformComponent>();

		SimpleBarrierTapeComponent& barriertapeComponent = barrierTapeEntity->getOrCreateComponentSafe<SimpleBarrierTapeComponent>();
		barriertapeComponent.initializeWithEntities(entity1, entity1BoneName, entity2, entity2BoneName);

		return barriertapeComponent;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SimpleBarrierTapeComponent::SimpleBarrierTapeComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mTransparency(1.0f)
	{
		// Nothing to do here
	}

	SimpleBarrierTapeComponent::~SimpleBarrierTapeComponent()
	{
		// Nothing to do here
	}

	void SimpleBarrierTapeComponent::initializeWithEntities(qsf::Entity& entity1, const std::string& entity1BoneName, qsf::Entity& entity2, const std::string& entity2BoneName)
	{
		mParentEntity1 = static_cast<qsf::Entity&>(qsf::LinkComponent::getTopmostAncestorPrototype(entity1, 0xff));
		mEntity1 = entity1;
		mBoneName1 = entity1BoneName;

		mParentEntity2 = static_cast<qsf::Entity&>(qsf::LinkComponent::getTopmostAncestorPrototype(entity2, 0xff));
		mEntity2 = entity2;
		mBoneName2 = entity2BoneName;

		// Create first section
		addSection();

		// TODO(tl): Check if i should use the same Animation step as tape ?
		mAnimationJobProxy.registerAt(Jobs::ANIMATION_FIREHOSE, boost::bind(&SimpleBarrierTapeComponent::updateAnimation, this, _1));
	}

	void SimpleBarrierTapeComponent::getWaypoints(std::vector<glm::vec3>& outWaypoints)
	{
		if (mSections.size() >= 2)
		{
			for (size_t index = 0; index < mSections.size()-1; ++index)
			{
				outWaypoints.push_back(mSections[index].mEndTransform.getPosition());
			}
		}
	}

	void SimpleBarrierTapeComponent::setTransparency(float transparency)
	{
		if (mTransparency != transparency)
		{
			mTransparency = transparency;

			// TODO(tl): Material Variations are created for every ogre Entity, not very efficient
			for (Section& section : mSections)
			{
				setTransparency(section);
			}
		}
	}

	void SimpleBarrierTapeComponent::resetTransparency()
	{
		for (Section& section : mSections)
		{
			resetTransparency(section);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void SimpleBarrierTapeComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// No implementation yet...
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	bool SimpleBarrierTapeComponent::onStartup()
	{
		return true;
	}

	void SimpleBarrierTapeComponent::onShutdown()
	{
		resetTransparency();
		mAnimationJobProxy.unregister();
	}

	void SimpleBarrierTapeComponent::onSetSimulating(bool simulating)
	{
		if (!simulating)
		{
			// Remove remaining sections
			while (!mSections.empty())
			{
				removeSection();
			}
		}
	}

	void SimpleBarrierTapeComponent::onSetActive(bool active)
	{
		if (isRunning())
		{
			if (active)
			{
				// Create first section
				addSection();

				// TODO(tl): Check if i should use the same Animation step as tape ?
				mAnimationJobProxy.registerAt(Jobs::ANIMATION_FIREHOSE, boost::bind(&SimpleBarrierTapeComponent::updateAnimation, this, _1));
			}
			else
			{
				while (!mSections.empty())
				{
					size_t oldSize = mSections.size();
					removeSection();
					if (oldSize <= mSections.size())	//TODO(tl): Failsafe, just as an ensurence
						mSections.clear();
				}

				resetTransparency();
				mAnimationJobProxy.unregister();
			}
		}
	}

	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void SimpleBarrierTapeComponent::updateAnimation(const qsf::JobArguments& jobArguments)
	{
		QSF_ASSERT(!mSections.empty(), "tape has no sections at all", QSF_REACT_THROW);

		// One of the connected entities got invalid?
		if (!mParentEntity1.valid() || !mParentEntity2.valid() || !mEntity1.valid() || !mEntity2.valid())
		{
			// Delete this whole entity
			MapHelper(getEntity().getMap()).destroyEntity(getEntity());
			return;
		}

		// TODO(fw): Move all this stuff into a SIMULATION_GENERAL update!
		{
			const Section& lastSection = mSections.back();
			const glm::vec3& lastSectionStart = lastSection.mStartTransform.getPosition();
			const glm::vec3& lastSectionEnd = lastSection.mEndTransform.getPosition();
			const float lastSectionSquaredLength = glm::distance2(lastSectionStart, lastSectionEnd);

			// Is another section needed?
			if (lastSectionSquaredLength > (BARRIERTAPE_MAXIMUM_SECTION_LENGTH * 1.1f) * (BARRIERTAPE_MAXIMUM_SECTION_LENGTH * 1.1f))
			{
				addSection();
			}
			else if (mSections.size() >= 2)
			{
				// Check if least two sections can be merged again
				// TODO(fw): Allow merging of more sections at once?
				const Section& section = mSections[mSections.size()-2];
				if (glm::distance2(section.mStartTransform.getPosition(), lastSectionEnd) < (BARRIERTAPE_MAXIMUM_SECTION_LENGTH * 0.9f) * (BARRIERTAPE_MAXIMUM_SECTION_LENGTH * 0.9f))
				{
					removeSection();
				}
			}
		}

		// Update end position of last section, i.e. at the warning tape
		if (!mSections.empty())
		{
			detail::updateTransform(mSections.begin()->mStartTransform, *mEntity1, mBoneName1);
			detail::updateTransform(mSections.back().mEndTransform, *mEntity2, mBoneName2);
		}

		// Update mesh
		updateSectionMesh(static_cast<uint32>(mSections.size()-1));

#if 0
		// TEST(fw): Debug output
		{
			// Draw the hose
			mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);

			for (size_t index = 0; index < mSections.size(); ++index)
			{
				const Section& section = mSections[index];

				try
				{
					const qsf::Segment drawSegment = qsf::Segment::fromTwoPoints(section.mStartTransform.getPosition(), section.mEndTransform.getPosition());
					mDebugDrawProxy.addRequest(qsf::SegmentDebugDrawRequest(drawSegment, qsf::Color4::WHITE));
				}
				catch (...)
				{
				}

				mDebugDrawProxy.addRequest(qsf::CircleDebugDrawRequest(section.mStartTransform.getPosition(), section.mStartTransform.getRotation() * qsf::CoordinateSystem::getUp(), 0.4f, qsf::Color4::WHITE, true));
				mDebugDrawProxy.addRequest(qsf::CircleDebugDrawRequest(section.mEndTransform.getPosition(),   section.mEndTransform.getRotation() * qsf::CoordinateSystem::getUp(),   0.3f, qsf::Color4::WHITE, true));
			}
		}
#endif
	}

	void SimpleBarrierTapeComponent::addSection()
	{
		// Add a new section at the end
		mSections.resize(mSections.size() + 1);

		if (mSections.size() < 2)
		{
			Section& newSection = mSections.back();

			detail::updateTransform(newSection.mStartTransform, *mEntity1, mBoneName1);
			detail::updateTransform(newSection.mEndTransform, *mEntity2, mBoneName2);
		}
		else
		{
			const uint32 baseIndex = static_cast<uint32>(mSections.size()-2);

			// Update the formerly last section and the new one
			Section& section1 = mSections[baseIndex];
			Section& section2 = mSections.back();

			// Get split transformation
			qsf::Transform splitTransform;
			{
				glm::vec3 splitPosition;
				glm::vec3 splitDirection;

				SimpleBarrierTapeMeshGenerator::SectionSpline spline;
				section1.mMeshGenerator->buildSectionSpline(*this, baseIndex, spline);
				section1.mMeshGenerator->evaluateSectionSpline(spline, 0.8f, splitPosition, splitDirection);

				// TODO(tl): Warning tape doesn't fall on the ground
				//detail::dropToGround(getEntity().getMap(), splitPosition);
				splitDirection.y = 0.0f;

				splitTransform.setPositionAndRotation(splitPosition, qsf::Math::getLookAtQuaternion(splitDirection, qsf::CoordinateSystem::getUp()));
			}

			section1.mEndTransform = splitTransform;
			section2.mStartTransform = splitTransform;
			detail::updateTransform(section2.mEndTransform, *mEntity2, mBoneName2);

			// Update mesh of section1 one last time
			updateSectionMesh(baseIndex);
		}

		{ // Prepare mesh
			Section& newSection = mSections.back();
			const uint32 sectionIndex = static_cast<uint32>(mSections.size() - 1);

			newSection.mOgreMeshName = (boost::format("em5::SimpleBarrierTapeComponent_%1%_%2%.mesh") % getEntityId() % sectionIndex).str();
			newSection.mMeshGenerator = new SimpleBarrierTapeMeshGenerator();

			// Set transparency
			if (!mSections.empty())
			{
				setTransparency(newSection);
			}
		}
	}

	void SimpleBarrierTapeComponent::removeSection()
	{
		if (mSections.size() >= 2)
		{
			Section& section1 = mSections[mSections.size() - 2];
			Section& section2 = mSections.back();

			section1.mEndTransform = section2.mEndTransform;
		}

		// Get the OGRE scene manager instance
		qsf::GridSceneManager* gridSceneManager = static_cast<qsf::GridSceneManager*>(getEntity().getMap().getOgreSceneManager());
		if (nullptr != gridSceneManager)
		{
			Section& sectionToRemove = mSections.back();

			if (nullptr != sectionToRemove.mOgreEntity)
			{
				if (nullptr != sectionToRemove.mOgreSceneNode)
				{
					sectionToRemove.mOgreSceneNode->detachObject(sectionToRemove.mOgreEntity);
				}
				gridSceneManager->destroyEntity(sectionToRemove.mOgreEntity);
				sectionToRemove.mOgreEntity = nullptr;
			}

			if (nullptr != sectionToRemove.mOgreSceneNode)
			{
				gridSceneManager->destroySceneNode(sectionToRemove.mOgreSceneNode);
				sectionToRemove.mOgreSceneNode = nullptr;
			}

			// Remove mesh
			Ogre::v1::MeshManager::getSingleton().unload(sectionToRemove.mOgreMeshName);
			Ogre::v1::MeshManager::getSingleton().remove(sectionToRemove.mOgreMeshName);

			// Destroy the mesh generator instance
			delete sectionToRemove.mMeshGenerator;
		}

		// One section less
		mSections.pop_back();
	}

	void SimpleBarrierTapeComponent::updateSectionMesh(uint32 sectionIndex)
	{
		Section& section = mSections[sectionIndex];

		const bool isUpdated = section.mMeshGenerator->updateMesh(section.mOgreMeshName, *this, sectionIndex);
		if (!isUpdated)
		{
			// Remove old mesh
			Ogre::v1::MeshManager::getSingleton().unload(section.mOgreMeshName);
			Ogre::v1::MeshManager::getSingleton().remove(section.mOgreMeshName);

			// Create new mesh
			section.mMeshGenerator->createMesh(section.mOgreMeshName, *this, sectionIndex);

			// Get the OGRE scene manager instance
			qsf::GridSceneManager* gridSceneManager = static_cast<qsf::GridSceneManager*>(getEntity().getMap().getOgreSceneManager());
			if (nullptr != gridSceneManager)
			{
				if (nullptr == section.mOgreSceneNode)
				{
					section.mOgreSceneNode = gridSceneManager->createCellChildSceneNode(qsf::Convert::getOgreVector3(section.mStartTransform.getPosition()));
				}

				if (nullptr != section.mOgreEntity)
				{
					// Destroy transparency
					resetTransparency(section);

					// Destroy the old OGRE entity
					section.mOgreSceneNode->detachObject(section.mOgreEntity);
					gridSceneManager->destroyEntity(section.mOgreEntity);
					section.mOgreEntity = nullptr;
				}

				try
				{
					// Create new OGRE entity
					section.mOgreEntity = gridSceneManager->createEntity(section.mOgreMeshName);
					section.mOgreEntity->setName(std::to_string(getEntityId()));

					// Enable shine-through effect for the created entity
					section.mOgreEntity->setRenderQueueGroup(77);

					// Attach our OGRE entity to our OGRE scene node
					gridSceneManager->attachObjectToCellChildSceneNode(*section.mOgreSceneNode, *section.mOgreEntity);

					// Set transparency
					setTransparency(section);
				}
				catch (const std::exception& e)
				{
					// Error!
					QSF_ERROR("Failed to create OGRE entity with OGRE mesh name \"" << section.mOgreMeshName << ". Exception caught: " << e.what(), QSF_REACT_NONE);
				}
			}
		}
	}

	void SimpleBarrierTapeComponent::setTransparency(Section& section)
	{
		Ogre::v1::Entity* ogreEntity = section.mOgreEntity;
		if (nullptr != ogreEntity)
		{
			static qsf::MaterialVariationManager& materialVariationManager = QSF_MATERIAL.getMaterialVariationManager();

			materialVariationManager.setEntityMaterialPropertyValue(*ogreEntity, "SrcBlend", qsf::MaterialPropertyValue::fromBlend((mTransparency != 0.0f) ? qsf::Renderer::Blend::SRC_ALPHA : qsf::Renderer::Blend::ONE));
			materialVariationManager.setEntityMaterialPropertyValue(*ogreEntity, "DestBlend", qsf::MaterialPropertyValue::fromBlend((mTransparency != 0.0f) ? qsf::Renderer::Blend::INV_SRC_ALPHA : qsf::Renderer::Blend::ZERO));
			materialVariationManager.setEntityMaterialPropertyValue(*ogreEntity, "IsDecal", qsf::MaterialPropertyValue::fromBoolean(mTransparency != 0.0f));
			materialVariationManager.setEntityMaterialPropertyValue(*ogreEntity, "DiffuseColor", qsf::MaterialPropertyValue::fromFloat4(1.0f, 1.0f, 1.0f, 1.0f - mTransparency));
		}
	}

	void SimpleBarrierTapeComponent::resetTransparency(Section& section)
	{
		Ogre::v1::Entity* ogreEntity = section.mOgreEntity;
		if (nullptr != ogreEntity)
		{
			QSF_MATERIAL.getMaterialVariationManager().resetEntityMaterialPropertyValues(*ogreEntity);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

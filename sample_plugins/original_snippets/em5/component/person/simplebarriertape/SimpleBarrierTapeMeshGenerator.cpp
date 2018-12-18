// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/person/simplebarriertape/SimpleBarrierTapeMeshGenerator.h"
#include "em5/component/person/simplebarriertape/SimpleBarrierTapeComponent.h"

#include <qsf/map/Entity.h>
#include <qsf/map/query/GroundMapQuery.h>
#include <qsf/asset/AssetProxy.h>
#include <qsf/math/Math.h>
#include <qsf/math/CoordinateSystem.h>

#include <OGRE/OgreRoot.h>	// TODO(fw): Break down...


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SimpleBarrierTapeMeshGenerator::SimpleBarrierTapeMeshGenerator()
	{
		// Nothing to do in here
	}

	SimpleBarrierTapeMeshGenerator::~SimpleBarrierTapeMeshGenerator()
	{
		// Nothing to do in here
	}

	bool SimpleBarrierTapeMeshGenerator::createMesh(const std::string& ogreMeshName, const SimpleBarrierTapeComponent& barriertapeComponent, uint32 sectionIndex)
	{
		fillMeshGenerator(ogreMeshName, barriertapeComponent, sectionIndex);

		// Create the mesh now
		createOgreMesh(ogreMeshName);

		// Success
		return true;
	}

	bool SimpleBarrierTapeMeshGenerator::updateMesh(const std::string& ogreMeshName, const SimpleBarrierTapeComponent& barriertapeComponent, uint32 sectionIndex)
	{
		fillMeshGenerator(ogreMeshName, barriertapeComponent, sectionIndex);

		// Try to update the mesh now
		return updateOgreMesh(ogreMeshName);
	}

	void SimpleBarrierTapeMeshGenerator::buildSectionSpline(const SimpleBarrierTapeComponent& barriertapeComponent, uint32 sectionIndex, SectionSpline& outSpline)
	{
		const SimpleBarrierTapeComponent::Section& section = barriertapeComponent.mSections[sectionIndex];

		const glm::vec3& startPosition = section.mStartTransform.getPosition();
		const glm::vec3& endPosition = section.mEndTransform.getPosition();

		const float length = glm::distance(startPosition, endPosition);

		{ // Build cubic spline

			// Little hack to bend the tape lessr
			const glm::vec3 direction = glm::normalize(endPosition - startPosition);
			const glm::vec3 startDirection = glm::mix(section.mStartTransform.getRotation() * qsf::CoordinateSystem::getIn(), direction, 0.5f) * length;
			const glm::vec3 endDirection = glm::mix(section.mEndTransform.getRotation() * qsf::CoordinateSystem::getIn(), direction, 0.5) * length;

			outSpline.mCoefficients[3] = -2.0f * endPosition + endDirection + 2.0f * startPosition + startDirection;
			outSpline.mCoefficients[2] =  3.0f * endPosition - endDirection - 3.0f * startPosition - 2.0f * startDirection;
			outSpline.mCoefficients[1] = startDirection;
			outSpline.mCoefficients[0] = startPosition;
		}
	}

	void SimpleBarrierTapeMeshGenerator::evaluateSectionSpline(const SectionSpline& spline, float t, glm::vec3& outPosition, glm::vec3& outDirection)
	{
		const float t2 = t*t;
		const float t3 = t*t*t;

		// Evaluate spline
		outPosition = spline.mCoefficients[3] * t3 + spline.mCoefficients[2] * t2 + spline.mCoefficients[1] * t + spline.mCoefficients[0];

		// Evaluate spline first derivative
		outDirection = 3.0f * spline.mCoefficients[3] * t2 + 2.0f * spline.mCoefficients[2] * t + spline.mCoefficients[1];
	}


	void SimpleBarrierTapeMeshGenerator::fillMeshGenerator(const std::string& ogreMeshName, const SimpleBarrierTapeComponent& barriertapeComponent, uint32 sectionIndex)
	{
		const uint32 BARRIERTAPE_GRAPHICS_DETAIL = 20;		// Number of cylindrical parts of one section
		const float BARRIERTAPE_GRAPHICS_WIDTH = 0.1f;		// Width of the graphics
		const float BARRIERTAPE_SAGGING = 0.03f;			// Sagging of the tape
		const float wrapUDistance = 0.5f;					// Meters along the tape until the hose texture repeats

		// Get the tape section to build geometry for
		const SimpleBarrierTapeComponent::Section& section = barriertapeComponent.mSections[sectionIndex];

		// Build points along tape hose
		glm::vec3 points[BARRIERTAPE_GRAPHICS_DETAIL+1];
		glm::vec3 directions1[BARRIERTAPE_GRAPHICS_DETAIL+1];
		glm::vec3 directions2[BARRIERTAPE_GRAPHICS_DETAIL+1];
		{
			// Update length
			const glm::vec3 startPosition = section.mStartTransform.getPosition();
			const glm::vec3 endPosition = section.mEndTransform.getPosition();
			section.mLength = glm::distance(startPosition, endPosition);

			// Build cubic spline
			SectionSpline spline;
			buildSectionSpline(barriertapeComponent, sectionIndex, spline);

			// TODO(sw) Uses deprecated ground map query constructor
			qsf::GroundMapQuery groundMapQuery(barriertapeComponent.getEntity().getMap(), nullptr, 0);

			for (uint32 i = 0; i <= BARRIERTAPE_GRAPHICS_DETAIL; ++i)
			{
				const float t = static_cast<float>(i) / static_cast<float>(BARRIERTAPE_GRAPHICS_DETAIL);

				// Evaluate spline
				glm::vec3 derivative;
				evaluateSectionSpline(spline, t, points[i], derivative);

				// Hanging through
				points[i].y += ((t - 0.5f) * (t - 0.5f) - 0.25f) * section.mLength * BARRIERTAPE_SAGGING;

				// Get the ground height at the given position
				float height = points[i].y;
				if (groundMapQuery.getHeightAtXZPosition(points[i].x, points[i].z, height))
				{
					points[i].y = std::max(points[i].y, height + BARRIERTAPE_GRAPHICS_WIDTH * 0.75f);
				}

				// The mesh is positioned relative to the given start position
				points[i] -= startPosition;

				// Evaluate spline first derivative to get directions
				if (derivative == qsf::Math::GLM_VEC3_ZERO)
				{
					derivative = qsf::Math::GLM_VEC3_UNIT_X;
				}
				const glm::quat rotation = qsf::Math::getLookAtQuaternion(derivative, qsf::CoordinateSystem::getUp());
				directions1[i] = rotation * qsf::CoordinateSystem::getRight();
				directions2[i] = rotation * qsf::CoordinateSystem::getUp();
			}
		}

		// Build vertices
		{
			const float startU = (sectionIndex > 0) ? (barriertapeComponent.mSections[sectionIndex].mLength / wrapUDistance) : 0.0f;

			// Create enough vertices
			const uint32 vertexCount = (BARRIERTAPE_GRAPHICS_DETAIL + 1) * 2;
			mVertices.resize(vertexCount);
			RenderVertex* renderVertex = &mVertices[0];

			for (uint32 i = 0; i <= BARRIERTAPE_GRAPHICS_DETAIL; ++i)
			{
				const float t = static_cast<float>(i) / static_cast<float>(BARRIERTAPE_GRAPHICS_DETAIL);
				const float currentU = startU + t * section.mLength / wrapUDistance;

				for (uint32 k = 0; k < 2; ++k)
				{
					const float currentV = k == 0 ? 0.0f : 0.13f;
					const glm::vec3 position = points[i] + directions2[i] * BARRIERTAPE_GRAPHICS_WIDTH * 0.5f * (k == 0 ? 1.0f : -1.0f);
					const glm::vec3 normal = directions1[i];

					renderVertex->position			= glm::vec4(position, 1.0f);
					renderVertex->normal			= glm::vec4(normal, 0.0f);
					renderVertex->textureCoordinate = glm::vec2(currentU, currentV);
					++renderVertex;
				}
			}
		}

		// Create render sub mesh
		mRenderSubMesh.resize(1);
		RenderSubMesh& renderSubMesh = mRenderSubMesh.back();

		// Get material name
		{
			const qsf::AssetProxy assetProxy("em5/material/default/equipment_police_barrier_tape");
			const qsf::GlobalAssetId globalAssetId = assetProxy.getGlobalAssetId();
			QSF_CHECK(qsf::isInitialized(globalAssetId), "Could not find warning tape material asset named \"" << assetProxy.getLocalAssetName() << "\"", QSF_REACT_NONE);

			renderSubMesh.materialName = std::to_string(globalAssetId);
		}

		// Build indices
		{
			const uint32 indexCount = BARRIERTAPE_GRAPHICS_DETAIL * 6;
			renderSubMesh.indices.resize(indexCount);

			for (uint32 i = 0; i < BARRIERTAPE_GRAPHICS_DETAIL; ++i)
			{
				renderSubMesh.indices[i * 6 + 0] = i * 2 + 0;
				renderSubMesh.indices[i * 6 + 1] = i * 2 + 3;
				renderSubMesh.indices[i * 6 + 2] = i * 2 + 1;
				renderSubMesh.indices[i * 6 + 3] = i * 2 + 0;
				renderSubMesh.indices[i * 6 + 4] = i * 2 + 2;
				renderSubMesh.indices[i * 6 + 5] = i * 2 + 3;
			}
		}
	}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5

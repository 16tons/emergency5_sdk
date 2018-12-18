// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"
#include "qsf/math/PlanarProjector.h"

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	// Defines a coordinate system to use.
	// Currently only a very simple system stub but by starting to use this interface the code is prepared for a later change
	class QSF_API_EXPORT CoordinateSystem
	{
	public:
		// Main axis access
		//@{
		static const glm::vec3& getUp();
		static const glm::vec3& getRight();
		static const glm::vec3& getIn();
		//@}

		// Access the projections onto the planes each ignoring one dimension
		//@{
		static const PlanarProjector<float>& getTopViewProjector();
		static const PlanarProjector<float>& getFrontViewProjector();
		static const PlanarProjector<float>& getSideViewProjector();
		//@}

		static bool isRightHanded();
		static bool isLeftHanded();

	private:
		static const PlanarProjector<float> mTopViewProjector;
		static const PlanarProjector<float> mFrontViewProjector;
		static const PlanarProjector<float> mSideViewProjector;
	};


	// inline implementation
	inline const glm::vec3& CoordinateSystem::getUp()
	{
		static const glm::vec3 gUp(0.0f, 1.0f, 0.0f);
		return gUp;
	}

	inline const glm::vec3& CoordinateSystem::getRight()
	{
		static const glm::vec3 gRight(1.0f, 0.0f, 0.0f);
		return gRight;
	}

	inline const glm::vec3& CoordinateSystem::getIn()
	{
		static const glm::vec3 gIn(0.0f, 0.0f, -1.0f);
		return gIn;
	}

	inline const PlanarProjector<float>& CoordinateSystem::getTopViewProjector()
	{
		return mTopViewProjector;
	}

	inline const PlanarProjector<float>& CoordinateSystem::getFrontViewProjector()
	{
		return mFrontViewProjector;
	}

	inline const PlanarProjector<float>& CoordinateSystem::getSideViewProjector()
	{
		return mSideViewProjector;
	}

	inline bool CoordinateSystem::isRightHanded()
	{
		return true;
	}

	inline bool CoordinateSystem::isLeftHanded()
	{
		return !isRightHanded();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf

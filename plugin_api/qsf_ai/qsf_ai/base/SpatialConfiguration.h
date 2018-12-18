// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <glm/fwd.hpp>

namespace qsf
{
	namespace ai
	{
		// Minor helper class that holds a combination of a position and a direction in 3D as a tiny spatial configuration.
		// This setup is used throughout several AI algorithms.
		template <typename Vector>
		class SpatialConfiguration
		{
		public:
			SpatialConfiguration(); // Initializes to zero vectors, helpful for several reference parameter situations
			SpatialConfiguration(const Vector& pos, const Vector& dir);

			bool operator ==(const SpatialConfiguration<Vector>& other) const;
			bool operator !=(const SpatialConfiguration<Vector>& other) const;

			Vector mPosition;
			Vector mDirection;
		};


		// inline implementation
		template <typename Vector>
		SpatialConfiguration<Vector>::SpatialConfiguration()
		{}

		template <typename Vector>
		SpatialConfiguration<Vector>::SpatialConfiguration(const Vector& pos, const Vector& dir) :
			mPosition(pos),
			mDirection(dir)
		{}

		template <typename Vector>
		bool SpatialConfiguration<Vector>::operator ==(const SpatialConfiguration<Vector>& other) const
		{
			return mPosition == other.mPosition && mDirection == other.mDirection;
		}

		template <typename Vector>
		bool SpatialConfiguration<Vector>::operator !=(const SpatialConfiguration<Vector>& other) const
		{
			return !(*this == other);
		}

		typedef SpatialConfiguration<glm::vec2> SpatialConfiguration2D;
		typedef SpatialConfiguration<glm::vec3> SpatialConfiguration3D;
	}
}

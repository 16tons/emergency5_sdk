// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <glm/glm.hpp>

#include <vector>


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
	*    Class representing a complex (e.g. it is not enforced to be convex or non-overlapping) polygon
	*/
	class QSF_API_EXPORT Complex2DPolygon
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/** Default constructor */
		Complex2DPolygon();

		/**
		*  @brief
		*    Removes all nodes of the polygon
		*/
		void clear();

		/**
		*  @brief
		*    Adds a new nodes to the polygon
		*/
		void addNode(const glm::vec2& node);

		/**
		*  @brief
		*   Tests if a point is inside the polygon
		*/
		bool isPointInPolygon(const glm::vec2& point) const;

		/**
		*  @brief
		*    Computes the distance of any point to the polygon.
		*    If the point is inside the polygon, the distance is 0
		*/
		float distanceToPolygon(const glm::vec2& point) const;

		/**
		*  @brief
		*    Returns the point on the edges of the polygon which is nearest to
		*    the given reference point or the reference point itself if it lies
		*    on the inside of the polygon.
		*/
		glm::vec2 getNearestPointOnPolygon(const glm::vec2& point) const;

		/**
		*  @brief
		*    Finds the edge in the polygon which is nearest to the given point
		*
		*  @return
		*    Will return "false" if c is inside the polygon (distance is 0 then)
		*/
		bool findNearestEdgeOnPolygon(const glm::vec2& point, uint32& outEdge, float& outSquaredDistance) const;


	//[-------------------------------------------------------]
	//[ Protected members                                     ]
	//[-------------------------------------------------------]
	protected:
		std::vector<glm::vec2> mNodes;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
